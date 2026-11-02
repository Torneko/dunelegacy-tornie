#!/usr/bin/env python3
"""Validate a graphics-only replacement of the existing 547 release.

Gameplay checks from the initial release remain explicitly inherited. This
report only attributes CTest, alignment and startup checks to the new packages.
"""
import argparse
import datetime
import hashlib
import json
import re
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BASE = '7c5149220fd0ee869205677127ae6c944e2f3113'
INITIAL_DOCUMENTS = '80227a263afcafebaf5094f939d9c787eae23149'
VERSION = '1.0.547'
SCOPE = 'super-tank-alignment-547'
GRAPHICS = {
    'src/units/SuperTank.cpp',
    'src/MapEditor/MapEditor.cpp',
    'src/FileClasses/GFXManager.cpp',
    'include/units/SuperTankRender.h',
}


def require(condition, message):
    if not condition:
        raise SystemExit(message)


def git(*args):
    return subprocess.check_output(['git', *args], cwd=ROOT)


def blob(ref, path):
    return git('show', ref + ':' + path)


def load(path):
    return json.loads(path.read_text(encoding='utf-8-sig'))


def digest(path):
    h = hashlib.sha256()
    with path.open('rb') as stream:
        while chunk := stream.read(8 * 1024 * 1024):
            h.update(chunk)
    return h.hexdigest()


def changed_lines(source, path):
    diff = git('diff', '--no-ext-diff', '--unified=0', BASE, source, '--', path).decode('utf-8', errors='replace')
    return [(line[0], line[1:]) for line in diff.splitlines()
            if line.startswith(('+', '-')) and not line.startswith(('+++', '---'))]


def without_render_include(data):
    return re.sub(rb'(?m)^[ \t]*#include[ \t]+[<"]units/SuperTankRender\.h[>"][ \t]*\r?\n', b'', data)


def mask_body(data, marker):
    """Mask one drawing function, retaining all constructors/gameplay methods."""
    require(data.count(marker) == 1, 'Missing/ambiguous drawing function ' + marker.decode())
    start = data.index(b'{', data.index(marker))
    depth, state, i = 0, 'code', start
    while i < len(data):
        char, pair = data[i:i + 1], data[i:i + 2]
        if state == 'line':
            if char == b'\n':
                state = 'code'
        elif state == 'block':
            if pair == b'*/':
                state, i = 'code', i + 1
        elif state in ('"', "'"):
            if char == b'\\':
                i += 1
            elif char == state.encode():
                state = 'code'
        elif pair in (b'//', b'/*'):
            state, i = ('line' if pair == b'//' else 'block'), i + 1
        elif char in (b'"', b"'"):
            state = char.decode()
        elif char == b'{':
            depth += 1
        elif char == b'}':
            depth -= 1
            if depth == 0:
                return data[:start] + b'{ /* drawing body */ }' + data[i + 1:]
        i += 1
    raise SystemExit('Unterminated drawing function ' + marker.decode())


def verify_super_tank_graphics(source):
    """Remove the exact SDL-only compositor and its Super Tank call changes."""
    path = 'src/FileClasses/GFXManager.cpp'
    old = without_render_include(blob(BASE, path)).replace(b'\r\n', b'\n')
    new = without_render_include(blob(source, path)).replace(b'\r\n', b'\n')
    helper = b'''// The authored Super Tank turret has its own palette. Compose in true colour
// so thumbnail generation preserves the same colours and transparency as play.
static sdl2::surface_ptr composeSuperTankEditorPicture(SDL_Surface* base, SDL_Surface* gun, int x, int y) {
    if(!base || !gun) return nullptr;
    auto picture=sdl2::surface_ptr(SDL_ConvertSurfaceFormat(base,SDL_PIXELFORMAT_RGBA32,0));
    if(!picture) return nullptr;
    SDL_SetSurfaceBlendMode(gun,SDL_BLENDMODE_BLEND);
    SDL_Rect destination{x,y,gun->w,gun->h};
    if(SDL_BlitSurface(gun,nullptr,picture.get(),&destination)!=0) return nullptr;
    return picture;
}

'''
    require(new.count(helper) == 1, 'Super Tank compositor differs from the reviewed SDL-only drawing helper')
    new = new.replace(helper, b'', 1)
    guard = b'''        if(gunObjPicID==ObjPic_SuperTankGun)
            return composeSuperTankEditorPicture(base.get(),gun.get(),gunOffsetX,gunOffsetY);
'''
    require(new.count(guard) == 2, 'Super Tank editor compositor guards differ')
    new = new.replace(guard, b'')
    for marker in (b'uiGraphic[UI_MapEditor_SuperTank][HOUSE_HARKONNEN] =',
                   b'auto superTank = composeEditorVehicle(',
                   b'auto superTank = composeRuntimeEditorVehicle('):
        matches = [line for line in old.splitlines(keepends=True) if marker in line]
        require(len(matches) == 1, 'Missing/ambiguous original Super Tank compositor call')
        original = matches[0]
        replacement = original.replace(b', 2, -4);', b', SuperTankRender::editorOffset().x, SuperTankRender::editorOffset().y);')
        replacement = replacement.replace(b',2,-4);', b',SuperTankRender::editorOffset().x,SuperTankRender::editorOffset().y);')
        if marker.startswith(b'uiGraphic'):
            replacement = replacement.replace(b'= combinePictures(', b'= composeSuperTankEditorPicture(')
        require(new.count(replacement) == 1, 'Super Tank compositor call differs from the reviewed placement change')
        new = new.replace(replacement, original, 1)
    require(old == new, 'Non-Super-Tank graphics or gameplay logic changed in ' + path)


def visual_proof(source):
    changed = git('diff', '--no-renames', '--name-only', BASE, source).decode().splitlines()
    protected = [p for p in changed if p.startswith(('config/', 'data/', 'mods/'))
                 or p in ('CMakeLists.txt', 'vcpkg.json', 'vcpkg-configuration.json')]
    require(not protected, 'Gameplay data/build version changed: ' + ', '.join(protected))
    runtime = [p for p in changed if p.startswith(('src/', 'include/'))]
    qa_friends = []
    for path in runtime:
        if path in GRAPHICS:
            continue
        if path == 'src/main.cpp':
            for sign, text in changed_lines(source, path):
                require(sign == '+' or not text.strip(), 'Existing main flow was removed: ' + text)
                require(not text.strip() or 'SuperTankAlignment' in text or '--verify-super-tank-alignment' in text,
                        'Unrelated main flow changed: ' + text)
            continue
        if path == 'include/MapEditor/MapEditor.h':
            old, new = blob(BASE, path), blob(source, path)
            friend = rb'(?m)^[ \t]*friend[ \t]+struct[ \t]+SuperTankAlignmentFixture[ \t]*;[ \t]*\r?\n'
            require(len(re.findall(friend, new)) == 1 and re.sub(friend, b'', new) == old,
                    'MapEditor header change exceeds the alignment QA friend')
            qa_friends.append(path)
            continue
        raise SystemExit('Unexpected non-rendering runtime source change: ' + path)

    for path, marker in [('src/units/SuperTank.cpp', b'void SuperTank::blitToScreen('),
                         ('src/MapEditor/MapEditor.cpp', b'void MapEditor::drawMap(')]:
        old = mask_body(without_render_include(blob(BASE, path)), marker).replace(b'\r\n', b'\n')
        new = mask_body(without_render_include(blob(source, path)), marker).replace(b'\r\n', b'\n')
        require(old == new, 'Non-drawing logic changed in ' + path)
    verify_super_tank_graphics(source)
    header = blob(source, 'include/units/SuperTankRender.h')
    require(not re.search(rb'\b(?:currentGame|currentGameMap|pLocalHouse|unitList|structureList|randomGen|objectData|setHealth|setPrice|bulletType|numWeapons|SAVEGAMEVERSION|NETWORK_PROTOCOL_VERSION)\b', header),
            'Rendering header references gameplay state')

    save = int(re.search(rb'#define SAVEGAMEVERSION\s+(\d+)', blob(source, 'include/Definitions.h')).group(1))
    protocol = int(re.search(rb'#define NETWORK_PROTOCOL_VERSION\s+(\d+)', blob(source, 'include/Network/NetworkManager.h')).group(1))
    require(save == 9840 and protocol == 25, 'Save format or network protocol changed')
    require(re.search(rb'project\(DuneLegacyTornie VERSION 1\.0\.547\b', blob(source, 'CMakeLists.txt')),
            'Game version changed')
    require(json.loads(blob(source, 'vcpkg.json'))['version'] == VERSION, 'Dependency manifest version changed')
    config = json.loads(blob(source, 'config/Release547.json'))
    require(config['superTank']['price'] == 925 and config['superTank']['hitPoints'] == 300
            and config['superTank']['itemID'] == 88, 'Super Tank statistics or identity changed')

    artwork = json.loads(blob(BASE, 'docs/RELEASE-1.0.545-assets.json'))['activeResources']
    frames = {item['path']: item['sha256'] for item in json.loads(blob(BASE, 'release_notes/Preparation-1.0.546.json'))['frames']}
    for item in artwork:
        item['sha256'] = frames.get(item['runtime'], item['sha256'])
    artwork += json.loads(blob(BASE, 'docs/RELEASE-1.0.547-assets.json'))['activeResources']
    require(len(artwork) == 22, 'Original artwork inventory changed')
    originals = []
    for item in artwork:
        path = item['runtime']
        expected = hashlib.sha256(blob(BASE, path)).hexdigest()
        require(expected == item['sha256'] and hashlib.sha256(blob(source, path)).hexdigest() == expected,
                'Original artwork changed: ' + path)
        originals.append({'path': path, 'sha256': expected})
    return dict(baseSource=BASE, changedRuntimeFiles=runtime, graphicsAllowlist=sorted(GRAPHICS),
                qaEntryPoint='src/main.cpp', qaFriendDeclarations=qa_friends,
                gameplayConfigurationDataAndModsByteIdentical=True,
                otherGameplaySourcesByteIdentical=True,
                superTankNonDrawingCodeUnchanged=True, editorNonDrawingCodeUnchanged=True,
                networkProtocol=protocol, saveFormat=save, price=925, hitPoints=300, itemID=88,
                originalArtwork=originals)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--staged', type=Path, required=True)
    parser.add_argument('--source', required=True)
    parser.add_argument('--windows-run', type=int, required=True)
    parser.add_argument('--linux-run', type=int, required=True)
    args = parser.parse_args()
    source = git('rev-parse', '--verify', args.source + '^{commit}').decode().strip()
    require(source != BASE, 'Replacement source is still the original release')
    proof = visual_proof(source)
    platforms = {}
    for platform, run in [('windows', args.windows_run), ('linux', args.linux_run)]:
        folder = args.staged / platform
        report = load(folder / (platform.title() + '-Build-' + VERSION + '.json'))
        require(report['source'] == source and report['version'] == VERSION and report['run'] == run,
                platform + ' provenance differs from the verified source/run')
        require(report['validationScope'] == SCOPE and report['engineAndNetworkTests']
                and report['networkProtocol'] == 25 and report['saveVersion'] == 9840
                and set(report['alignmentChecks']) == {'fr', 'en'}, platform + ' validation scope differs')
        if platform == 'windows':
            require(all(set(report[key]) == {'fr', 'en'} for key in
                        ['installedRuntimeChecks', 'menuStartupChecks', 'videoOptionsChecks']),
                    'Windows FR/EN startup checks missing')
            require(report['package'] == 'DuneLegacyTornie-1.0.547-Windows-x64.zip', 'Windows package name differs')
            require(digest(folder / report['package']) == report['sha256'], 'Windows package SHA differs')
        else:
            startup = report['graphicalStartup']
            require(startup['passed'] and report['softwareFallback']
                    and set(startup['languages']) == {'fr', 'en'}
                    and set(startup['images']) == {'standard', 'Mint-compatible'}, 'Linux startup coverage differs')
            packages = report['packages']
            require(len(packages) == 5 and len({p['package'] for p in packages}) == 5, 'Linux package inventory differs')
            require(all(sum(p['package'].endswith(suffix) for p in packages) == count
                        for suffix, count in [('.AppImage', 2), ('.deb', 1), ('.rpm', 1), ('.tar.gz', 1)]),
                    'Linux package types differ')
            for package in packages:
                path = folder / package['package']
                require(path.is_file() and path.stat().st_size == package['size']
                        and digest(path) == package['sha256'], 'Linux package SHA/size differs: ' + path.name)
        platforms[platform] = report
    audit = load(args.staged / ('Package-Resources-Audit-' + VERSION + '.json'))
    require(audit['source'] == source and audit['version'] == VERSION
            and audit['originalArtworkFiles'] == 22 and audit['vanillaFiles'] == 205
            and len(audit['packages']) == 4 and all(p['passed'] for p in audit['packages'].values()),
            'Package artwork/Vanilla/resource audit differs')
    expected_audits = {'windows', 'linuxTar'} | {
        p['package'] for p in platforms['linux']['packages'] if p['package'].endswith('.AppImage')}
    require(set(audit['packages']) == expected_audits, 'Audited package inventory differs')
    require(audit['packages']['windows']['sha256'] == platforms['windows']['sha256'], 'Audited Windows ZIP differs')
    for package in platforms['linux']['packages']:
        key = 'linuxTar' if package['package'].endswith('.tar.gz') else package['package']
        if key in audit['packages']:
            require(audit['packages'][key]['sha256'] == package['sha256'], 'Audited Linux package differs')

    initial_path = ROOT / 'release_notes/initial-1.0.547/Validation-1.0.547.json'
    if initial_path.exists():
        initial_raw = initial_path.read_bytes()
    else:
        current = ROOT / 'release_notes/Validation-1.0.547.json'
        initial_raw = current.read_bytes()
        if json.loads(initial_raw.decode('utf-8-sig')).get('source') != BASE:
            initial_raw = blob(INITIAL_DOCUMENTS, 'release_notes/Validation-1.0.547.json')
    initial = json.loads(initial_raw.decode('utf-8-sig'))
    require(initial['version'] == VERSION and initial['source'] == BASE
            and initial['status'] == 'release-verified', 'Initial release validation is not the published baseline')
    report = dict(version=VERSION, status='release-verified', validationScope=SCOPE,
                  date='2026-11-01', verifiedAt=datetime.datetime.now(datetime.timezone.utc).isoformat(),
                  source=source, networkProtocol=25, saveFormat=9840, platforms=platforms,
                  packageResources=audit, visualOnlyProof=proof,
                  inheritedGameplayValidation=dict(source=BASE,
                      document='release_notes/initial-1.0.547/Validation-1.0.547.json',
                      sha256=hashlib.sha256(initial_raw).hexdigest(),
                      description='Initial gameplay, campaign and memory checks remain historical; they are not claimed as rerun for this graphics-only correction.'),
                  currentChecks=['engine/network CTest', 'FR/EN eight-direction three-zoom game/editor/preview alignment',
                                 'FR/EN Windows menus/video', 'both Linux AppImages startup and software fallback',
                                 'unchanged original artwork and Vanilla/package resources'],
                  limits=initial.get('limits', []))
    initial_path.parent.mkdir(parents=True, exist_ok=True)
    if not initial_path.exists():
        initial_path.write_bytes(initial_raw)
    encoded = json.dumps(report, ensure_ascii=False, indent=2) + '\n'
    for path in [ROOT / 'release_notes/Validation-1.0.547.json', ROOT / 'website/documents/Validation-1.0.547.json']:
        path.write_text(encoded, encoding='utf-8', newline='\n')
    print('Verified graphics-only 547 replacement:', source)
    print('Initial rich gameplay validation preserved:', initial_path.relative_to(ROOT).as_posix())


if __name__ == '__main__':
    main()
