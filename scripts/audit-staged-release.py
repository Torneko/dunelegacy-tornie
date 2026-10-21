#!/usr/bin/env python3
"""Check staged Windows/Linux payloads against the checked-out release source."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import tarfile
import tempfile
import zipfile


def sha(data):
    return hashlib.sha256(data).hexdigest()


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--staged',type=Path,required=True)
    parser.add_argument('--version',required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    source=subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip()
    paths=subprocess.check_output(['git','ls-tree','-r','--name-only','HEAD','mods'],text=True).splitlines()
    expected={path:sha(Path(path).read_bytes()) for path in paths}
    artfile=Path('docs')/('RELEASE-'+args.version+'-assets.json')
    artwork=json.loads(artfile.read_text())['activeResources'] if artfile.exists() else []
    for entry in artwork:
        assert sha(Path(entry['runtime']).read_bytes())==entry['sha256'],entry['runtime']
    report={'version':args.version,'source':source,'modFiles':len(expected),'originalArtworkFiles':len(artwork),'packages':{}}
    windows=args.staged/'windows'/('DuneLegacyTornie-'+args.version+'-Windows-x64.zip')
    with zipfile.ZipFile(windows) as archive:
        assert archive.testzip() is None,'Windows ZIP CRC failed'
        names=archive.namelist()
        for path,digest in expected.items():
            matches=[name for name in names if name==path or name.endswith('/'+path)]
            assert len(matches)==1 and sha(archive.read(matches[0]))==digest,path
        for entry in artwork:
            path=entry['runtime'];matches=[name for name in names if name==path or name.endswith('/'+path)]
            assert len(matches)==1 and sha(archive.read(matches[0]))==entry['sha256'],path
    report['packages']['windows']={'passed':True,'sha256':sha(windows.read_bytes())}
    linux=args.staged/'linux'
    tar=next(linux.glob('*.tar.gz'))
    # A compressed TAR can require a full rewind when resources are requested
    # out of archive order. Hash each required member in one forward pass while
    # retaining the missing/duplicate checks used by the other package formats.
    mod_counts={path:0 for path in expected}
    artwork_counts=[0 for entry in artwork]
    artwork_by_name={}
    for index,entry in enumerate(artwork):
        artwork_by_name.setdefault(Path(entry['runtime']).name,[]).append((index,entry))
    with tarfile.open(tar,mode='r|gz') as archive:
        for member in archive:
            if not member.isfile():continue
            parts=member.name.split('/')
            mod_paths=['/'.join(parts[index:]) for index,part in enumerate(parts) if part=='mods']
            mod_paths=[path for path in mod_paths if path in expected]
            art_entries=artwork_by_name.get(parts[-1],[]) if len(parts)>1 else []
            if not mod_paths and not art_entries:continue
            digest=sha(archive.extractfile(member).read())
            for path in mod_paths:
                mod_counts[path]+=1
                assert digest==expected[path],path
            for index,entry in art_entries:
                artwork_counts[index]+=1
                assert digest==entry['sha256'],entry['runtime']
    for path,count in mod_counts.items():assert count==1,path
    for index,entry in enumerate(artwork):assert artwork_counts[index]==1,entry['runtime']
    report['packages']['linuxTar']={'passed':True,'sha256':sha(tar.read_bytes())}
    for image in sorted(linux.glob('*.AppImage')):
        image.chmod(image.stat().st_mode|0o100)
        with tempfile.TemporaryDirectory(prefix='dunelegacy-payload-') as tmp:
            subprocess.run([str(image.resolve()),'--appimage-extract'],cwd=tmp,stdout=subprocess.DEVNULL,check=True)
            extracted=Path(tmp)/'squashfs-root'
            candidates=list(extracted.rglob('mods'))
            roots=[root.parent for root in candidates if (root/'Tornie/mod.ini').is_file()]
            assert len(roots)==1,'Ambiguous AppImage payload root: '+image.name
            root=roots[0]
            for path,digest in expected.items():assert sha((root/path).read_bytes())==digest,image.name+':'+path
            for entry in artwork:
                matches=list(extracted.rglob(Path(entry['runtime']).name))
                assert len(matches)==1 and sha(matches[0].read_bytes())==entry['sha256'],image.name+':'+entry['runtime']
        report['packages'][image.name]={'passed':True,'sha256':sha(image.read_bytes())}
    assert len(list(linux.glob('*.AppImage')))==2,'Both standard and Mint AppImages required'
    args.output.write_text(json.dumps(report,indent=2)+'\n')
    print('Payload audit passed:',len(expected),'mod files and',len(artwork),'original images in Windows, Linux TAR and both AppImages')


if __name__=='__main__':main()
