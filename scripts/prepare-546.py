#!/usr/bin/env python3
"""Prepare version-546 asset bindings, faction tech and editable Vanilla maps."""
from pathlib import Path
import hashlib
import importlib.util
import json
import re
import subprocess

ROOT = Path(__file__).resolve().parents[1]
BASELINE = '89b1403f603a709a6883431adcfd91e317c859b7'
MODS = ('Tornie','Jericho','ArcaKev','TornieLite','JerichoLite','ArcaKevLite')


def set_field(path, section, key, value):
    text = path.read_bytes().decode('latin1')
    match = re.search(r'^\['+re.escape(section)+r'\][^\r\n]*\r?\n(.*?)(?=^\[|\Z)', text, re.M|re.S)
    if not match:
        return False
    body = match.group(1)
    pattern = r'^'+re.escape(key)+r'\s*=[^\r\n]*'
    if re.search(pattern, body, re.M):
        body = re.sub(pattern, key+' = '+str(value), body, flags=re.M)
    else:
        eol = '\r\n' if '\r\n' in text else '\n'
        body = body.rstrip('\r\n')+eol+key+' = '+str(value)+eol+eol
    path.write_bytes((text[:match.start(1)]+body+text[match.end(1):]).encode('latin1'))
    return True


def main():
    frames = []
    for folder in ('data','website/assets/preparation545'):
        for target, source in (('Atreides','Ordos'),('Ordos','Atreides')):
            src = f'{folder}/CampaignHerald{source}.png'
            data = subprocess.check_output(['git','show',f'{BASELINE}:{src}'],cwd=ROOT)
            dest = ROOT / folder / f'CampaignHerald{target}.png'
            dest.write_bytes(data)
            frames.append(dict(path=dest.relative_to(ROOT).as_posix(),source=src,sha256=hashlib.sha256(data).hexdigest()))
    for mod in MODS:
        path = ROOT / 'mods' / mod / 'ObjectData.ini'
        set_field(path,'Hightech Factory','TechLevel(W)',4)
        set_field(path,'Carryall','TechLevel(W)',4)
        set_field(path,'Chemical Carryall','TechLevel(W)',5)
        set_field(ROOT/'mods'/mod/'mod.ini','Mod','Game Version','1.0.546')
    # Wildspade remains gated by each edition's roster. Other Vanilla factions
    # retain their original technology; no Chemical Carryall is added to Vanilla.
    set_field(ROOT/'config/ObjectData.ini.default','Hightech Factory','TechLevel(W)',4)
    set_field(ROOT/'config/ObjectData.ini.default','Carryall','TechLevel(W)',4)
    spec = importlib.util.spec_from_file_location('original_coop', ROOT/'scripts/original-coop-campaign.py')
    original = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(original)
    exported = []
    for house in (*original.NAMES[:8], 'Kleshmersh'):
        for mission in range(1,23):
            data, origin = original.source_bytes('vanilla',house,mission)
            name = f'SCEN{original.LETTERS[house]}{mission:03}.INI'
            dest = ROOT/'data/campaign_vanilla'/name.lower()
            dest.parent.mkdir(parents=True,exist_ok=True)
            dest.write_bytes(data)
            exported.append(dict(file=name,source=origin,sha256=hashlib.sha256(data).hexdigest()))
    report = dict(baseline=BASELINE,frames=frames,vanillaEditorMaps=exported,
                  wildspadeTech=dict(hightech=4,carryall=4,chemicalCarryall=5,ornithopter=7))
    (ROOT/'release_notes/Preparation-1.0.546.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(dict(correctedFrames=len(frames),vanillaEditorMaps=len(exported),wildspadeTech=report['wildspadeTech'])))


if __name__ == '__main__':
    main()
