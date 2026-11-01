#!/usr/bin/env python3
"""Combine the verified 547 platform packages and resource audit."""
from pathlib import Path
import argparse,json

root=Path(__file__).resolve().parents[1]
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--staged',type=Path,required=True)
parser.add_argument('--source',required=True)
parser.add_argument('--windows-run',type=int,required=True)
parser.add_argument('--linux-run',type=int,required=True)
args=parser.parse_args()
read=lambda path:json.loads(path.read_text(encoding='utf-8-sig'))
local=read(root/'release_notes/Validation-1.0.547-local.json')
platforms={}
for platform,run in [('windows',args.windows_run),('linux',args.linux_run)]:
    report=read(args.staged/platform/(platform.title()+'-Build-1.0.547.json'))
    assert report['source']==args.source and report['version']=='1.0.547'
    assert report['networkProtocol']==25 and report['saveVersion']==9840
    assert report['engineAndNetworkTests'] and report['run']==run and report['validationScope']=='release-547'
    platforms[platform]=report
audit=read(args.staged/'Package-Resources-Audit-1.0.547.json')
assert audit['source']==args.source and audit['originalArtworkFiles']==22 and audit['vanillaFiles']==205
assert len(audit['packages'])==4 and all(value['passed'] for value in audit['packages'].values())
report=dict(version='1.0.547',status='release-verified',date='2026-10-31',source=args.source,
            networkProtocol=25,saveFormat=9840,platforms=platforms,packageResources=audit,
            localValidation=local,website=dict(languages=['fr','en'],browserContexts=164),limits=local['limits'])
for path in [root/'release_notes/Validation-1.0.547.json',root/'website/documents/Validation-1.0.547.json']:
    path.write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8',newline='\n')
print('547 Windows, Linux/Mint and packaged resources verified against',args.source)
