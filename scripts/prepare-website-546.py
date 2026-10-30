#!/usr/bin/env python3
"""Update the existing bilingual website for the unpublished 546 preview."""
from pathlib import Path
import json
import re
ROOT=Path(__file__).resolve().parents[1]
p=ROOT/'website/assets/site-data.js';text=p.read_text(encoding='utf-8')
data=json.loads(text[text.index('{'):text.rindex('}')+1])
data.update(version='1.0.546',previewRevision=2,localChanges=True,localPreparation=True,releaseStatus='local-preparation')
policy=json.loads((ROOT/'config/TornieCampaign546.json').read_text())
campaigns=json.loads((ROOT/'release_notes/Campaigns-1.0.546.json').read_text(encoding='utf-8'))
data['campaignRevision']=dict(revision=2,modifiedEditions=policy['modifiedEditions'],support=policy['support'])
for mod in policy['modifiedEditions']:
    for row in data['campaignPlans'][mod]:
        row['persistentSupport']=policy['support'][mod][row['house']]
        row['stageOpponents']={str(m['stage']):m['opponents'] for m in campaigns['maps'] if m['mod']==mod and m['house']==row['house']}
data['sourceBaseCommit']='89b1403f603a709a6883431adcfd91e317c859b7'
data['sourceCommit']=data['sourceBaseCommit']
for item in data['units']+data['buildings']:
    for route in item['routes']:
        if route['house']!='W' or not isinstance(route.get('level'),int):continue
        if item['name'] in ('Hightech Factory','Carryall'):
            route['level']=4;route['configuredTech']=4
        elif item['name']=='Chemical Carryall':
            route['level']=5;route['configuredTech']=5
for item in data['units']:
    if 'description' in item:item['description']=item['description'].replace('Wildspade : niveau 6','Wildspade : niveau 5')
p.write_text('window.SITE_DATA = '+json.dumps(data,ensure_ascii=False,separators=(',',':'))+';\n',encoding='utf-8')
english=ROOT/'website/assets/site-en-data.js'
english.write_text(english.read_text(encoding='utf-8').replace('Wildspade: level 6','Wildspade: level 5'),encoding='utf-8',newline='\n')
for language,index,app,note,heading,body in (
 ('fr','index.html','app.js','Preparation-1.0.546-FR.md','1.0.546 r2 — préparation locale',
  'Campagnes révisées uniquement dans Tornie et Tornie Lite ; celles de Jericho et ArcaKev sont rétablies. Soutien fixe, bases éloignées aux missions 002–004, trois adversaires en 020–021 puis quatre en 022. Protocole 24. Les téléchargements publics restent ceux de la 1.0.545.'),
 ('en','index-en.html','app-en.js','Preparation-1.0.546-EN.md','1.0.546 r2 — local preparation',
  'Campaign changes apply only to Tornie and Tornie Lite; Jericho and ArcaKev campaigns are restored. Persistent support, distant bases in 002–004, three opponents in 020–021 and four in 022. Protocol 24. Published downloads still point to 1.0.545.')):
    p=ROOT/'website'/index;s=p.read_text(encoding='utf-8')
    s=s.replace('Guide de Dune Legacy Tornie 1.0.545','Guide de Dune Legacy Tornie 1.0.546')
    s=s.replace('Dune Legacy Tornie 1.0.545 guide','Dune Legacy Tornie 1.0.546 guide')
    s=s.replace('VERSION <span>1.0.545</span>','VERSION <span>1.0.546 · preview</span>')
    s=s.replace('1.0.546 · preview','1.0.546 r2 · preview')
    s=s.replace('Édition 1.0.545 — octobre 2026','Préparation locale 1.0.546 — octobre 2026')
    s=s.replace('Edition 1.0.545','Preview 1.0.546')
    s=s.replace('?rev=545-e8cf3e3','?rev=546-r2').replace('?rev=546-preview','?rev=546-r2')
    s=re.sub(r'<section class="notice preparation546".*?</section>\s*','',s,flags=re.S)
    insert=f'<section class="notice preparation546" style="margin:1rem 2rem;padding:1rem" aria-label="{heading}"><h2>{heading}</h2><p>{body}</p><a href="documents/{note}">{heading}</a></section>'
    # Keep the notice outside both dynamic content and the two-column grid.
    s=s.replace('<div class="layout">',insert+'\n  <div class="layout">',1)
    p.write_text(s,encoding='utf-8',newline='\n')
    p=ROOT/'website/assets'/app;s=p.read_text(encoding='utf-8')
    s=s.replace('Wildspade : niveau 6','Wildspade : niveau 5').replace('Wildspade: level 6','Wildspade: level 5')
    if language=='fr':
        s=s.replace('Chaque allié actif reçoit les forces et renforts adaptés à sa faction, un MCV déployable proche et un WOR dans les intros concernées.',
         'Chaque allié actif reçoit les forces et renforts adaptés à sa faction. Dans l’intro Rebels, il reçoit un port stellaire et une petite économie pour acheter son MCV ; les autres intros conservent le départ coop avec MCV déployable.')
        s=s.replace('Dans l’intro Rebels, il reçoit','Dans l’intro Rebels de Tornie, il reçoit')
        new_section='''function campaignRevisionSection(){if(!D.campaignRevision?.modifiedEditions.includes(current.mod))return '';const rows=D.campaignPlans[current.mod];return `<section class="campaign-plan"><h2>Campagnes Tornie · révision 546 r2</h2><p>Un soutien fixe par campagne, même si le principal adversaire change. Missions 001–019 : deux adversaires ; 020–021 : trois ; 022 : quatre. En 002–004, les bâtiments du soutien sont à au moins dix cases des bases et départs MCV alliés, coop comprise. Commencez une nouvelle campagne.</p><table><thead><tr><th>Campagne</th><th>Soutien permanent</th></tr></thead><tbody>${rows.map(row=>`<tr><th>${esc(row.house)}</th><td>${esc(row.persistentSupport)}</td></tr>`).join('')}</tbody></table></section>`;}'''
    else:
        s=s.replace('Each active ally receives matching starting forces and reinforcements, a nearby deployable MCV and an intro WOR when applicable.',
         'Each active ally receives matching forces and reinforcements. In the Rebels opening, allies receive a starport and a small economy to buy their MCV; other openings retain the cooperative deployable MCV start.')
        s=s.replace('In the Rebels opening, allies receive','In Tornie’s Rebels opening, allies receive')
        new_section='''function campaignRevisionSection(){if(!D.campaignRevision?.modifiedEditions.includes(current.mod))return '';const rows=D.campaignPlans[current.mod];return `<section class="campaign-plan"><h2>Tornie campaigns · revision 546 r2</h2><p>One persistent support opponent per campaign, even when the primary opponent changes. Missions 001–019: two opponents; 020–021: three; 022: four. In 002–004, support buildings stay at least ten tiles from allied bases and MCV starts, including co-op. Start a new campaign.</p><table><thead><tr><th>Campaign</th><th>Persistent support</th></tr></thead><tbody>${rows.map(row=>`<tr><th>${esc(row.house)}</th><td>${esc(row.persistentSupport)}</td></tr>`).join('')}</tbody></table></section>`;}'''
    if 'function campaignRevisionSection()' in s:
        s=re.sub(r'function campaignRevisionSection\(\).*?\n(?=function campaignSection)',new_section+'\n',s,flags=re.S)
    else:
        s=s.replace('function campaignSection(){',new_section+'\nfunction campaignSection(){')
        s=s.replace('function campaignSection(){const rows=',"function campaignSection(){if(D.campaignRevision?.modifiedEditions.includes(current.mod))return campaignRevisionSection();const rows=",1)
    lines=s.splitlines()
    for i,line in enumerate(lines):
        if line.startswith('function coopSection()'):
            lines[i]=line.replace('1.0.545','1.0.546 r2').replace('protocole 22','protocole 24').replace('protocol 22','protocol 24')
    s='\n'.join(lines)+'\n'
    p.write_text(s,encoding='utf-8',newline='\n')
print('Existing FR/EN website updated for the local 546 preview; published 545 URLs retained.')
