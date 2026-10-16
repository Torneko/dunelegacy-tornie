#!/usr/bin/env python3
"""Bootstrap ArcaKev from Jericho, then rebuild its distinct campaign role plans.

Faction IDs stay stable. The requested menu order is presentation metadata.
Only the new mods are written; source campaigns and artwork stay untouched.
"""
from pathlib import Path
import argparse, configparser, itertools, json, re, shutil
ROOT = Path(__file__).resolve().parents[1]
NAMES = ['Harkonnen','Atreides','Ordos','Fremen','Sardaukar','Mercenary',
         'Neutral','Rebels','Corruptique','Wildspade','Kleshmersh','Tharpique']
ORDER = NAMES[:6]+['Kleshmersh','Wildspade','Rebels','Neutral','Tharpique','Corruptique']
PREFIX = dict(zip(NAMES,['HAR','ATR','ORD','FRE','SAR','MER','NEU','REB','COR','WIL','KLE','THA']))

def remap(text, mapping):
    tokens = re.compile(r'\b(?:'+'|'.join(sorted(mapping,key=len,reverse=True))+r')\b')
    return tokens.sub(lambda m:mapping[m[0]],text)

def build():
    path=ROOT/'config/CampaignPlans.json';plans=json.loads(path.read_text())
    for mod, source, offsets in [('ArcaKev','Jericho',(3,6,10)),('ArcaKevLite','JerichoLite',(2,3,4))]:
        dest=ROOT/'mods'/mod;src=ROOT/'mods'/source
        if not dest.exists():shutil.copytree(src,dest)
        pool=NAMES[:6] if mod.endswith('Lite') else NAMES
        order=pool if mod.endswith('Lite') else ORDER
        original={r['house']:r for r in plans['mods'][source]}
        if mod not in plans['mods']:
            plans['mods'][mod]=[dict(original[house],opponents=[pool[(pool.index(house)+n)%len(pool)] for n in offsets]) for house in order]
        rows=plans['mods'][mod]
        assert [r['house'] for r in rows]==order
        briefing='; ArcaKev role plan; generated from config/CampaignPlans.json.\r\n'
        for row in rows:
            house=row['house'];old=original[house]
            mapping=dict(zip(old['opponents'],row['opponents']))
            for mission in range(1,23):
                filename=f"scen{row['letter'].lower()}{mission:03}.ini"
                content=remap((src/'campaign'/filename).read_bytes().decode('cp850'),mapping).encode('cp850')
                (dest/'campaign'/filename).write_bytes(content)
                mirror=src/'data'/filename
                if mirror.exists():(dest/'data'/filename).write_bytes(content)
            prefix_map={PREFIX[a]:PREFIX[b] for a,b in mapping.items()}
            filename=f"REGION{row['letter']}.INI"
            text=(src/'campaign'/filename).read_bytes().decode('cp850')
            text=remap(remap(text,mapping),prefix_map)
            for folder in ['campaign','data']:(dest/folder/filename).write_bytes(text.encode('cp850'))
            ini=configparser.ConfigParser(interpolation=None,strict=False);ini.read_string(content.decode('cp850'))
            opening=configparser.ConfigParser(interpolation=None,strict=False)
            opening.read_string((dest/'campaign'/f"scen{row['letter'].lower()}001.ini").read_bytes().decode('cp850'))
            briefing+=f"\r\n[{house}]\r\nTemplate={row['vanillaTemplate']}\r\n"+''.join(f'Opponent{i+1}={h}\r\n' for i,h in enumerate(row['opponents']))+f"OpeningQuota={opening[house].get('Quota','0')}\r\n"
        (dest/'data/CampaignPlan.ini').write_bytes(briefing.encode())
        info=(dest/'mod.ini').read_text().replace('Display Name = '+source.replace('Lite',' Lite'),'Display Name = '+mod.replace('Lite',' Lite'),1)
        info=re.sub(r'(?m)^Description = .*$', 'Description = '+mod.replace('Lite',' Lite')+': normal, green and blue spice; custom units and buildings; distinct faction campaigns.',info)
        info=re.sub(r'(?m)^Game Version = .*$', 'Game Version = 1.0.541',info)
        info=re.sub(r'(?m)^(Red Spice|Purple Spice) = .*$',r'\1 = false',info)
        (dest/'mod.ini').write_text(info)
        if not mod.endswith('Lite'):
            custom=(ROOT/'mods/Tornie/CustomHouse.ini').read_bytes().replace(b'Palette Index = 224',b'Palette Index = 192')
            (dest/'CustomHouse.ini').write_bytes(custom)
        manifest=json.loads((dest/'manifest.json').read_text())
        manifest['mod'].update(id=mod,displayName=mod.replace('Lite',' Lite'),gameVersion='1.0.541')
        manifest['isolation']['root']='mods/'+mod
        if not mod.endswith('Lite'):
            manifest['customHouse'].update(displayName='Corruptique',scenarioLetter='C',regionPrefix='COR',paletteIndex=192,fallbackHouse=2)
        manifest['factionOrder']=order
        manifest['spiceFamilies']=['normal','green','blue']
        (dest/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
        for file in (dest/'campaign/coop').glob('*.ini'):
            raw=file.read_bytes();raw=raw.replace(('Mod='+source).encode(),('Mod='+mod).encode());file.write_bytes(raw)
        print(mod,len(rows),'campaigns, normal/green/blue spice')
    plans['version']='1.0.541';path.write_text(json.dumps(plans,indent=2)+'\n')

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--write',action='store_true',required=True);parser.parse_args();build()
