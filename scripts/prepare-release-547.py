#!/usr/bin/env python3
"""Pin the 547 website to its tested game source before the release is published."""
from pathlib import Path
import argparse,json

root=Path(__file__).resolve().parents[1]
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--source',required=True)
parser.add_argument('--published',action='store_true')
args=parser.parse_args()
path=root/'website/assets/site-data.js';text=path.read_text(encoding='utf-8')
data=json.loads(text[text.index('{'):text.rindex('}')+1])
assert data['version']=='1.0.547' and data['rules547']['protocol']==25
status='released' if args.published else 'release-candidate'
data.update(sourceCommit=args.source,publishedSourceCommit=args.source,sourceRevision=args.source,
            releaseRevision=args.source,releaseStatus=status,localPreparation=False,localChanges=False)
data['rules547'].update(sourceCommit=args.source,status=status)
data['automatedValidation']='Contrôles moteur/réseau et vérifications des paquets Windows/Linux installés FR/EN : Validation-1.0.547.json.'
path.write_text('window.SITE_DATA ='+json.dumps(data,ensure_ascii=False,separators=(',',':'))+';\n',encoding='utf-8',newline='\n')
print('547 website pinned to',args.source,status)
