#!/usr/bin/env python3
"""Build a small FR/EN guide archive from the checked-out release source."""
import argparse,json,re,subprocess,zipfile
from pathlib import Path

root=Path(__file__).resolve().parents[1]
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--output',type=Path,required=True)
args=parser.parse_args()
version=re.search(r'project\(DuneLegacyTornie VERSION ([\d.]+)',(root/'CMakeLists.txt').read_text()).group(1)
source=subprocess.check_output(['git','rev-parse','HEAD'],cwd=root,text=True).strip()
prefix='DuneLegacyTornie-'+version+'-Guides/'
files=[root/name for name in ['HOWTOPLAY.md','HOWTOPLAY-FR.md','README.md','AUTHORS','COPYING']]
files+=list((root/'docs').glob('*.md'))
files+=list((root/'release_notes').glob('*.md'))
files+=list((root/'release_notes/previews').glob('*'))
files+=list((root/'release_notes/artwork').glob('*.png'))
files+=list((root/'website/documents').glob('*.md'))
args.output.parent.mkdir(parents=True,exist_ok=True)
with zipfile.ZipFile(args.output,'w',zipfile.ZIP_DEFLATED,compresslevel=6) as archive:
    for path in sorted(set(files)):
        if path.is_file():archive.write(path,prefix+path.relative_to(root).as_posix())
    archive.writestr(prefix+'Documentation-Source.json',json.dumps(dict(version=version,source=source),indent=2)+'\n')
with zipfile.ZipFile(args.output) as archive:assert archive.testzip() is None
print('Guides:',args.output.name,args.output.stat().st_size,'bytes')
