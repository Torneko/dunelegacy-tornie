#!/usr/bin/env python3
"""Deterministic 546 campaign recipes, based on the reviewed 545 scenarios.

Only Tornie/TornieLite campaigns are changed. Other authors' campaigns and
Vanilla archives remain byte-identical to the reviewed baseline.
Normal production follows the runtime-audited faction routes. Initial gifts and
one-stage-ahead finite reinforcement detachments are explicit scenario bonuses.
"""
from pathlib import Path
from functools import lru_cache
import argparse
import hashlib
import importlib.util
import io
import json
import subprocess
import tarfile

ROOT=Path(__file__).resolve().parents[1]
BASE='89b1403f603a709a6883431adcfd91e317c859b7'
spec=importlib.util.spec_from_file_location('coop',ROOT/'scripts/original-coop-campaign.py')
coop=importlib.util.module_from_spec(spec);spec.loader.exec_module(coop)
ALL_PLANS=json.loads((ROOT/'config/CampaignPlans.json').read_text(encoding='utf-8'))['mods']
POLICY=json.loads((ROOT/'config/TornieCampaign546.json').read_text(encoding='utf-8'))
PLANS={mod:ALL_PLANS[mod] for mod in POLICY['modifiedEditions']}
ALIASES={'Construction Yard':'Const Yard','Light Factory':'Light Fctry','Heavy Factory':'Heavy Fctry',
         'Hightech Factory':'Hi-Tech','House IX':'IX','Radar':'Outpost','Starport':'Starport',
         'Gun-Turret':'Turret','Rocket-Turret':'R-Turret','Repair Yard':'Repair',
         'Advanced Windtrap':'Advanced Windtrap 3x3'}
EXTRA_SIZES={'advanced windtrap 3x3':(3,3),'advanced windtrap 2x3':(2,3),
 'advanced windtrap 3x2':(3,2),'worfinery':(3,2),'doublefinery':(5,2),
 'tech center':(3,2),'love factory':(3,2),'chaos factory':(3,2),
 'scoutpost':(1,1),'flamepost':(1,1),'chemipost':(1,1),'slowpost':(1,1)}
coop.SIZES.update(EXTRA_SIZES)
FAMILIES={
 'foot':('Soldier','Infantry','Trooper','Troopers','Gas Infantry','Flame Trooper'),
 'scout':('Trike','Raider Trike','Quad','Rocket Trike','Sonic Trike','Flame Quad','Raider Quad'),
 'armour':('Tank','Ranged Tank','Slow Tank','Siege Tank','Flame Tank','Elite Siege Tank','Chemical Siege Tank','Devastator','Sonic Tank','Deviator'),
 'support':('Launcher','Elite Launcher','Sonic Tank','Deviator'),
 'worker':('Harvester','Rebel Harvester','Capacity Harvester','Fast Harvester')}
FAMILY={unit:family for family,units in FAMILIES.items() for unit in units}


@lru_cache(None)
def baseline():
    payload=subprocess.check_output(['git','archive',BASE,'--','mods','website/assets/site-data.js'],cwd=ROOT)
    with tarfile.open(fileobj=io.BytesIO(payload)) as archive:
        return {p.name:archive.extractfile(p).read() for p in archive if p.isfile()}


def restore_other_campaigns(check=False):
    """Restore only campaigns/briefings belonging to the other authors."""
    restored=[]
    for name,data in baseline().items():
        parts=Path(name).parts
        if len(parts)<4 or parts[0]!='mods' or parts[1] not in POLICY['preservedEditions']:continue
        campaign=parts[2]=='campaign'
        mirror=parts[2]=='data' and (parts[-1].lower().startswith('scen') or parts[-1]=='CampaignPlan.ini')
        if not (campaign or mirror):continue
        target=ROOT/name
        assert target.resolve().is_relative_to(ROOT.resolve())
        if check:assert target.read_bytes()==data,('Foreign campaign changed',name)
        elif not target.is_file() or target.read_bytes()!=data:
            target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(data)
        restored.append(dict(file=name,sha256=hashlib.sha256(data).hexdigest()))
    return restored


@lru_cache(None)
def stage_opponents(mod,house):
    row=next(r for r in ALL_PLANS[mod] if r['house']==house)
    rivals=row['opponents'];support=POLICY['support'][mod][house]
    assert support not in (house,*rivals)
    result={}
    for stage,start in enumerate(coop.MISSIONS,1):
        stop=coop.MISSIONS[stage] if stage<9 else 23
        scores={enemy:0 for enemy in rivals}
        for mission in range(start,stop):
            ini=coop.read(baseline()[f'mods/{mod}/campaign/scen{row["letter"].lower()}{mission:03}.ini'])
            owners,_,_=coop.normalized_owners(mod,house,rivals,mission,ini)
            for (section,key),owner in owners.items():
                if owner not in scores:continue
                value=ini[section][key].split(',')
                if len(value)>1 and value[1].strip()=='Sandworm':continue
                scores[owner]+=4 if section=='STRUCTURES' else 1
        ordered=sorted(rivals,key=lambda enemy:(-scores[enemy],rivals.index(enemy)))
        if stage==1:ordered=[rivals[0],*rivals[1:]]
        result[stage]=(ordered[0],support,*ordered[1:])
    return result


@lru_cache(None)
def routes():
    text=baseline()['website/assets/site-data.js'].decode('utf-8')
    data=json.loads(text[text.index('{'):text.rindex('}')+1])
    result={}
    for item in data['units']+data['buildings']:
        for route in item['routes']:
            level=route.get('level')
            if not isinstance(level,int):continue
            if route['house']=='W' and item['name'] in ('Hightech Factory','Carryall'):level=4
            if route['house']=='W' and item['name']=='Chemical Carryall':level=5
            key=route['mod'],route['house'],item['name']
            result[key]=min(level,result.get(key,100))
    return result


def level(mod,house,item):
    return routes().get((mod,coop.LETTERS[house],item),100)


def choose(mod,house,family,tech,index=0):
    choices=[u for u in FAMILIES[family] if level(mod,house,u)<=tech]
    # A scripted opening infantry detachment is available before factories.
    if not choices:return 'Harvester' if family=='worker' else 'Trooper' if family=='foot' else 'Trike' if family=='scout' else choose(mod,house,'foot',tech,index)
    choices.sort(key=lambda u:(level(mod,house,u),FAMILIES[family].index(u)))
    # Alternate standard and advanced equipment rather than upgrading every unit.
    return choices[-1] if index%3 else choices[index%len(choices)]


class Placement:
    def __init__(self,ini):
        self.ini=ini
        self.size,self.offset,self.cells=coop.terrain(ini)
        self.occupied,self.buildings=coop.occupied_cells(ini,self.offset)
        self.next_id=6000
        self.added=[]
        self.protected=set()
        for field in ('Bloom','Special'):
            for p in ini['MAP'].get(field,'').split(','):
                if p.strip():self.protected.add(self.xy(int(p)))

    def xy(self,p):return p%64-self.offset,p//64-self.offset
    def pos(self,p):return (p[1]+self.offset)*64+p[0]+self.offset
    def key(self):
        self.next_id+=1
        while 'ID'+str(self.next_id) in self.ini['STRUCTURES'] or 'ID'+str(self.next_id) in self.ini['UNITS']:
            self.next_id+=1
        return 'ID'+str(self.next_id)
    def origin(self,owner):
        values=[self.xy(int(v.split(',')[3])) for k,v in self.ini['STRUCTURES'].items()
                if k.startswith('ID') and v.split(',')[0]==owner and v.split(',')[1] in ('Const Yard','Starport','Star Port')]
        if not values:values=[self.xy(int(v.split(',')[3])) for v in self.ini['UNITS'].values() if v.split(',')[0]==owner]
        return values[0] if values else (self.size//2,self.size//2)
    def find(self,w,h,anchor,avoid=(),minimum=0,forbidden=frozenset()):
        forbidden=set(forbidden)
        for ax,ay in avoid:
            forbidden.update((x,y) for y in range(max(0,ay-minimum+1),min(self.size,ay+minimum))
                             for x in range(max(0,ax-minimum+1),min(self.size,ax+minimum)))
        candidates=[]
        for y in range(1,self.size-h):
            for x in range(1,self.size-w):
                area={(x+dx,y+dy) for dy in range(h) for dx in range(w)}
                if not area.isdisjoint(self.occupied|self.protected|forbidden):continue
                if any(self.cells[p]=='a' for p in area):continue
                distance=max(abs(x-anchor[0]),abs(y-anchor[1]))
                sand=sum(self.cells[p] not in '28' for p in area)
                candidates.append((distance+2*sand,distance,y,x))
        assert candidates,('No free footprint',w,h,anchor)
        _,_,y,x=min(candidates)
        return x,y
    def building(self,owner,item,anchor,avoid=(),minimum=0,exact=False):
        w,h=coop.SIZES[item.lower()]
        point=anchor if exact else self.find(w,h,anchor,avoid,minimum)
        area={(point[0]+dx,point[1]+dy) for dy in range(h) for dx in range(w)}
        for p in sorted(area):
            # Slabs make each added base functional and avoid sand damage.
            pos=self.pos(p);self.ini['STRUCTURES']['GEN'+str(pos)]=owner+',Concrete';self.cells[p]='8'
        self.ini['STRUCTURES'][self.key()]=f'{owner},{item},256,{self.pos(point)}'
        self.occupied.update(area);self.buildings.update(area)
        self.added.append(dict(owner=owner,item=item,position=self.pos(point)))
        return point
    def unit(self,owner,item,anchor,mode='Area Guard',avoid=(),minimum=0):
        p=self.find(1,1,anchor,avoid,minimum)
        self.ini['UNITS'][self.key()]=f'{owner},{item},256,{self.pos(p)},64,{mode}'
        self.occupied.add(p)


def write(ini):
    out=io.StringIO();ini.write(out,space_around_delimiters=False)
    return ('; Dune Legacy Tornie 1.0.546 campaign recipe; original terrain seed retained.\n'+out.getvalue()).encode('cp850')


def rebuild(mod,row,mission):
    house=row['house'];original_roles=row['opponents'];stage=max(i+1 for i,n in enumerate(coop.MISSIONS) if mission>=n)
    path=f'mods/{mod}/campaign/scen{row["letter"].lower()}{mission:03}.ini'
    source=baseline()[path];ini=coop.read(source)
    owners,_,_=coop.normalized_owners(mod,house,original_roles,mission,ini)
    full_roles=stage_opponents(mod,house)[stage]
    roles=full_roles[:(4 if stage==9 else 3 if stage==8 else 2)]
    for section in coop.OWNED_SECTIONS:
        if section not in ini:ini.add_section(section)
        for key,value in list(ini[section].items()):
            p=value.split(',');owner=owners[section,key]
            if owner in original_roles and owner not in roles:owner=roles[2] if stage==8 and len(roles)>2 else roles[0]
            p[0]=owner if owner!='CoopWildlife' else p[0]
            ini[section][key]=','.join(p)
    for enemy in coop.NAMES:
        if enemy!=house and enemy not in roles and enemy in ini:ini.remove_section(enemy)
    ini['BASIC']['TechLevel']=str(stage)
    ini['BASIC']['CampaignRecipe']='546'
    ini['BASIC'].update(CampaignRevision='2',PrimaryOpponent=roles[0],SupportOpponent=roles[1],
                        LateOpponent=full_roles[2],FinalOpponent=full_roles[3],OpponentCount=str(len(roles)))
    if stage==2:ini['BASIC']['SupportBaseClearance']=str(POLICY['stage2BaseClearance'])
    # Retain original objectives and terrain outside the revised openings.
    if stage==1:
        ini[house]['Credits']='1500';ini[house]['Quota']='2000'
        ini['BASIC']['OpeningStyle']='Trade' if house=='Rebels' else 'Aviation' if house=='Wildspade' else 'Economy'
    for role,enemy in enumerate(roles):
        if enemy not in ini:ini[enemy]={}
        ini[enemy].update(Brain='CPU',Quota='0',MaxUnit=str(18+3*stage),Credits=str((600 if role else 900)+stage*150))
    # Every non-worker unit and existing delivery follows faction equipment.
    # The two opening bonus tanks remain explicit gifts; Special is replaced
    # by a concrete, modest advanced detachment, not a random super-unit.
    gifts=[]
    for section in ('UNITS','REINFORCEMENTS'):
        for index,(key,value) in enumerate(list(ini[section].items())):
            p=[x.strip() for x in value.split(',')];owner,item=p[:2]
            if owner not in (house,*roles) or item in ('Sandworm','MCV','Carryall','Chemical Carryall','Ornithopter','Frigate'):continue
            own=owner==house;tech=stage if own else min(9,stage+1)
            if stage==1 and section=='UNITS' and own and item in ('Tank','Troopers'):
                gifts.append(key);continue
            if item=='Special':
                p[1]=choose(mod,owner,'scout' if stage<4 else 'armour',min(9,stage+1),index);gifts.append(key)
            elif item in FAMILY:
                p[1]=choose(mod,owner,FAMILY[item],tech,index) if FAMILY[item]!='worker' else choose(mod,owner,'worker',tech,index)
            ini[section][key]=','.join(p)
    # New enemy bases use original connected terrain and add their own slabs.
    placement=Placement(ini);home=placement.origin(house)
    home_cells=set()
    army=[]
    for key,value in ini['STRUCTURES'].items():
        p=value.split(',')
        if not key.startswith('ID') or p[0]!=house:continue
        x,y=placement.xy(int(p[3]));w,h=coop.SIZES[p[1].lower()]
        home_cells.update((x+dx,y+dy) for dx in range(w) for dy in range(h))
    for value in ini['UNITS'].values():
        p=value.split(',')
        if p[0]==house:army.append(placement.xy(int(p[3])))
    added_bases=[]
    for role,enemy in enumerate(roles):
        existing=[v for k,v in ini['STRUCTURES'].items() if k.startswith('ID') and v.startswith(enemy+',Const Yard,')]
        if existing:continue
        far_x=2 if home[0]>placement.size//2 else placement.size-8
        far_y=2 if home[1]>placement.size//2 else placement.size-7
        corners=((far_x,far_y),(2 if far_x>2 else placement.size-8,far_y),(far_x,2 if far_y>2 else placement.size-7))
        target=corners[role%len(corners)]
        clearance=POLICY['stage2BaseClearance'] if stage==2 and role==1 else 16 if stage==1 else 10
        # Plan the entire small base as one footprint, so no support building
        # drifts back toward the player while being placed around its yard.
        anchor=placement.find(6,5,target,avoid=home_cells,minimum=clearance)
        added_bases.append(anchor)
        for item,dx,dy in (('Const Yard',0,0),('Windtrap',3,0),('Refinery',0,3)):
            placement.building(enemy,item,(anchor[0]+dx,anchor[1]+dy),exact=True)
        if stage>=2:
            factory='WOR' if level(mod,enemy,'WOR')<=stage+1 else 'Barracks'
            if level(mod,enemy,factory)<=stage+1:placement.building(enemy,factory,(anchor[0]+4,anchor[1]+3),exact=True)
        placement.unit(enemy,choose(mod,enemy,'worker',stage+1),anchor)
        for n in range(3+stage//3):placement.unit(enemy,choose(mod,enemy,'foot' if n%2 else 'scout',min(9,stage+1),n),anchor,
            avoid=army,minimum=POLICY['stage2ArmyClearance'] if stage==2 and role==1 else 0)
    if stage==1:
        if house=='Rebels':
            for key,value in list(ini['STRUCTURES'].items()):
                if key.startswith('ID') and value.split(',')[0]==house:del ini['STRUCTURES'][key]
            # Reclaim the old yard/WOR footprints for the new trade post.
            enemy_additions=placement.added.copy()
            placement=Placement(ini)
            placement.added=enemy_additions
            for item in ('Starport','Windtrap','Windtrap','Refinery'):
                placement.building(house,item,home)
            placement.unit(house,'Harvester',home,'Guard')
            ini['CHOAM']={'MCV':'1','Harvester':'1','Trike':'3','Quad':'2'}
        else:
            placement.building(house,'Windtrap',home)
        if house=='Wildspade':
            # Loaned aircraft teach reconnaissance/transport without unlocking
            # Ornithopter production before its normal stage-seven gate.
            placement.unit(house,'Carryall',home,'Guard')
            placement.unit(house,'Ornithopter',home,'Guard')
            gifts.extend(['loaned Carryall','loaned Ornithopter'])
            # Limited enemy missile infantry punish careless low passes.
            for enemy in roles[:2]:placement.unit(enemy,'Trooper',placement.origin(enemy))
    # Place one visible faction-specific facility as it becomes available.
    facilities=('Worfinery','Doublefinery','Advanced Windtrap','Advanced Windtrap 2x3','Advanced Windtrap 3x2','Scoutpost','Flamepost','Chemipost','Slowpost','Tech Center','Love Factory','Chaos Factory')
    for owner in (house,*roles):
        unlocked=[f for f in facilities if level(mod,owner,f)<=stage]
        if unlocked and (stage>=5 or owner!=house):
            # Rotate late-game facilities over scenario variants.
            item=unlocked[(mission+coop.NAMES.index(owner))%len(unlocked)]
            placement.building(owner,ALIASES.get(item,item),placement.origin(owner),avoid=home_cells if stage==2 and owner==roles[1] else (),
                minimum=POLICY['stage2BaseClearance'] if stage==2 and owner==roles[1] else 0)
    # Finite, staggered reinforcements on both sides. Existing waves are kept
    # with their times/locations and translated equipment.
    deliveries=[]
    waves=[(house,5,2+stage//3),(roles[0],6,2+stage//2),(roles[1],9,2+stage//3)]
    waves.extend((enemy,11+2*i,2+stage//3) for i,enemy in enumerate(roles[2:]))
    for owner,minute,count in waves:
        for n in range(count):
            tech=min(9,stage+(1 if owner==house and stage in (3,5,7) else 0))
            unit=choose(mod,owner,'scout' if n%2==0 and stage<5 else 'armour' if stage>=4 else 'foot',tech,n+1)
            key='546'+str(len(deliveries)+1)
            ini['REINFORCEMENTS'][key]=f'{owner},{unit},{"Homebase" if owner==house else "Enemybase"},{minute}'
            deliveries.append(dict(owner=owner,unit=unit,minute=minute,advancedGift=owner==house and tech>stage))
    data=write(ini)
    return path,data,dict(mod=mod,house=house,mission=mission,stage=stage,terrainSeed=ini['MAP']['Seed'],
        initialCredits=int(ini[house]['Credits']),quota=int(ini[house].get('Quota','0')),
        opponentSupport=roles[1],opponents=list(roles),opponentCount=len(roles),addedBuildings=placement.added,reinforcements=deliveries,scriptedOpeningGifts=gifts,
        sourceSha256=hashlib.sha256(source).hexdigest(),sha256=hashlib.sha256(data).hexdigest())


def main():
    parser=argparse.ArgumentParser();parser.add_argument('--check',action='store_true')
    parser.add_argument('--only-openings',action='store_true');args=parser.parse_args()
    reports=[]
    restored=restore_other_campaigns(check=args.check)
    for mod,rows in PLANS.items():
        for row in rows:
            for mission in ((1,) if args.only_openings else range(1,23)):
                path,data,report=rebuild(mod,row,mission)
                if args.check:assert (ROOT/path).read_bytes()==data,path
                else:
                    (ROOT/path).write_bytes(data)
                    mirror=ROOT/'mods'/mod/'data'/Path(path).name
                    if mirror.is_file():mirror.write_bytes(data)
                reports.append(report)
        plan=ROOT/'mods'/mod/'data/CampaignPlan.ini'
        original=coop.read(baseline()[plan.relative_to(ROOT).as_posix()])
        for row in rows:
            section=original[row['house']];section['OpeningQuota']='2000';section['CampaignRevision']='2'
            section['PersistentSupport']=POLICY['support'][mod][row['house']]
            for stage,enemies in stage_opponents(mod,row['house']).items():
                active=4 if stage==9 else 3 if stage==8 else 2
                for index,enemy in enumerate(enemies[:active],1):section[f'Stage{stage}Opponent{index}']=enemy
        output=io.StringIO();original.write(output,space_around_delimiters=False)
        if not args.check:plan.write_text(output.getvalue(),encoding='utf-8',newline='\n')
    if args.only_openings:
        old=json.loads((ROOT/'release_notes/Campaigns-1.0.546.json').read_text(encoding='utf-8'))['maps']
        changed={(r['mod'],r['house'],r['mission']):r for r in reports}
        reports=[changed.get((r['mod'],r['house'],r['mission']),r) for r in old if r['mod'] in PLANS]
    report=dict(version='1.0.546',revision=2,baseline=BASE,soloMaps=len(reports),campaigns=sum(map(len,PLANS.values())),
      modifiedEditions=list(PLANS),restoredForeignCampaignFiles=restored,support=POLICY['support'],
      policy='Runtime faction routes; finite support waves; explicit opening and occasional one-stage-ahead gifts.',maps=reports)
    if not args.check:(ROOT/'release_notes/Campaigns-1.0.546.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(dict(soloMaps=len(reports),campaigns=report['campaigns'],check=args.check)))


if __name__=='__main__':main()
