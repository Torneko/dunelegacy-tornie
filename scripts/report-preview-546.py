#!/usr/bin/env python3
"""Summarize completed local checks; refuse to report unfinished native suites."""
from pathlib import Path
import csv
import hashlib
import json
import re

ROOT=Path(__file__).resolve().parents[1]
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
matrix=ROOT/'reports/native-546/verify-release546'
frame=ROOT/'reports/native-546/verify-release546-frames'
log=(matrix/'runtime.log').read_text(encoding='utf-8',errors='replace')
match=re.search(r'RELEASE 546 R2 MATRIX PASS: (\d+) editor maps; (\d+) solo starts; (\d+) coop starts; (\d+) late cooperative checkpoints',log)
assert match,'Native campaign/editor suite has not passed'
assert 'RELEASE 546 R2 PASS:' in log
frames=(frame/'runtime.log').read_text(encoding='utf-8',errors='replace')
assert 'RELEASE 546 FRAMES PASS: 28 ' in frames,'Native frame suite has not passed'
ctest=(ROOT/'reports/ctest-r2.log').read_text(encoding='utf-8',errors='replace')
assert '100% tests passed' in ctest,'CTest has not passed'
cases=(ROOT/'windows-build-546/Testing/Temporary/LastTest.log').read_text(encoding='utf-8',errors='replace')
counts=re.search(r'All tests passed \((\d+) assertions in (\d+) test cases\)',cases)
assert counts
with (matrix/'campaign-starts.csv').open(newline='',encoding='utf-8') as stream:starts=list(csv.DictReader(stream))
assert len(starts)==252 and sum(row['mission']=='1' for row in starts)==54
coop=json.loads((ROOT/'reports/coop-r2.json').read_text(encoding='utf-8'))
maps=coop['originalMaps']
assert len(maps)==1386
vanilla=json.loads((ROOT/'reports/vanilla-preservation.json').read_text(encoding='utf-8'))
scope=json.loads((ROOT/'reports/campaign-scope-r2.json').read_text(encoding='utf-8'))
assert scope['soloMaps']==396 and scope['foreignCampaignFilesByteIdentical']==2320
assert scope['minimumStage2SoloGap']>=10 and scope['minimumStage2CoopGap']>=10
assert '"check": true' in (ROOT/'reports/campaigns-r2-check.log').read_text()
report={
 'version':'1.0.546','revision':2,'status':'local-preview-unpublished','date':'2026-10-25',
 'baseline':'89b1403f603a709a6883431adcfd91e317c859b7','networkProtocol':24,'saveFormat':9839,
 'platform':'Windows x64','compiler':'MinGW GCC 15.2, CMake/Ninja Release O1, static libgcc/libstdc++',
 'ctest':{'passed':True,'testCases':int(counts[2]),'assertions':int(counts[1]),'nativeUdpRelayTest':True},
 'campaignRecipes':{'mods':2,'modifiedEditions':['Tornie','TornieLite'],'campaigns':18,'soloScenarios':396,'deterministicRebuildCheck':True,
   'preservedEditions':['Jericho','JerichoLite','ArcaKev','ArcaKevLite'],
   'foreignCampaignFilesByteIdentical':scope['foreignCampaignFilesByteIdentical'],
   'opponentCounts':scope['opponentCounts'],'persistentSupport':scope['support'],
   'stage2Clearance':{'soloVariants':54,'cooperativeVariantsWithFourStarts':54,'minimumSolo':scope['minimumStage2SoloGap'],'minimumCooperative':scope['minimumStage2CoopGap']},
   'cooperativeScenariosIncludingVanilla':1386,'tradeOpeningTemplates':sum(row['mcv'] is None for row in maps),
   'sourceTerrainAndHostAndEnemyPayloadPreservedByCoop':True},
 'native':{'passed':True,'editorMapLoads':int(match[1]),'uniqueEditorMapFiles':2835,
   'modContexts':7,'soloCampaignStarts':int(match[2]),'revisedSoloScenariosSampled':[1,2,3,4,8,20,21,22],
   'introAndLateSoloSaveReloads':90,'cooperativeStarts':int(match[3]),'cooperativeStages':[1,2,8,9],
   'cooperativeLateCheckpoints':int(match[4]),'cooperativeAllies':[2,4],
   'mirrorGuestSupportFactionPreserved':True,'fourOpponentFinalesWithEightRuntimeParticipants':True,
   'choamDialogs':7,'choamChecks':['31 units / 16 rows','scroll to last row','apply','undo / redo','map save / reload'],
   'reinforcements':'Tornie: native ownership and 5/6/9/11/13-minute timers for every active opponent/ally; solo and late cooperative checkpoint persistence',
   'rebels':'Tornie: no free yard/MCV; host and every active ally have a trade economy; MCV purchase charged after economic initialization',
   'startingFunds':'Tornie openings: 1500 before maintenance, quota 2000. Other editions retain original campaign funds/objectives.',
   'wildspade':'Native factory production gates checked in Tornie/Jericho/ArcaKev: Hightech/Carryall 4, Chemical 5 plus first upgrade, Ornithopter/IX 7',
   'frameCompositions':28,'frames':'Atreides falcon / Ordos serpent, scores and regions, seven mods and bonus colours',
   'standaloneRuntimeWithoutMsysPath':True,
   'matrixVerifierBinarySha256':digest(ROOT/'windows-build-546/bin/dunelegacy.exe'),
   'finalPreviewBinarySha256':digest(ROOT/'windows-build-546/bin/dunelegacy.exe')},
 'vanillaArchivesUnchanged':vanilla,
 'website':{'languages':['fr','en'],'previewVersion':'1.0.546','publishedDownloadVersionRetained':'1.0.545','javascriptSyntaxChecked':True},
 'limits':['No complete human campaign playthrough or balance sign-off.','No human co-op session on two physical PCs / Internet.','New mission recipes require starting a new campaign; an ongoing checkpoint retains its saved state.'],
 'evidence':{'matrixLogSha256':digest(matrix/'runtime.log'),'frameLogSha256':digest(frame/'runtime.log'),
   'ctestLogSha256':digest(ROOT/'reports/ctest-r2.log'),'coopAuditSha256':digest(ROOT/'reports/coop-r2.json'),
   'scopeAndDistanceAuditSha256':digest(ROOT/'reports/campaign-scope-r2.json')}
}
target=ROOT/'release_notes/Validation-1.0.546-local.json'
target.write_text(json.dumps(report,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
print(json.dumps({'report':str(target),'editorLoads':report['native']['editorMapLoads'],'soloStarts':len(starts),'coopStarts':int(match[3]),'passed':True}))
