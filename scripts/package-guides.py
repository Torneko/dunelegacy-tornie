#!/usr/bin/env python3
"""Build FR/EN guides from a pinned documentation revision for the tested game."""
import argparse,json,re,subprocess,zipfile
from pathlib import Path

parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--output',type=Path,required=True)
parser.add_argument('--root',type=Path,default=Path(__file__).resolve().parents[1])
parser.add_argument('--documentation-ref',default='HEAD')
args=parser.parse_args()
root=args.root.resolve()
version=re.search(r'project\(DuneLegacyTornie VERSION ([\d.]+)',(root/'CMakeLists.txt').read_text()).group(1)
source=subprocess.check_output(['git','rev-parse','HEAD'],cwd=root,text=True).strip()
documentation_source=subprocess.check_output(['git','rev-parse',args.documentation_ref+'^{commit}'],cwd=root,text=True).strip()
def blob(path):
    return subprocess.check_output(['git','show',documentation_source+':'+path],cwd=root)
documentation_version=re.search(rb'project\(DuneLegacyTornie VERSION ([\d.]+)',blob('CMakeLists.txt')).group(1).decode()
assert documentation_version==version, 'Documentation revision uses a different game version'
prefix='DuneLegacyTornie-'+version+'-Guides/'
tracked=subprocess.check_output(['git','ls-tree','-r','-z','--name-only',documentation_source],cwd=root).decode('utf-8').split('\0')
def include(path):
    p=Path(path)
    return (path in ['HOWTOPLAY.md','HOWTOPLAY-FR.md','README.md','AUTHORS','COPYING']
            or (p.parent.as_posix() in ['docs','release_notes','website/documents'] and p.suffix=='.md')
            or p.parent.as_posix()=='release_notes/previews'
            or (p.parent.as_posix()=='release_notes/artwork' and p.suffix=='.png'))
files=sorted(path for path in tracked if include(path))
args.output.parent.mkdir(parents=True,exist_ok=True)
with zipfile.ZipFile(args.output,'w',zipfile.ZIP_DEFLATED,compresslevel=6) as archive:
    for path in files:
        archive.writestr(prefix+path,blob(path))
    archive.writestr(prefix+'Documentation-Source.json',json.dumps(dict(version=version,source=source,documentationSource=documentation_source),indent=2)+'\n')
with zipfile.ZipFile(args.output) as archive:assert archive.testzip() is None
print('Guides:',args.output.name,args.output.stat().st_size,'bytes')
