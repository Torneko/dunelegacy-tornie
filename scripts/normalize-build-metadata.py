#!/usr/bin/env python3
"""Correct the known 547 report-only save-version typo without changing packages."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path

p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--staged',type=Path,required=True)
p.add_argument('--version',required=True)
a=p.parse_args()
source=subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip()
save=int(re.search(rb'#define SAVEGAMEVERSION\s+(\d+)',Path('include/Definitions.h').read_bytes()).group(1))
protocol=int(re.search(rb'#define NETWORK_PROTOCOL_VERSION\s+(\d+)',Path('include/Network/NetworkManager.h').read_bytes()).group(1))
for platform in ['windows','linux']:
    path=a.staged/platform/(platform.title()+'-Build-'+a.version+'.json')
    raw=path.read_bytes();report=json.loads(raw.decode('utf-8-sig'))
    assert report['source']==source and report['version']==a.version
    assert report['engineAndNetworkTests'] and report['networkProtocol']==protocol
    if report['saveVersion']==save:continue
    assert a.version=='1.0.547' and report['saveVersion']==9839 and save==9840 and protocol==25
    report['metadataCorrection']={
        'field':'saveVersion','originalValue':9839,'correctValue':9840,
        'originalReportSha256':hashlib.sha256(raw).hexdigest(),
        'verifiedSource':source,'sourceHeader':'include/Definitions.h',
        'reason':'The workflow report retained the previous save version; the tested source defines 9840. Package bytes are unchanged.'}
    report['saveVersion']=save
    path.write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8',newline='\n')
    print(platform+': corrected report saveVersion 9839 -> 9840; packages unchanged')
