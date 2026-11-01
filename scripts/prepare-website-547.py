#!/usr/bin/env python3
"""Maintain the existing bilingual site for the 547 release without rebuilding its artwork."""
from pathlib import Path
import copy, hashlib, html, json, re, shutil

ROOT=Path(__file__).resolve().parents[1]
SITE=ROOT/'website'
def read_js(path):
    text=path.read_text(encoding='utf-8')
    return json.loads(text[text.index('{'):text.rindex('}')+1])
def write_js(path,name,data):
    path.write_text('window.'+name+' ='+json.dumps(data,ensure_ascii=False,separators=(',',':'))+';\n',encoding='utf-8',newline='\n')

data=read_js(SITE/'assets/site-data.js')
for key in ['version','releasedVersion']:data[key]='1.0.547'
for key in ['releaseUrl','windowsUrl','mintUrl','localSourceUrl','guidesUrl']:
    data[key]=data[key].replace('1.0.546','1.0.547')
for key in data['offlineDownloads']:data['offlineDownloads'][key]=data['offlineDownloads'][key].replace('1.0.546','1.0.547')
data['saveVersion']=9840
data['releaseRevision']='547';data['documentationRevision']='547-20261031'
data['localChanges']=False;data['localPreparation']=False
data['sourceRevision']='v1.0.547';data['sourceCommit']='v1.0.547';data['publishedSourceCommit']='v1.0.547'
data['automatedValidation']='Contrôles moteur/réseau et essais installés FR/EN : voir Validation-1.0.547.json.'
metadata=json.loads((ROOT/'config/Release547.json').read_text())
data['rules547']={**metadata,'protocol':25,'status':'prepared','sourceCommit':'v1.0.547'}
elite=next(unit for unit in data['units'] if unit['id']=='elite-siege-tank')
existing=next((unit for unit in data['units'] if unit['id']=='super-tank'),None)
replacement_routes=[route for route in elite['routes'] if route['house']=='K'] or (existing['routes'] if existing else [])
super_tank=copy.deepcopy(elite)
super_tank.update(id='super-tank',name='Super Tank',label='Super Tank',category='Véhicule lourd',
    description='Char de siège incendiaire de Kleshmersh : 925 crédits, 300 PV comme le Siege Tank, salve de deux projectiles de feu et explosion dommageable à la destruction comme le Devastator.',
    image='assets/png/01_Icones_pour_le_site/SuperTankIcon.png',sprites=['assets/png/02_Sprites_des_unites/SuperTankGun.png'])
super_tank['routes']=[{**route,'unit':'Super Tank','price':925,'priceBasis':'unit'} for route in replacement_routes]
elite['routes']=[route for route in elite['routes'] if route['house']!='K']
data['units']=[unit for unit in data['units'] if unit['id']!='super-tank']+[super_tank]
wind=next(item for item in data['buildings'] if item['id']=='windtrap')
wind['description']='Fournit l’énergie de la base. Hors Vanilla, toutes les factions peuvent recevoir « Améliorée au départ » dans l’éditeur, même sans Chaos : production doublée, animation verte et étoile verte dans l’éditeur. L’amélioration achetable en partie reste liée aux plans Rebels.'
for name,folder in [('SuperTankIcon.png','01_Icones_pour_le_site'),('SuperTankGun.png','02_Sprites_des_unites')]:
    target=SITE/'assets/png'/folder/name;target.parent.mkdir(parents=True,exist_ok=True)
    source=ROOT/'data'/name;shutil.copyfile(source,target)
    relative=f'{folder}/{name}'
    data['assets']=[asset for asset in data['assets'] if asset['file']!=relative]
    data['assets'].append(dict(file=relative,url='assets/png/'+relative,width=91 if 'Icon' in name else 128,height=55 if 'Icon' in name else 16,transparent='Gun' in name,
                             sha256=hashlib.sha256(source.read_bytes()).hexdigest(),sources=['data/'+name],distributions=['Commun']))
data['pngCount']=sum(asset.get('file','').lower().endswith('.png') for asset in data['assets'])
write_js(SITE/'assets/site-data.js','SITE_DATA',data)
english=read_js(SITE/'assets/site-en-data.js')
english['units']['Super Tank']=dict(label='Super Tank',category='Heavy vehicle',description='Kleshmersh incendiary siege tank: 925 credits, 300 HP matching the Siege Tank, a two-projectile fire salvo and a damaging Devastator-style explosion on destruction.')
english['buildings']['Windtrap']['description']='Powers the base. Outside Vanilla, every faction can use the editor’s Pre-upgraded option without Chaos: double initial power, green animation and a green editor star. The purchasable upgrade still follows Rebels blueprints.'
write_js(SITE/'assets/site-en-data.js','SITE_EN',english)

summaries={
 'FR':'Super Tank de Kleshmersh à 925 crédits, sélection d’armée et de toutes les moissonneuses, Windtraps améliorés dans l’éditeur pour toutes les factions hors Vanilla, même sans Chaos, avec étoile verte. Protocole 25 ; sauvegardes 9840.',
 'EN':'Kleshmersh Super Tank at 925 credits, army and all-Harvester selection, pre-upgraded editor Windtraps for every non-Vanilla faction without requiring Chaos, with a green star. Protocol 25; save format 9840.'}
for language,app,index in [('FR','app.js','index.html'),('EN','app-en.js','index-en.html')]:
    fr=language=='FR';path=SITE/'assets'/app;text=path.read_text(encoding='utf-8')
    text=text.replace('1.0.546','1.0.547').replace('protocole 24','protocole 25').replace('Protocole 24','Protocole 25').replace('protocol 24','protocol 25').replace('Protocol 24','Protocol 25').replace('9839','9840')
    # The campaign overhaul remains attributed to its original release.
    text=text.replace('Campagnes Tornie — 1.0.547','Campagnes Tornie — 1.0.546').replace('Tornie campaigns — 1.0.547','Tornie campaigns — 1.0.546')
    text=re.sub(r'(<section class="campaign-plan"><h2>[^<]*)1\.0\.547',lambda m:m[1]+'1.0.546',text)
    label='Notes de version' if fr else 'Release notes'
    note='Notes-1.0.547.md' if fr else 'Notes-1.0.547-EN.md'
    title='1.0.547 — Super Tank et sélections' if fr else '1.0.547 — Super Tank and selection'
    notice='const nextPreparationNotice=()=>`<section class="notice release547"><h2>'+title+'</h2><p>'+summaries[language]+'</p><p><a href="documents/'+note+'">'+label+'</a> · <a href="${downloadUrl(\'windowsUrl\')}">Windows 1.0.547</a> · <a href="${downloadUrl(\'mintUrl\')}">Linux Mint 1.0.547</a></p></section>`;'
    text=re.sub(r'^const nextPreparationNotice=.*$',lambda _:notice,text,flags=re.M)
    path.write_text(text,encoding='utf-8',newline='\n')
    path=SITE/index;text=path.read_text(encoding='utf-8').replace('1.0.546','1.0.547').replace('546-docs-20261030','547-20261031').replace('546-links-20261031','547-20261031').replace('changes-546','changes-547')
    title='1.0.547 — Super Tank et sélections' if fr else '1.0.547 — Super Tank and selection'
    text=re.sub(r'<section class="notice release546".*?</section>',lambda _:'<section class="notice release547" style="margin:1rem 2rem;padding:1rem"><h2>'+title+'</h2><p>'+summaries[language]+'</p><a href="documents/'+note+'">'+label+'</a></section>',text)
    path.write_text(text,encoding='utf-8',newline='\n')
    source=ROOT/'release_notes'/('1.0.547-'+language+'.md')
    (SITE/'documents'/note).write_bytes(source.read_bytes())
    other='changes-547-en.html' if fr else 'changes-547.html'
    body='<!doctype html><html lang="'+language.lower()+'"><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>Dune Legacy Tornie 1.0.547</title><link rel="stylesheet" href="assets/style.css"><body><main class="prose" style="max-width:900px;margin:2rem auto;padding:1.5rem"><nav><a href="'+index+'">Guide</a> · <a href="'+other+'">'+('English' if fr else 'Français')+'</a></nav><h1>Dune Legacy Tornie 1.0.547</h1><p>'+summaries[language]+'</p><p><a href="documents/'+note+'">'+label+'</a></p><pre style="white-space:pre-wrap;font:inherit">'+html.escape(source.read_text(encoding='utf-8'))+'</pre></main></body></html>\n'
    (SITE/('changes-547.html' if fr else 'changes-547-en.html')).write_text(body,encoding='utf-8',newline='\n')

for name in ['HOWTOPLAY-FR.md','HOWTOPLAY.md','docs/COOP-CAMPAIGN.md','docs/COOP-CAMPAIGN-EN.md']:
    path=ROOT/name;text=path.read_text(encoding='utf-8').replace('1.0.546','1.0.547').replace('protocole 24','protocole 25').replace('protocol 24','protocol 25').replace('9839','9840')
    path.write_text(text,encoding='utf-8',newline='\n')
for language,name in [('FR','HOWTOPLAY-FR.md'),('EN','HOWTOPLAY.md')]:
    path=ROOT/name;text=path.read_text(encoding='utf-8')
    addition=('## Ajouts 1.0.547\n\nSous Chemical Carryall : **Armée**, puis **Moissonneuses**, avec compteurs et contrôle allié. Kleshmersh reçoit le **Super Tank à 925 crédits**, 300 PV, deux projectiles incendiaires et explosion à la destruction, en remplacement de l’Elite Siege Tank. Usine lourde + IX, normalement au niveau 7. Dans l’éditeur hors Vanilla, tout Windtrap standard peut être amélioré au départ, sans Chaos, avec étoile verte. Les anciennes sauvegardes gardent leurs tables ; démarrer une nouvelle partie ou mission pour les remplacements.\n\n' if language=='FR' else '## Additions in 1.0.547\n\nBelow Chemical Carryall: **Army**, then **Harvesters**, with counts and allied-control permissions. Kleshmersh receives the **Super Tank for 925 credits**, with 300 HP, two fire projectiles and an explosion on destruction, replacing the Elite Siege Tank. Heavy Factory + IX, normally technology 7. In the non-Vanilla editor, every standard Windtrap can start pre-upgraded without Chaos, with a green star. Old saves retain their tables; start a new game or mission for the replacements.\n\n')
    if ('## Ajouts 1.0.547' if language=='FR' else '## Additions in 1.0.547') not in text:
        text=text[:text.index('\n')+1]+'\n'+addition+text[text.index('\n')+1:]
    text=text.replace('SUIVI-1.0.546','SUIVI-1.0.547')
    path.write_text(text,encoding='utf-8',newline='\n')
    web_text=text.replace('(docs/','(')
    (SITE/'documents'/('Guide-1.0.547-'+language+'.md')).write_text(web_text,encoding='utf-8',newline='\n')
for name in ['COOP-CAMPAIGN.md','COOP-CAMPAIGN-EN.md','CAMPAIGNS.md','ACHIEVEMENTS.md']:
    shutil.copyfile(ROOT/'docs'/name,SITE/'documents'/name)
for language in ['FR','EN']:
    source=ROOT/'docs'/('SUIVI-1.0.546-'+language+'.md')
    text=source.read_text(encoding='utf-8').replace('1.0.546','1.0.547').replace('30 octobre','31 octobre').replace('30 October','31 October').replace('**24**','**25**').replace('**9839**','**9840**')
    heading='## Ajouts de la 1.0.547\n\n' if language=='FR' else '## Additions in 1.0.547\n\n'
    paragraph=summaries[language]+'\n\n'+('[Notes détaillées](../release_notes/1.0.547-FR.md)' if language=='FR' else '[Detailed notes](../release_notes/1.0.547-EN.md)')+'\n\n'
    at=text.index('\n\n')+2;text=text[:at]+heading+paragraph+text[at:]
    target=ROOT/'docs'/('SUIVI-1.0.547-'+language+'.md');target.write_text(text,encoding='utf-8',newline='\n')
    (SITE/'documents'/target.name).write_text(text.replace('../release_notes/1.0.547-FR.md','Notes-1.0.547.md').replace('../release_notes/1.0.547-EN.md','Notes-1.0.547-EN.md'),encoding='utf-8',newline='\n')
    path=ROOT/'release_notes'/('Upcoming-Release-'+language+'.md');text=path.read_text(encoding='utf-8').replace('- [ ]','- [x]')
    text=text.replace('Prochaine mise à jour — demandes à réaliser','Préparation réalisée — 1.0.547').replace('Next update — requested work','Completed preparation — 1.0.547')
    text=text.replace('Le numéro de la prochaine version reste à fixer. Les éléments ci-dessous sont demandés et ne sont pas encore implémentés.','Contenu intégré dans la préparation 1.0.547, dont les essais locaux FR/EN passent. La publication attend la validation des paquets Windows/Linux.').replace('The next version number is not yet assigned. The items below are requested and have not been implemented.','Implemented in the 1.0.547 preparation, with local FR/EN checks passing. Publication awaits the Windows/Linux package checks.')
    text=text.replace('non encore intégré au jeu','intégré au jeu').replace('not yet integrated into the game','integrated into the game')
    path.write_text(text,encoding='utf-8',newline='\n')

shutil.copyfile(ROOT/'release_notes/1.0.547-FR.md',ROOT/'release_notes/RELEASE_NOTES.md')
manifest=dict(version='1.0.547',author='Tornie_Panther',activeResources=[dict(runtime='data/'+name,sha256=hashlib.sha256((ROOT/'data'/name).read_bytes()).hexdigest()) for name in ['SuperTankGun.png','SuperTankIcon.png']])
(ROOT/'docs/RELEASE-1.0.547-assets.json').write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8',newline='\n')
print('547 site, routes, original PNGs, guides and release notes prepared')
