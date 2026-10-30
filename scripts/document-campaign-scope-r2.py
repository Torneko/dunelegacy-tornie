from pathlib import Path
import json
import re
root=Path(__file__).resolve().parents[1]
policy=json.loads((root/'config/TornieCampaign546.json').read_text())
fr='''# Dune Legacy Tornie 1.0.546 r2 — préparation locale

La refonte des campagnes concerne uniquement **Tornie et Tornie Lite** : 18 campagnes, 396 scénarios solo et leurs variantes coop. Les campagnes, briefings et cartes coop de **Jericho, Jericho Lite, ArcaKev et ArcaKev Lite sont rétablis à l’identique de la référence 545**, en attendant l’avis de leurs auteurs. Aucune demande n’a été envoyée aux auteurs depuis cette préparation.

- Un deuxième adversaire de soutien est fixé pour toute une campagne. Il varie entre campagnes : Rebels affronte Harkonnen comme soutien, même quand son adversaire principal change.
- Missions 001–019 : deux adversaires. Missions 020–021 : trois. Finale 022 : quatre. Les nouveaux adversaires ont des bâtiments, des forces et des renforts adaptés.
- En 002–004, chaque bâtiment de soutien reste à au moins dix cases de l’emprise des bâtiments du joueur. Le contrôle inclut aussi les emprises des MCV de départ des partenaires coop, à deux ou quatre alliés.
- Le soutien conserve sa faction si un invité coop choisit cette même maison : les propriétaires et équipes restent distincts. Reprises, progression et participants supplémentaires suivent les quatre emplacements ennemis.
- Les intros Tornie gardent les deux petites bases ennemies, 1 500 crédits initiaux et un quota de 2 000 crédits raffinés. Les frais d’entretien commencent ensuite normalement.
- Rebels, uniquement dans Tornie : port stellaire, petite exploitation, aucun chantier/VCM gratuit ; le VCM s’achète au CHOAM. Les partenaires reçoivent la même économie.
- Wildspade, dans Tornie : Carryall et Ornithoptère prêtés dans l’intro, avec menace antiaérienne. La règle technologique demandée pour tous les mods qui proposent Wildspade est conservée : usine high-tech/Carryall au niveau 4, Carryall chimique au niveau 5 après la première amélioration, Ornithoptères/IX au niveau 7.
- Correctifs communs conservés : cadres Atreides/Ordos, CHOAM de l’éditeur, listes des campagnes Vanilla solo et coop. Les archives Vanilla sont inchangées.
- Réseau : **protocole 24**, même paquet 546 r2 et même mod sur tous les postes. Format de sauvegarde 9839 conservé. Commencer une nouvelle campagne pour utiliser les nouvelles missions ; un checkpoint existant garde son état enregistré.

Cette préparation est locale et non publiée. Les liens publics du guide restent ceux de la 545 publiée. L’équilibrage complet et la coop humaine sur deux PC restent à confirmer. Le rapport du paquet donne les contrôles effectivement terminés.

## Soutien permanent par campagne

| Édition | Campagne | Soutien |
|---|---|---|
'''
en='''# Dune Legacy Tornie 1.0.546 r2 — local preparation

Campaign changes apply only to **Tornie and Tornie Lite**: 18 campaigns, 396 solo scenarios and their cooperative variants. **Jericho, Jericho Lite, ArcaKev and ArcaKev Lite campaigns, briefings and co-op maps are restored byte-for-byte to the 545 baseline**, pending their authors’ feedback. No message has been sent to those authors from this preparation.

- Each campaign keeps one persistent supporting opponent. Support varies across campaigns; Rebels keeps Harkonnen support even when the primary opponent changes.
- Missions 001–019: two opponents. Missions 020–021: three. Final mission 022: four. Additional opponents have bases, forces and matching reinforcements.
- In 002–004, support building footprints remain at least ten tiles from player building footprints and from allied starting MCV footprints in two/four-ally co-op.
- The support faction stays fixed when a co-op guest chooses that same faction: ownership and teams remain distinct. Checkpoints, progression and optional allies retain the four enemy slots.
- Tornie openings retain two small enemy bases, 1,500 initial credits and a 2,000 refined-spice quota. Normal maintenance starts afterward.
- Rebels, in Tornie only: a starport and small economy, without a free construction yard/MCV; buy the MCV from CHOAM. Partners receive the same economy.
- Wildspade, in Tornie: loaned Carryall/Ornithopter and missile infantry threats in the opening. The previously requested shared Wildspade technology change remains wherever the faction is available: Hightech/Carryall 4, Chemical Carryall 5 plus upgrade one, Ornithopter/IX 7.
- Shared fixes remain: Atreides/Ordos frames, editor CHOAM and Vanilla solo/co-op editor sources. Vanilla archives are unchanged.
- Network **protocol 24** requires matching 546 r2 packages and mods. Save format 9839 remains. Start a new campaign for the revised missions; existing checkpoints retain their stored state.

This local preparation is unpublished. Public guide download links still point to released 545 packages. Full balance playthroughs and human two-PC co-op remain unverified. The delivered report lists the checks actually completed.

## Persistent support by campaign

| Edition | Campaign | Support |
|---|---|---|
'''
table=''.join(f'| {mod} | {house} | {support} |\n' for mod,rows in policy['support'].items() for house,support in rows.items())
for language,text in (('FR',fr),('EN',en)):
    (root/f'website/documents/Preparation-1.0.546-{language}.md').write_text(text+table,encoding='utf-8')
readme=root/'README.md';s=readme.read_text(encoding='utf-8')
s=re.sub(r'^\*\*1\.0\.546 — préparation locale.*$',
    '**1.0.546 r2 — préparation locale / local preview** : refonte de 18 campagnes Tornie/Tornie Lite seulement ; Jericho/ArcaKev et Lite rétablis. Soutien permanent, bases de soutien éloignées en 002–004, trois adversaires en 020–021, quatre en 022 ; coops adaptées. Correctifs CHOAM, cadres et éditeur Vanilla conservés. Protocole **24**, sauvegardes **9839**. [FR](website/documents/Preparation-1.0.546-FR.md) · [EN](website/documents/Preparation-1.0.546-EN.md). Préparation non publiée ; les liens historiques ci-dessous restent ceux de la 545.',s,flags=re.M)
readme.write_text(s,encoding='utf-8')
doc=root/'docs/WEBSITE.md';s=doc.read_text(encoding='utf-8')
if not s.startswith('# Préparation locale 1.0.546 r2'):
    s='# Préparation locale 1.0.546 r2\n\nCampagnes Tornie/Tornie Lite seulement ; autres auteurs préservés. Guide FR/EN aligné sur le protocole 24 et le soutien permanent. Paquet local non publié ; liens publics 545 conservés. Voir website/documents/Preparation-1.0.546-FR.md.\n\n'+s
doc.write_text(s,encoding='utf-8')
print('Campaign scope, support table and protocol 24 documented in FR/EN')
