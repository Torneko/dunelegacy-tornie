#!/usr/bin/env python3
"""Create the local Windows preview without changing installed game profiles."""
from pathlib import Path
import argparse
import hashlib
import json
import shutil
import subprocess
import zipfile

ROOT=Path(__file__).resolve().parents[1]
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--release',action='store_true',help='Build the final portable Windows package in a separate folder')
parser.add_argument('--guide',type=Path,help='Prepared bilingual guide directory')
args=parser.parse_args()
OUT=ROOT.parent/('DuneLegacyTornie-1.0.546-Windows-x64' if args.release else 'Dune-Legacy-Tornie-1.0.546-Windows-preview-r2')
assert OUT.resolve().parent==ROOT.parent.resolve()
BIN=ROOT/'windows-build-546/bin'
RUNTIME=OUT/'runtime'
RUNTIME.mkdir(parents=True,exist_ok=True)
for name in ('dunelegacy.exe',*[p.name for p in BIN.glob('*.dll')]):
    shutil.copy2(BIN/name,RUNTIME/name)
for folder in ('data','mods','config'):
    shutil.copytree(BIN/folder,RUNTIME/folder,dirs_exist_ok=True)
shutil.copytree(args.guide or ROOT/'website',OUT/'guide',dirs_exist_ok=True)
for name in ('COPYING','AUTHORS','README.md'):
    shutil.copy2(ROOT/name,OUT/name)
source_dir=OUT/'source';source_dir.mkdir(exist_ok=True)
base='89b1403f603a709a6883431adcfd91e317c859b7'
patch=subprocess.check_output(['git','diff','--binary','--no-ext-diff',base],cwd=ROOT)
(source_dir/'changes-546.patch').write_bytes(patch)
added=subprocess.check_output(['git','ls-files','--others','--exclude-standard','-z'],cwd=ROOT).decode('utf-8').split('\0')
for name in added:
    if not name:continue
    item=ROOT/name
    assert item.resolve().is_relative_to(ROOT.resolve())
    if item.is_file():
        dest=source_dir/'additions'/name;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(item,dest)
(source_dir/'README.txt').write_text(f'''Corresponding source for the unpublished local 1.0.546 preview

Upstream: https://github.com/Torneko/dunelegacy-tornie
Base commit: {base}
1. Obtain the repository and check out the exact base commit above.
2. Apply changes-546.patch using git apply --binary.
3. Copy the contents of additions/ over that checkout, preserving paths.
4. Follow the repository README/CMake instructions to build.

COPYING and AUTHORS retain the upstream licence and attribution. The patch
contains all tracked local changes; additions contains every new source/data
file. Generated binaries, private profiles and temporary reports are excluded.
''',encoding='utf-8')
notes=OUT/'notes';notes.mkdir(exist_ok=True)
for name in ('Preparation-1.0.546-FR.md','Preparation-1.0.546-EN.md'):
    shutil.copy2(ROOT/'website/documents'/name,notes/name)
for name in ('Preparation-1.0.546.json','Campaigns-1.0.546.json','Validation-1.0.546-local.json'):
    source=ROOT/'release_notes'/name
    if source.is_file():shutil.copy2(source,notes/name)
evidence=notes/'validation';evidence.mkdir(exist_ok=True)
for original,name in (
 ('reports/ctest-r2.log','ctest.log'),
 ('reports/native-546/verify-release546/runtime.log','native-editor-campaigns.log'),
 ('reports/native-546/verify-release546/campaign-starts.csv','solo-starts.csv'),
 ('reports/native-546/verify-release546-frames/runtime.log','native-frames.log'),
 ('reports/coop-r2.json','coop-scenario-audit.json'),
 ('reports/campaign-scope-r2.json','scope-and-distance-audit.json')):
    shutil.copy2(ROOT/original,evidence/name)
profile=OUT/'profile/mods';profile.mkdir(parents=True,exist_ok=True)
if not (profile/'active_mod.txt').exists():(profile/'active_mod.txt').write_text('Tornie\n',encoding='utf-8')
(OUT/'Lancer-1.0.546.cmd').write_text('@echo off\nset "DUNELEGACY_USER_DIR=%~dp0profile"\ncd /d "%~dp0runtime"\nstart "Dune Legacy Tornie 1.0.546" "dunelegacy.exe"\n',encoding='ascii',newline='\r\n')
(OUT/'Ouvrir-le-guide.cmd').write_text('@echo off\nstart "" "%~dp0guide\\index.html"\n',encoding='ascii',newline='\r\n')
(OUT/'LIRE-AVANT-DE-JOUER.txt').write_text('''DUNE LEGACY TORNIE 1.0.546 R2 — PREPARATION LOCALE

1. Extraire tout le ZIP dans un dossier normal.
2. Lancer Lancer-1.0.546.cmd. Le premier démarrage prépare les mods et peut prendre quelques instants.
3. Commencer une nouvelle campagne pour découvrir les nouvelles missions.
4. Dans l’éditeur, ouvrir la liste des cartes puis choisir Campagne - Vanilla ou Campagne COOP - Vanilla.

Les préférences et sauvegardes de cette préparation se trouvent dans profile/.
Le prototype Extended 0.2 et les jeux installés conservent leurs propres dossiers.
Les campagnes révisées sont celles de Tornie et Tornie Lite uniquement. Jericho et ArcaKev, Lite compris, retrouvent leurs campagnes et coops de référence.
Le soutien est fixe par campagne (Rebels : Harkonnen). Trois adversaires en 020–021, quatre en 022. Bases de soutien éloignées en 002–004.
En réseau, tous les postes doivent utiliser ce même paquet et le même mod : protocole 24.
Le guide FR/EN s’ouvre avec Ouvrir-le-guide.cmd. Ses liens publics restent ceux de la 545 publiée.

Les tests automatisés ne remplacent pas un équilibrage après plusieurs parties ni un essai coop humain sur deux PC.
Voir notes/Validation-1.0.546-local.json et les notes FR/EN pour les vérifications effectuées.

Sources locales modifiées : ../Dune-Legacy-1.0.546
Sources reconstructibles incluses : source/README.txt, patch et fichiers ajoutés.
Projet d’origine : https://github.com/Torneko/dunelegacy-tornie
Cette préparation n’a pas été publiée dans le dépôt distant. Licence et auteurs : COPYING et AUTHORS.
''',encoding='utf-8',newline='\r\n')
if args.release:
    for language in ('FR','EN'):
        shutil.copy2(ROOT/f'release_notes/1.0.546-{language}.md',notes/f'1.0.546-{language}.md')
    readme=OUT/'LIRE-AVANT-DE-JOUER.txt'
    text=readme.read_text(encoding='utf-8')
    text=text.replace('1.0.546 R2 — PREPARATION LOCALE','1.0.546 — WINDOWS PORTABLE')
    text=text.replace('de cette préparation','de cette version')
    text=text.replace('Ses liens publics restent ceux de la 545 publiée.','Voir les notes 1.0.546 FR/EN dans notes/.')
    text=text.replace('Cette préparation n’a pas été publiée dans le dépôt distant.','Le rapport de livraison précise le statut de publication et les plateformes vérifiées.')
    readme.write_text(text,encoding='utf-8',newline='\r\n')
    source_readme=source_dir/'README.txt'
    source_readme.write_text(source_readme.read_text(encoding='utf-8').replace('the unpublished local 1.0.546 preview','Dune Legacy Tornie 1.0.546'),encoding='utf-8')
manifest={str(p.relative_to(OUT)).replace('\\','/'):hashlib.sha256(p.read_bytes()).hexdigest()
          for p in OUT.rglob('*') if p.is_file() and 'profile' not in p.relative_to(OUT).parts and p.name!='SHA256.json'}
(OUT/'SHA256.json').write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8')
archive=Path(str(OUT)+'.zip')
with zipfile.ZipFile(archive,'w',compression=zipfile.ZIP_DEFLATED,compresslevel=6) as target:
    for item in sorted(OUT.rglob('*')):
        if not item.is_file():continue
        relative=item.relative_to(OUT)
        if relative.parts[0]=='profile' and relative.as_posix()!='profile/mods/active_mod.txt':continue
        target.write(item,Path(OUT.name)/relative)
print(json.dumps(dict(folder=str(OUT),archive=str(archive),bytes=archive.stat().st_size,
    sha256=hashlib.sha256(archive.read_bytes()).hexdigest(),files=len(manifest)),indent=2))
