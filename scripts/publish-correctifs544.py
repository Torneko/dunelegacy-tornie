#!/usr/bin/env python3
"""Promote the two verified 544 package runs and the matching bilingual site."""
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tarfile
import time
import urllib.request
import zipfile

VERSION = '1.0.544'
TAG = 'v' + VERSION
REPO = os.environ['GITHUB_REPOSITORY']
SOURCE = os.environ['PACKAGE_SOURCE']
SITE = os.environ['WEBSITE_SOURCE']
OLD_TAG = 'd979cea6ab8d2554ac7ab19d1444dcdf182ee245'
OLD_SITE = '69dec6d4c434fe275903493ec8f92d0fb5e29974'
ROOT = Path('.publish544').resolve()
SUPPORT = ROOT / 'supporting'
DIST = ROOT / 'offline/DuneLegacyTornie-Site-Local/dist'
STATE = ROOT / 'state.json'
PUBLIC = 'https://torneko.github.io/dunelegacy-tornie/'


def run(*args, capture=False):
    return subprocess.check_output(args, text=True).strip() if capture else subprocess.run(args, check=True)


def api(endpoint, *args):
    return json.loads(run('gh', 'api', 'repos/' + REPO + '/' + endpoint, *args, capture=True))


def write(path, data):
    path.write_text(json.dumps(data, ensure_ascii=False, indent=2) + '\n')


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def data_js(path):
    text = path.read_text()
    return json.loads(text[text.index('{'):].strip().removesuffix(';'))


def checker(*bases):
    env = dict(os.environ, RELEASE_VERSION=VERSION, NETWORK_PROTOCOL='20', REQUIRE_WINDOWS_VERSION=VERSION,
               SITE_VALIDATION_OUTPUT=str(SUPPORT / ('Website-Validation-' + VERSION + '.json')))
    subprocess.run(['node', 'scripts/check-documentation-site.cjs', *bases], env=env, check=True)


def assets():
    return [{'name': p.name, 'size': p.stat().st_size, 'sha256': digest(p)}
            for p in sorted(SUPPORT.iterdir()) if p.is_file() and not p.name.startswith('SHA256SUMS-')]


def upload(paths):
    run('gh', 'release', 'upload', TAG, *map(str, paths), '--repo', REPO, '--clobber')


def gate():
    # Only these superseded staging runs may be stopped. The original PR run
    # and every unrelated repository workflow are left alone.
    runs = api('actions/runs?branch=publish%2F544-correctifs&per_page=30')['workflow_runs']
    for item in runs:
        if (item['head_sha'] == '9ef0e1355e74faadae69bb965d474e218fa6a641'
                and item['status'] in ['pending', 'queued', 'in_progress']):
            run('gh', 'api', '--method', 'POST', 'repos/' + REPO + '/actions/runs/' + str(item['id']) + '/cancel')
    for attempt in range(180):
        runs = api('actions/runs?head_sha=' + SOURCE + '&per_page=30')['workflow_runs']
        selected = [r for r in runs if r['path'] in ['.github/workflows/build.yml', '.github/workflows/linux-release.yml']]
        assert not any(r['status'] == 'completed' and r['conclusion'] != 'success' for r in selected), 'A package run failed'
        if len(selected) == 2 and all(r['status'] == 'completed' and r['conclusion'] == 'success' for r in selected):
            print('PACKAGE GATE PASSED: both complete runs succeeded for ' + SOURCE, flush=True)
            return
        if attempt % 4 == 0:
            print('Waiting for the matching Windows/Linux package checks...', flush=True)
        time.sleep(15)
    raise RuntimeError('Package checks did not finish in 45 minutes')


def prepare():
    ROOT.mkdir(exist_ok=True)
    SUPPORT.mkdir(exist_ok=True)
    runs = api('actions/runs?head_sha=' + SOURCE + '&per_page=30')['workflow_runs']
    state = {'version': VERSION, 'source': SOURCE, 'preparedWebsite': SITE,
             'networkProtocol': 20, 'saveVersion': 9838, 'runs': {}}
    for platform, workflow, artifact in [('linux', 'linux-release.yml', 'DuneLegacyTornie-Linux-Packages'),
                                          ('windows', 'build.yml', 'DuneLegacyTornie-Windows-Installer')]:
        matches = [r for r in runs if r['path'] == '.github/workflows/' + workflow and r['head_sha'] == SOURCE
                   and r['status'] == 'completed' and r['conclusion'] == 'success']
        assert matches, platform + ' has no successful package run for the chosen source'
        selected = matches[0]
        state['runs'][platform] = selected['id']
        jobs = api('actions/runs/' + str(selected['id']) + '/jobs?per_page=100')['jobs']
        required = ['build-linux'] if platform == 'linux' else ['build-windows', 'test-linux', 'validate-french-locale']
        assert all(any(j['name'] == name and j['conclusion'] == 'success' for j in jobs) for name in required)
        target = ROOT / platform
        target.mkdir(exist_ok=True)
        run('gh', 'run', 'download', str(selected['id']), '--repo', REPO, '--name', artifact, '--dir', str(target))
        report = json.loads((target / (platform.title() + '-Build-' + VERSION + '.json')).read_text(encoding='utf-8-sig'))
        assert report['source'] == SOURCE and report['workflowSource'] == SOURCE
        assert report['run'] == selected['id'] and report['version'] == VERSION
        assert report['networkProtocol'] == 20 and report['saveVersion'] == 9838
        assert report['engineAndNetworkTests'] is True
        if platform == 'linux':
            assert report['graphicalStartup']['passed'] and report['softwareFallback']
            assert len(list(target.glob('*.AppImage'))) == 2
            assert all(len(list(target.glob(ext))) == 1 for ext in ['*.deb', '*.rpm', '*.tar.gz'])
            assert len(list(target.iterdir())) == 6
        else:
            assert report['installedRuntimeChecks'] == ['fr', 'en']
            assert report['menuStartupChecks'] == ['fr', 'en'] and report['videoOptionsChecks'] == ['fr', 'en']
            assert report['package'] == 'DuneLegacyTornie-' + VERSION + '-Windows-x64.zip'
            assert digest(target / report['package']) == report['sha256']
            assert len(list(target.iterdir())) == 2
        for path in target.iterdir():
            shutil.copy2(path, SUPPORT / path.name)

    for revision in [SOURCE, SITE, OLD_SITE]:
        run('git', 'fetch', '--no-tags', 'origin', revision)
    expected = run('git', 'ls-tree', '-r', '--name-only', SOURCE, 'mods', capture=True).splitlines()
    hashes = {name: hashlib.sha256(subprocess.check_output(['git', 'show', SOURCE + ':' + name])).hexdigest()
              for name in expected}
    resource_audit = {'source': SOURCE, 'files': len(hashes), 'packages': {}}
    with zipfile.ZipFile(next((ROOT / 'windows').glob('*.zip'))) as archive:
        for name, wanted in hashes.items():
            matches = [p for p in archive.namelist() if p == name or p.endswith('/' + name)]
            assert len(matches) == 1 and hashlib.sha256(archive.read(matches[0])).hexdigest() == wanted, name
        resource_audit['packages']['windows'] = True
    with tarfile.open(next((ROOT / 'linux').glob('*.tar.gz'))) as archive:
        members = archive.getmembers()
        for name, wanted in hashes.items():
            matches = [p for p in members if p.isfile() and (p.name == name or p.name.endswith('/' + name))]
            assert len(matches) == 1 and hashlib.sha256(archive.extractfile(matches[0]).read()).hexdigest() == wanted, name
        resource_audit['packages']['linuxTar'] = True
    write(SUPPORT / ('Package-Resources-Audit-Correctifs-' + VERSION + '.json'), resource_audit)

    DIST.mkdir(parents=True, exist_ok=True)
    with subprocess.Popen(['git', 'archive', SITE], stdout=subprocess.PIPE) as archive:
        subprocess.run(['tar', '-x', '-C', str(DIST)], stdin=archive.stdout, check=True)
        archive.stdout.close()
        assert archive.wait() == 0
    site_data = data_js(DIST / 'assets/site-data.js')
    assert site_data['version'] == VERSION and site_data['sourceCommit'] == SOURCE
    assert site_data['saveVersion'] == 9838 and site_data['rules544']['protocol'] == 20
    assert site_data['rules544']['patch']['status'] == 'released'
    assert site_data['localChanges'] is False and site_data['localPreparation'] is False
    originals = run('git', 'ls-tree', '-r', '--name-only', OLD_SITE, 'assets/png', capture=True).splitlines()
    preserved = 0
    for name in originals:
        if name.lower().endswith('.png'):
            original = subprocess.check_output(['git', 'show', OLD_SITE + ':' + name])
            assert (DIST / name).read_bytes() == original, 'Original image changed: ' + name
            preserved += 1
    state['preservedOriginalPngs'] = preserved
    state['websiteTree'] = run('git', 'rev-parse', SITE + '^{tree}', capture=True)
    run('git', 'archive', '--format=zip', '--prefix=DuneLegacyTornie-' + VERSION + '/',
        '-o', str(SUPPORT / ('DuneLegacyTornie-' + VERSION + '-Sources.zip')), SOURCE)
    for suffix, lang in [('', 'FR'), ('-EN', 'EN')]:
        shutil.copy2(DIST / ('documents/Notes-' + VERSION + suffix + '.md'), SUPPORT / ('Release-Notes-' + VERSION + '-' + lang + '.md'))
        shutil.copy2(DIST / ('documents/Correctifs-' + VERSION + '-' + lang + '.md'), SUPPORT / ('Correctifs-' + VERSION + '-' + lang + '.md'))
    for path in SUPPORT.iterdir():
        shutil.copy2(path, DIST / 'documents' / path.name)
    checker(DIST.as_uri() + '/')
    state['initialRelease'] = api('releases/tags/' + TAG)
    write(STATE, state)
    print('PREPARED: both platforms, exact source, original PNGs and offline website verified', flush=True)


def packages():
    state = json.loads(STATE.read_text())
    current = api('git/ref/tags/' + TAG)['object']
    assert current['type'] == 'commit' and current['sha'] in [OLD_TAG, SOURCE], 'Release tag moved unexpectedly'
    if current['sha'] != SOURCE:
        api('git/refs/tags/' + TAG, '--method', 'PATCH', '-f', 'sha=' + SOURCE, '-F', 'force=true')
    package_files = [p for p in SUPPORT.iterdir() if p.name.startswith(('DuneLegacyTornie-' + VERSION + '-', 'Linux-Build-', 'Windows-Build-'))]
    upload(package_files)
    body = (SUPPORT / ('Correctifs-' + VERSION + '-FR.md')).read_text() + '\n---\n\n' + (SUPPORT / ('Correctifs-' + VERSION + '-EN.md')).read_text()
    body += '\n\n[Site FR](https://torneko.github.io/dunelegacy-tornie/) · [English guide](https://torneko.github.io/dunelegacy-tornie/index-en.html)\n'
    notes = ROOT / 'release-body.md'
    notes.write_text(body)
    run('gh', 'release', 'edit', TAG, '--repo', REPO, '--title', '1.0.544 — Correctifs Slowpost, graphismes et Worfinery', '--notes-file', str(notes))
    state['packagesPublished'] = [p.name for p in package_files]
    write(STATE, state)
    print('PACKAGES_PUBLISHED: ready for the matching gh-pages merge', flush=True)


def wait_site():
    state = json.loads(STATE.read_text())
    for attempt in range(120):
        current = api('git/ref/heads/gh-pages')['object']['sha']
        commit = api('git/commits/' + current)
        if commit['tree']['sha'] == state['websiteTree']:
            try:
                request = urllib.request.Request(PUBLIC + 'assets/site-data.js?publication=' + SOURCE, headers={'Cache-Control': 'no-cache'})
                text = urllib.request.urlopen(request, timeout=30).read().decode()
                data = json.loads(text[text.index('{'):].strip().removesuffix(';'))
                if data['sourceCommit'] == SOURCE and data['rules544']['protocol'] == 20:
                    state['website'] = current
                    write(STATE, state)
                    print('MATCHING_WEBSITE_PUBLISHED: ' + current, flush=True)
                    return
            except Exception as error:
                print('Pages update pending: ' + str(error), flush=True)
        if attempt % 4 == 0:
            print('Waiting for the verified bilingual website to be published...', flush=True)
        time.sleep(15)
    raise RuntimeError('Matching published website did not become available')


def finalize():
    state = json.loads(STATE.read_text())
    assert state.get('website') and state.get('packagesPublished')
    checker(DIST.as_uri() + '/', PUBLIC)
    report = json.loads((DIST / ('documents/Validation-Correctifs-' + VERSION + '.json')).read_text())
    report.update(status='released', source=SOURCE, networkProtocol=20, saveVersion=9838)
    report['ci'] = {p: {'run': n, 'conclusion': 'success', 'url': 'https://github.com/' + REPO + '/actions/runs/' + str(n)} for p, n in state['runs'].items()}
    report['publication'] = {'authorized': True, 'gameTagReplaced': True, 'publicDownloadsReplaced': True,
                             'websitePublished': True, 'website': state['website'], 'preparedWebsite': SITE,
                             'websiteTree': state['websiteTree'], 'offlineWebsiteMatchesPublishedTree': True}
    report['website'].update(publicAndOfflineRoutes=72, preservedOriginalPngs=state['preservedOriginalPngs'])
    for name in ['Validation-Correctifs-', 'Validation-']:
        write(SUPPORT / (name + VERSION + '.json'), report)
    publication = {k: state[k] for k in ['version', 'source', 'website', 'preparedWebsite', 'websiteTree', 'runs', 'networkProtocol', 'saveVersion', 'preservedOriginalPngs']}
    publication['assets'] = assets()
    write(SUPPORT / ('Publication-Validation-' + VERSION + '.json'), publication)
    for path in SUPPORT.iterdir():
        shutil.copy2(path, DIST / 'documents' / path.name)
    checksums(SUPPORT, DIST / ('documents/SHA256SUMS-' + VERSION + '.txt'))
    archive_path = SUPPORT / ('DuneLegacyTornie-Site-Local-' + VERSION + '.zip')
    with zipfile.ZipFile(archive_path, 'w', zipfile.ZIP_DEFLATED) as archive:
        for path in sorted((ROOT / 'offline').rglob('*')):
            if path.is_file():
                archive.write(path, path.relative_to(ROOT / 'offline'))
    checksums(SUPPORT, SUPPORT / ('SHA256SUMS-' + VERSION + '.txt'))
    upload([p for p in SUPPORT.iterdir() if p.name not in state['packagesPublished']])
    print('DOCUMENTS_PUBLISHED: offline/public checks, sources, bilingual notes and checksums', flush=True)


def checksums(folder, target):
    target.write_text(''.join(digest(p) + '  ' + p.name + '\n' for p in sorted(folder.iterdir())
                              if p.is_file() and not p.name.startswith('SHA256SUMS-')))


def audit():
    state = json.loads(STATE.read_text())
    target = ROOT / 'downloaded'
    target.mkdir(exist_ok=True)
    run('gh', 'release', 'download', TAG, '--repo', REPO, '--dir', str(target))
    checks = (target / ('SHA256SUMS-' + VERSION + '.txt')).read_text().splitlines()
    for line in checks:
        sha, name = line.split('  ', 1)
        assert digest(target / name) == sha, 'Public download mismatch: ' + name
    expected = {line.split('  ', 1)[1] for line in checks} | {'SHA256SUMS-' + VERSION + '.txt'}
    assert {p.name for p in target.iterdir()} == expected, 'Unexpected unverified release assets'
    assert api('git/ref/tags/' + TAG)['object']['sha'] == SOURCE
    assert api('git/commits/' + api('git/ref/heads/gh-pages')['object']['sha'])['tree']['sha'] == state['websiteTree']
    result = {'passed': True, 'source': SOURCE, 'website': state['website'], 'websiteTree': state['websiteTree'],
              'networkProtocol': 20, 'saveVersion': 9838, 'downloadedAssets': len(expected),
              'publicAndOfflineRoutes': 72, 'assets': assets(), 'runs': state['runs']}
    write(SUPPORT / ('Final-Publication-Audit-Correctifs-' + VERSION + '.json'), result)
    checksums(SUPPORT, SUPPORT / ('SHA256SUMS-' + VERSION + '.txt'))
    upload([SUPPORT / ('Final-Publication-Audit-Correctifs-' + VERSION + '.json'), SUPPORT / ('SHA256SUMS-' + VERSION + '.txt')])
    print('PUBLICATION COMPLETE: ' + str(len(expected) + 1) + ' verified release assets', flush=True)


if __name__ == '__main__':
    {'gate': gate, 'prepare': prepare, 'packages': packages, 'wait-site': wait_site, 'finalize': finalize, 'audit': audit}[sys.argv[1]]()
