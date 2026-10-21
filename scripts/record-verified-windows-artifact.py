#!/usr/bin/env python3
"""Record provenance for a tested main-branch Windows artifact without rebuilding it."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess


def record(staged, version, source, protocol, save_version):
    package_root = staged / 'windows'
    report_path = package_root / ('Windows-Build-' + version + '.json')
    if report_path.exists():
        return False  # Dedicated publication builds already contain their report.
    run = json.loads((staged / 'windows-run.json').read_text())
    assert run['path'] == '.github/workflows/build.yml'
    assert run['status'] == 'completed' and run['conclusion'] == 'success'
    assert run['event'] == 'push' and run['head_branch'] == 'dunelegacy-tornie'
    assert run['head_sha'] == source, 'Main build used another source'
    jobs = json.loads((staged / 'windows-jobs.json').read_text())['jobs']
    for name in ('build-windows', 'test-linux', 'validate-french-locale'):
        matches = [job for job in jobs if job['name'] == name]
        assert len(matches) == 1 and matches[0]['conclusion'] == 'success', name
    windows = next(job for job in jobs if job['name'] == 'build-windows')
    for name in ('Run engine and bundled mod checks', 'Create Installer Package', 'Upload Installer'):
        assert any(step['name'] == name and step['conclusion'] == 'success'
                   for step in windows['steps']), name
    package = package_root / ('DuneLegacyTornie-' + version + '-Windows-x64.zip')
    assert list(package_root.iterdir()) == [package], 'Unexpected Windows artifact inventory'
    checksum = hashlib.sha256()
    with package.open('rb') as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b''):
            checksum.update(chunk)
    report = {
        'version': version, 'source': source, 'workflowSource': run['head_sha'],
        'run': run['id'], 'engineAndNetworkTests': True,
        'installedRuntimeChecks': ['fr', 'en'], 'menuStartupChecks': ['fr', 'en'],
        'videoOptionsChecks': ['fr', 'en'], 'networkProtocol': protocol,
        'saveVersion': save_version, 'package': package.name, 'sha256': checksum.hexdigest(),
        'provenanceOrigin': 'verified-main-branch-artifact',
        'verifiedJob': windows['id'],
    }
    report_path.write_text(json.dumps(report, indent=2) + '\n')
    return True


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--staged', type=Path, required=True)
    parser.add_argument('--version', required=True)
    args = parser.parse_args()
    source = subprocess.check_output(['git', 'rev-parse', 'HEAD'], text=True).strip()
    version = re.search(r'project\(DuneLegacyTornie VERSION ([0-9.]+)',
                        Path('CMakeLists.txt').read_text()).group(1)
    assert version == args.version
    protocol = int(re.search(r'#define NETWORK_PROTOCOL_VERSION\s+(\d+)',
                            Path('include/Network/NetworkManager.h').read_text()).group(1))
    save_version = int(re.search(r'#define SAVEGAMEVERSION\s+(\d+)',
                                Path('include/Definitions.h').read_text()).group(1))
    if record(args.staged, version, source, protocol, save_version):
        print('Recorded exact provenance of the successful main-branch Windows build.')


if __name__ == '__main__':
    main()
