#!/usr/bin/env python3
"""Audit campaign scope, persistent opponents and solo/co-op base separation."""
from pathlib import Path
import hashlib
import importlib.util
import json
ROOT=Path(__file__).resolve().parents[1]
(ROOT/'reports').mkdir(exist_ok=True)
spec=importlib.util.spec_from_file_location('recipes',ROOT/'scripts/rebuild-campaigns-546.py')
r=importlib.util.module_from_spec(spec);spec.loader.exec_module(r)
foreign=r.restore_other_campaigns(check=True)

def footprint(value,offset,unit=False):
    p=value.split(',');position=int(p[3]);x,y=position%64-offset,position//64-offset
    w,h=(2,2) if unit else r.coop.SIZES[p[1].lower()]
    return {(x+dx,y+dy) for dx in range(w) for dy in range(h)}

def structures(ini,owner,offset,section='STRUCTURES'):
    return set().union(*(footprint(value,offset) for key,value in ini[section].items()
                        if key.startswith('ID') and value.startswith(owner+','))) if section in ini else set()

def distance(a,b):
    assert a and b
    return min(max(abs(x-u),abs(y-v)) for x,y in a for u,v in b)

counts={2:0,3:0,4:0};solo_clearance=[];coop_clearance=[];support_table={}
for mod,rows in r.PLANS.items():
    for row in rows:
        house=row['house'];support=r.POLICY['support'][mod][house]
        support_table[mod+'/'+house]=support
        for mission in range(1,23):
            path=ROOT/f'mods/{mod}/campaign/scen{row["letter"].lower()}{mission:03}.ini'
            data=path.read_bytes();ini=r.coop.read(data);stage=ini.getint('BASIC','TechLevel')
            enemies={name for name in r.coop.NAMES if name!=house and name in ini}
            expected=4 if mission==22 else 3 if mission>=20 else 2
            assert len(enemies)==expected and support in enemies,(mod,house,mission,enemies)
            assert ini['BASIC']['SupportOpponent']==support
            assert all(any(v.startswith(enemy+',Const Yard,') for k,v in ini['STRUCTURES'].items() if k.startswith('ID')) for enemy in enemies)
            counts[expected]+=1
            if mission==1:assert ini.getint(house,'Credits')==1500 and ini.getint(house,'Quota')==2000
            if 2<=mission<=4:
                _,offset,_=r.coop.terrain(ini)
                gap=distance(structures(ini,house,offset),structures(ini,support,offset))
                assert gap>=10,(mod,house,mission,'solo gap',gap)
                solo_clearance.append(gap)
            variant=mission-r.coop.MISSIONS[stage-1]
            coop_path=r.coop.destination(mod,r.coop.runtime_faction(mod,house),stage,variant)
            coop=r.coop.read(coop_path.read_bytes())
            marker=coop['COOP_TEMPLATE']
            assert marker['SourceOpponent2']==support
            assert marker['SourceSha256']==r.coop.canonical_source_hash(data)
            assert marker['ParticipantCount']=='6'
            assert sum(int(marker[f'EnemyFaction{n}'])>=0 for n in range(1,5))==expected
            if 2<=mission<=4:
                enemy_cells=structures(coop,'Player4',offset)
                allied_cells=structures(coop,'Player1',offset)|structures(coop,'Player2',offset)
                for section in ('UNITS','COOP_EXTRA_UNITS3','COOP_EXTRA_UNITS4'):
                    for key,value in coop[section].items():
                        if value.split(',')[1]=='MCV' and (key.startswith('IDCOOP') or value.startswith('Player1,')):
                            allied_cells.update(footprint(value,offset,unit=True))
                gap=distance(allied_cells,enemy_cells)
                assert gap>=10,(mod,house,mission,'cooperative gap',gap)
                coop_clearance.append(gap)
report=dict(revision=2,modifiedEditions=list(r.PLANS),campaigns=18,soloMaps=sum(counts.values()),
    opponentCounts=counts,support=support_table,foreignCampaignFilesByteIdentical=len(foreign),
    stage2SoloVariants=len(solo_clearance),minimumStage2SoloGap=min(solo_clearance),
    stage2CoopVariantsIncludingFourStarts=len(coop_clearance),minimumStage2CoopGap=min(coop_clearance),
    foreignCampaignFingerprint=hashlib.sha256(json.dumps(foreign,sort_keys=True).encode()).hexdigest())
(ROOT/'reports/campaign-scope-r2.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
print(json.dumps(report,ensure_ascii=False))
