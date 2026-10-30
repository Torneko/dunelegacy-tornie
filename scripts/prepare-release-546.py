#!/usr/bin/env python3
"""Promote the existing bilingual 546 guide; deployment waits for publication."""
from pathlib import Path
import argparse
import html
import json
import re
import shutil

ROOT=Path(__file__).resolve().parents[1]
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--published',action='store_true')
parser.add_argument('--source',default='v1.0.546')
args=parser.parse_args()
site=ROOT/'website'
base='https://github.com/Torneko/dunelegacy-tornie'
path=site/'assets/site-data.js'
text=path.read_text(encoding='utf-8')
data=json.loads(text[text.index('{'):text.rindex('}')+1])
status='released' if args.published else 'release-candidate'
data.update(version='1.0.546',releasedVersion='1.0.546',releaseStatus=status,
            localPreparation=False,localChanges=False,sourceCommit=args.source,
            sourceRevision=args.source,releaseRevision=args.source,publishedSourceCommit=args.source,
            releaseUrl=base+'/releases/tag/v1.0.546')
for key,name in [('windowsUrl','Windows-x64.zip'),('mintUrl','Linux-Mint-Compatible.AppImage'),('localSourceUrl','Sources.zip')]:
    data[key]=base+'/releases/download/v1.0.546/DuneLegacyTornie-1.0.546-'+name
    data['offlineDownloads'][key]='documents/DuneLegacyTornie-1.0.546-'+name
data['rules546']=dict(status=status,protocol=24,saveVersion=9839,achievements=73,
    campaignEditions=['Tornie','TornieLite'],campaigns=18,soloScenarios=396,
    persistentSupport=True,stage2BaseClearance=10,previousMissionOpponents=3,finalOpponents=4,
    editorChoamItems=31,vanillaCampaignEditor=True,wildspadeHightech=4,wildspadeCarryall=4,
    wildspadeChemicalCarryall=5,sourceCommit=args.source)
data.pop('previewRevision',None)
path.write_text('window.SITE_DATA = '+json.dumps(data,ensure_ascii=False,separators=(',',':'))+';\n',encoding='utf-8')

for lang,index,app,note,summary in [
 ('FR','index.html','app.js','Notes-1.0.546.md','Campagnes Tornie/Tornie Lite révisées, coops adaptées, soutien permanent, finales à quatre adversaires. CHOAM de l’éditeur, campagnes Vanilla et cadres Atreides/Ordos corrigés. Protocole 24 ; sauvegardes 9839.'),
 ('EN','index-en.html','app-en.js','Notes-1.0.546-EN.md','Revised Tornie/Tornie Lite campaigns and matching co-op, persistent support and four-opponent finales. Editor CHOAM, Vanilla campaign access and Atreides/Ordos frames fixed. Protocol 24; save format 9839.')]:
    source=ROOT/f'release_notes/1.0.546-{lang}.md'
    shutil.copy2(source,site/'documents'/note)
    heading='1.0.546 — campagnes et éditeur' if lang=='FR' else '1.0.546 — campaigns and editor'
    label='Notes de version' if lang=='FR' else 'Release notes'
    notice=f'<section class="notice release546" style="margin:1rem 2rem;padding:1rem"><h2>{heading}</h2><p>{summary}</p><a href="documents/{note}">{label}</a></section>'
    p=site/index;s=p.read_text(encoding='utf-8')
    s=re.sub(r'<section class="notice (?:preparation546|release546)".*?</section>\s*','',s,flags=re.S)
    s=s.replace('<div class="layout">',notice+'\n  <div class="layout">',1)
    s=s.replace('1.0.546 r2 · preview','1.0.546').replace('?rev=546-r2','?rev=546-final')
    s=s.replace('changes-545-en.html','changes-546-en.html').replace('changes-545.html','changes-546.html')
    s=s.replace('Notes 1.0.545','Notes 1.0.546').replace('1.0.545 notes','1.0.546 notes')
    s=s.replace('Préparation locale 1.0.546','Édition 1.0.546').replace('Preview 1.0.546','Edition 1.0.546')
    p.write_text(s,encoding='utf-8',newline='\n')
    p=site/'assets'/app;s=p.read_text(encoding='utf-8')
    s=s.replace('1.0.546 r2','1.0.546').replace('révision 546 r2','1.0.546').replace('revision 546 r2','1.0.546')
    release_notice=f'const nextPreparationNotice=()=>`<section class="notice release546"><h2>{heading}</h2><p>{summary}</p><p><a href="documents/{note}">{label}</a> · <a href="${{downloadUrl(\'windowsUrl\')}}">Windows 1.0.546</a> · <a href="${{downloadUrl(\'mintUrl\')}}">Linux Mint 1.0.546</a></p></section>`;'
    s=re.sub(r'^const nextPreparationNotice=.*$',lambda m:release_notice,s,flags=re.M)
    s=re.sub(r'<section class="notice network539">.*?</section>',lambda m:m[0].replace('1.0.545','1.0.546').replace('protocole 22','protocole 24').replace('protocol 22','protocol 24'),s)
    for old,new in [('Windows · 1.0.545','Windows · 1.0.546'),('Windows x64 · 1.0.545','Windows x64 · 1.0.546'),('Linux AppImage · 1.0.545','Linux AppImage · 1.0.546'),('Linux AppImage 1.0.545','Linux AppImage 1.0.546'),('Windows / Linux release 1.0.545','Windows / Linux release 1.0.546'),('Version Windows / Linux 1.0.545','Version Windows / Linux 1.0.546'),('version actuelle 1.0.545 utilise le protocole 22','version actuelle 1.0.546 utilise le protocole 24'),('current 1.0.545 uses protocol 22','current 1.0.546 uses protocol 24'),('DuneLegacyTornie-1.0.545-Windows-x64.zip','DuneLegacyTornie-1.0.546-Windows-x64.zip'),('Release 1.0.545 · Linux','Release 1.0.546 · Linux'),('Version 1.0.545 · Linux','Version 1.0.546 · Linux')]:s=s.replace(old,new)
    s=s.replace('Windows ou Linux 1.0.545, le protocole 22','Windows ou Linux 1.0.546, le protocole 24').replace('Windows or Linux 1.0.545, protocol 22','Windows or Linux 1.0.546, protocol 24')
    # Historical feature sections also contain current installation advice.
    # Keep their historical headings while pointing that advice at this release.
    for old,new in [('Retélécharger sur tous les postes : protocole 22','Avec la 1.0.546 : protocole 24'),
                    ('Download this replacement on every computer: protocol 22','For 1.0.546, use protocol 24'),
                    ('Download again on every computer: protocol 22','For 1.0.546, use protocol 24'),
                    ('Retélécharger les paquets 1.0.545','Utiliser les paquets 1.0.546'),
                    ('Download the 1.0.545 packages again','Use the 1.0.546 packages')]:
        s=s.replace(old,new)
    p.write_text(s,encoding='utf-8',newline='\n')
    other='changes-546-en.html' if lang=='FR' else 'changes-546.html'
    page=f'<!doctype html><html lang="{lang.lower()}"><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>Dune Legacy Tornie 1.0.546</title><link rel="stylesheet" href="assets/style.css"><body><main class="prose" style="max-width:900px;margin:2rem auto;padding:1.5rem"><nav><a href="{index}">Guide</a> · <a href="{other}">{"English" if lang=="FR" else "Français"}</a></nav><h1>Dune Legacy Tornie 1.0.546</h1><p>{summary}</p><p><a href="documents/{note}">{label}</a></p><pre style="white-space:pre-wrap;font:inherit">{html.escape(source.read_text(encoding="utf-8"))}</pre></main></body></html>\n'
    (site/('changes-546.html' if lang=='FR' else 'changes-546-en.html')).write_text(page,encoding='utf-8')

readme=ROOT/'README.md';s=readme.read_text(encoding='utf-8')
s=re.sub(r'^\*\*(?:\[)?1\.0\.546.*$', '**[1.0.546 — Windows / Linux / Mint](https://github.com/Torneko/dunelegacy-tornie/releases/tag/v1.0.546)** : campagnes Tornie/Tornie Lite et coops révisées, soutien permanent, trois adversaires en 020–021 et quatre en 022 ; CHOAM, cadres et éditeur Vanilla corrigés. Protocole **24**, sauvegardes **9839**. [FR](release_notes/1.0.546-FR.md) · [EN](release_notes/1.0.546-EN.md).',s,flags=re.M)
readme.write_text(s,encoding='utf-8')
(ROOT/'release_notes/RELEASE_NOTES.md').write_text((ROOT/'release_notes/1.0.546-FR.md').read_text(encoding='utf-8')+'\n---\n\n'+(ROOT/'release_notes/1.0.546-EN.md').read_text(encoding='utf-8'),encoding='utf-8')
validation=json.loads((ROOT/'release_notes/Validation-1.0.546-local.json').read_text(encoding='utf-8'))
validation.update(status=status,date='2026-10-30',localWindowsEvidenceDate='2026-10-25',source=args.source)
validation['website'].update(releaseVersion='1.0.546',publishedDownloadVersionRetained=None)
for dest in [ROOT/'release_notes/Validation-1.0.546.json',site/'documents/Validation-1.0.546.json']:
    dest.write_text(json.dumps(validation,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
print('Prepared existing FR/EN 1.0.546 website and notes:',status)
