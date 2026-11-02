#!/usr/bin/env python3
"""Audit prepared 547 replacement assets locally, optionally checking server metadata.

No network requests are made. Source and documentation counts come from the
specified Git revisions; the existing release layout stays at 18 assets.
"""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import subprocess
import zipfile


VERSION = '1.0.547'
REPO = Path(__file__).resolve().parents[1]
SUMS = f'SHA256SUMS-{VERSION}.txt'
WINDOWS = f'DuneLegacyTornie-{VERSION}-Windows-x64.zip'
SOURCES = f'DuneLegacyTornie-{VERSION}-Sources.zip'
GUIDES = f'DuneLegacyTornie-{VERSION}-Guides-FR-EN.zip'
OFFLINE = f'DuneLegacyTornie-Site-Local-{VERSION}.zip'
WEB_VALIDATION = f'Website-Validation-{VERSION}.json'
PUBLICATION = f'Publication-Validation-{VERSION}.json'
EXPECTED = {
    WINDOWS, SOURCES, GUIDES, OFFLINE, SUMS, WEB_VALIDATION, PUBLICATION,
    f'DuneLegacyTornie-{VERSION}-Linux-Mint-Compatible.AppImage',
    f'DuneLegacyTornie-{VERSION}-Linux-x86_64.AppImage',
    *(f'DuneLegacyTornie-{VERSION}-Linux-x64.{suffix}' for suffix in ['deb', 'rpm', 'tar.gz']),
    *(f'{platform}-Build-{VERSION}.json' for platform in ['Windows', 'Linux']),
    *(f'Release-Notes-{VERSION}-{language}.md' for language in ['FR', 'EN']),
    f'Validation-{VERSION}.json', f'Package-Resources-Audit-{VERSION}.json',
}


def require(condition, message):
    if not condition:
        raise ValueError(message)


def git(*args):
    return subprocess.check_output(['git', *args], cwd=REPO)


def revision(value):
    return git('rev-parse', '--verify', value + '^{commit}').decode().strip()


def git_tree(ref, path=None):
    args = ['ls-tree', '-rz', ref] + ([path] if path else [])
    result = {}
    for row in git(*args).split(b'\0'):
        if not row:
            continue
        attrs, name = row.split(b'\t', 1)
        _mode, kind, sha = attrs.decode().split()
        if kind == 'blob':
            result[name.decode('utf-8')] = sha
    return result


def stream_digest(stream, algorithm='sha256', prefix=b''):
    h = hashlib.new(algorithm)
    h.update(prefix)
    while chunk := stream.read(8 * 1024 * 1024):
        h.update(chunk)
    return h.hexdigest()


def digest(path):
    with path.open('rb') as stream:
        return stream_digest(stream)


def manifest(data, label, expected_count):
    entries = {}
    lines = data.decode('utf-8-sig').splitlines()
    require(len(lines) == expected_count, f'{label}: expected {expected_count} checksum lines')
    for line in lines:
        match = re.fullmatch(r'([0-9a-fA-F]{64})  ([^/\\]+)', line)
        require(match is not None, f'{label}: invalid checksum entry {line!r}')
        sha, name = match.groups()
        require(name not in entries, f'{label}: duplicate entry {name}')
        require(name not in ('.', '..'), f'{label}: invalid filename')
        entries[name] = sha.lower()
    return entries


def archive(path):
    z = zipfile.ZipFile(path)
    names = z.namelist()
    require(len(names) == len(set(names)), f'{path.name}: duplicate ZIP entries')
    for name in names:
        parts = PurePosixPath(name).parts
        require(name and not name.startswith('/') and '\\' not in name
                and '..' not in parts and ':' not in parts[0],
                f'{path.name}: invalid archive path {name!r}')
    bad = z.testzip()
    require(bad is None, f'{path.name}: CRC failed for {bad}')
    return z


def files(z):
    return {entry.filename for entry in z.infolist() if not entry.is_dir()}


def verify_blob(z, name, expected):
    # The Git object format determines the blob digest, not ZIP timestamps or modes.
    algorithm = 'sha1' if len(expected) == 40 else 'sha256'
    size = z.getinfo(name).file_size
    with z.open(name) as stream:
        actual = stream_digest(stream, algorithm, f'blob {size}\0'.encode())
    require(actual == expected, f'ZIP file differs from Git: {name}')


def guide_path(name):
    p = PurePosixPath(name)
    return (name in ['HOWTOPLAY.md', 'HOWTOPLAY-FR.md', 'README.md', 'AUTHORS', 'COPYING']
            or (str(p.parent) in ['docs', 'release_notes', 'website/documents'] and p.suffix == '.md')
            or str(p.parent) == 'release_notes/previews'
            or (str(p.parent) == 'release_notes/artwork' and p.suffix == '.png'))


def audit(args):
    source, website = revision(args.source), revision(args.website)
    local = {p.name: p for p in args.assets.iterdir() if p.is_file()}
    require(len(EXPECTED) == 18 and set(local) == EXPECTED,
            'Asset directory must contain exactly the 18 release files; '
            f'missing={sorted(EXPECTED-set(local))}, extra={sorted(set(local)-EXPECTED)}')
    sums = manifest(local[SUMS].read_bytes(), SUMS, 17)
    require(set(sums) == EXPECTED - {SUMS}, 'Public manifest inventory differs from release layout')
    verified = []
    for name, path in sorted(local.items()):
        sha = digest(path)
        if name != SUMS:
            require(sums[name] == sha, f'Public manifest hash differs: {name}')
        verified.append(dict(name=name, size=path.stat().st_size, sha256=sha))
    print('PASS: 18 prepared assets and 17 manifest hashes', flush=True)

    server_verified = False
    release = None
    if args.release:
        release = json.loads(args.release.read_text(encoding='utf-8-sig'))
        require(release['tag_name'] == 'v' + VERSION, 'Server release tag differs')
        server = {a['name']: a for a in release['assets']}
        require(len(release['assets']) == 18 and set(server) == EXPECTED,
                'Server asset inventory differs from prepared release')
        for item in verified:
            remote = server[item['name']]
            require(remote['size'] == item['size'], f'Server size differs: {item["name"]}')
            require(remote['digest'] == 'sha256:' + item['sha256'],
                    f'Server digest differs: {item["name"]}')
        server_verified = True
        print('PASS: optional server metadata matches every prepared asset', flush=True)

    with archive(local[WINDOWS]) as z:
        prefix = f'DuneLegacyTornie-{VERSION}-Windows-x64/'
        require(prefix + 'dunelegacy.exe' in files(z)
                and all(name.startswith(prefix) for name in files(z)), 'Windows ZIP layout differs')

    with archive(local[SOURCES]) as z:
        require(z.comment.decode('ascii') == source, 'Source ZIP commit comment differs')
        tree = git_tree(source)
        prefix = f'DuneLegacyTornie-{VERSION}/'
        require(files(z) == {prefix + path for path in tree}, 'Source ZIP file inventory differs')
        for path, sha in tree.items():
            verify_blob(z, prefix + path, sha)
        source_count = len(tree)
    print(f'PASS: source ZIP contains {source_count} byte-identical Git files', flush=True)

    with archive(local[GUIDES]) as z:
        prefix = f'DuneLegacyTornie-{VERSION}-Guides/'
        proof_name = prefix + 'Documentation-Source.json'
        proof = json.loads(z.read(proof_name))
        require(proof['version'] == VERSION and proof['source'] == source
                and proof['documentationSource'] == website, 'Guide provenance differs')
        tree = {path: sha for path, sha in git_tree(website).items() if guide_path(path)}
        require(files(z) == {prefix + path for path in tree} | {proof_name},
                'Guide ZIP file inventory differs from pinned documentation')
        for path, sha in tree.items():
            verify_blob(z, prefix + path, sha)
        guide_count = len(tree)
    print(f'PASS: guide ZIP contains {guide_count} byte-identical documentation files', flush=True)

    with archive(local[OFFLINE]) as z:
        prefix = 'DuneLegacyTornie-Site-Local/dist/'
        inner = manifest(z.read(prefix + 'documents/' + SUMS), 'Offline manifest', 15)
        require(set(inner) == set(sums) - {OFFLINE, WEB_VALIDATION},
                'Offline manifest inventory differs from embedded release layout')
        for name, sha in inner.items():
            require(sums[name] == sha, f'Public/embedded manifest disagreement: {name}')
            with z.open(prefix + 'documents/' + name) as stream:
                require(stream_digest(stream) == sha, f'Embedded payload differs: {name}')
        tree = git_tree(website, 'website')
        for path, sha in tree.items():
            verify_blob(z, prefix + path.removeprefix('website/'), sha)
        publication = json.loads(z.read(prefix + 'documents/' + PUBLICATION))
        require(publication['version'] == VERSION and publication['source'] == source
                and publication['website'] == website, 'Offline publication provenance differs')
        website_count = len(tree)
    print(f'PASS: offline ZIP contains {website_count} exact website files and 15 embedded hashes', flush=True)

    report = dict(version=VERSION, passed=True, source=source, website=website,
                  assets=verified, manifestEntries=17,
                  sourceZipFilesByteIdentical=source_count,
                  guidesFilesByteIdentical=guide_count,
                  websiteFilesByteIdentical=website_count,
                  embeddedFiles=15, archivesCrcAndUniqueEntries=True,
                  serverAssetsVerified=server_verified)
    if release is not None:
        report.update(release=release.get('html_url'), releaseDraft=release.get('draft'))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, ensure_ascii=False, indent=2) + '\n',
                           encoding='utf-8', newline='\n')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--assets', type=Path, required=True)
    parser.add_argument('--source', required=True)
    parser.add_argument('--website', required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--release', type=Path)
    args = parser.parse_args()
    try:
        audit(args)
    except (OSError, ValueError, KeyError, zipfile.BadZipFile, subprocess.CalledProcessError) as exc:
        parser.exit(1, f'Audit failed: {exc}\n')


if __name__ == '__main__':
    main()
