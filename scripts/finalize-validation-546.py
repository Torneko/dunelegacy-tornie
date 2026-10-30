#!/usr/bin/env python3
"""Record successful CI packages separately from the earlier local preview."""
from pathlib import Path
import argparse
import json

ROOT=Path(__file__).resolve().parents[1]
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--staged',type=Path,required=True)
parser.add_argument('--source',required=True)
parser.add_argument('--windows-run',type=int,required=True)
parser.add_argument('--linux-run',type=int,required=True)
args=parser.parse_args()
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
local=read(ROOT/'release_notes/Validation-1.0.546-local.json')
builds={}
for platform,run in [('windows',args.windows_run),('linux',args.linux_run)]:
    build=read(args.staged/platform/(platform.title()+'-Build-1.0.546.json'))
    assert build['version']=='1.0.546' and build['source']==args.source
    assert build['networkProtocol']==24 and build['saveVersion']==9839
    assert build['engineAndNetworkTests'] and build['run']==run
    assert build['validationScope']=='release-546'
    builds[platform]=build
audit=read(args.staged/'Package-Resources-Audit-1.0.546.json')
assert audit['source']==args.source and len(audit['packages'])==4
assert all(package['passed'] for package in audit['packages'].values())
report=dict(version='1.0.546',status='release-verified',date='2026-10-30',source=args.source,
    networkProtocol=24,saveFormat=9839,platforms=builds,packageResources=audit,
    campaignRecipes=local['campaignRecipes'],runtimeChecksPerLanguage=dict(editorMapLoads=5265,
      soloStarts=252,cooperativeStarts=224,lateCooperativeRestores=76,frameCompositions=28,
      languages=['fr','en'],platforms=['Windows x64','Linux x86_64 Mint AppImage']),
    localPreviewEvidence=local,limits=local['limits'],website=dict(languages=['fr','en'],
      releaseVersion='1.0.546',localRoutes=36,localResearchContexts=84,localCampaignContexts=28),
    ciUrls=dict(windows=f'https://github.com/Torneko/dunelegacy-tornie/actions/runs/{args.windows_run}',
                linux=f'https://github.com/Torneko/dunelegacy-tornie/actions/runs/{args.linux_run}'))
for path in [ROOT/'release_notes/Validation-1.0.546.json',ROOT/'website/documents/Validation-1.0.546.json']:
    path.write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
print('Recorded both CI platforms and exact audited package hashes for',args.source)
