# Campaigns / Campagnes — 1.0.541

Six mod plans, stable runtime faction IDs. / Six plans de mods, identifiants de factions stables.

ArcaKev order / ordre : Harkonnen, Atreides, Ordos, Fremen, Sardaukar, Mercenary, Kleshmersh, Wildspade, Rebels, Neutral, Tharpique, Corruptique. Lite editions offer the first six.

## Tornie

| Campaign / Campagne | First / Premier | Second / Deuxième | Final |
|---|---|---|---|
| Harkonnen | Atreides | Sardaukar | Rebels |
| Atreides | Ordos | Mercenary | Corruptique |
| Ordos | Fremen | Neutral | Wildspade |
| Fremen | Sardaukar | Rebels | Kleshmersh |
| Sardaukar | Mercenary | Corruptique | Tharpique |
| Mercenary | Neutral | Wildspade | Harkonnen |
| Neutral | Rebels | Kleshmersh | Atreides |
| Rebels | Corruptique | Tharpique | Ordos |
| Corruptique | Wildspade | Harkonnen | Fremen |
| Wildspade | Kleshmersh | Atreides | Sardaukar |
| Kleshmersh | Tharpique | Ordos | Mercenary |
| Tharpique | Harkonnen | Fremen | Neutral |

## Jericho

| Campaign / Campagne | First / Premier | Second / Deuxième | Final |
|---|---|---|---|
| Harkonnen | Ordos | Mercenary | Wildspade |
| Atreides | Fremen | Neutral | Kleshmersh |
| Ordos | Sardaukar | Rebels | Tharpique |
| Fremen | Mercenary | Corruptique | Harkonnen |
| Sardaukar | Neutral | Wildspade | Atreides |
| Mercenary | Rebels | Kleshmersh | Ordos |
| Neutral | Corruptique | Tharpique | Fremen |
| Rebels | Wildspade | Harkonnen | Sardaukar |
| Corruptique | Kleshmersh | Atreides | Mercenary |
| Wildspade | Tharpique | Ordos | Neutral |
| Kleshmersh | Harkonnen | Fremen | Rebels |
| Tharpique | Atreides | Sardaukar | Corruptique |

## TornieLite

| Campaign / Campagne | First / Premier | Second / Deuxième | Final |
|---|---|---|---|
| Harkonnen | Atreides | Ordos | Fremen |
| Atreides | Ordos | Fremen | Sardaukar |
| Ordos | Fremen | Sardaukar | Mercenary |
| Fremen | Sardaukar | Mercenary | Harkonnen |
| Sardaukar | Mercenary | Harkonnen | Atreides |
| Mercenary | Harkonnen | Atreides | Ordos |

## JerichoLite

| Campaign / Campagne | First / Premier | Second / Deuxième | Final |
|---|---|---|---|
| Harkonnen | Sardaukar | Mercenary | Atreides |
| Atreides | Mercenary | Harkonnen | Ordos |
| Ordos | Harkonnen | Atreides | Fremen |
| Fremen | Atreides | Ordos | Sardaukar |
| Sardaukar | Ordos | Fremen | Mercenary |
| Mercenary | Fremen | Sardaukar | Harkonnen |

## ArcaKev

| Campaign / Campagne | First / Premier | Second / Deuxième | Final |
|---|---|---|---|
| Harkonnen | Fremen | Neutral | Kleshmersh |
| Atreides | Sardaukar | Rebels | Tharpique |
| Ordos | Mercenary | Corruptique | Harkonnen |
| Fremen | Neutral | Wildspade | Atreides |
| Sardaukar | Rebels | Kleshmersh | Ordos |
| Mercenary | Corruptique | Tharpique | Fremen |
| Kleshmersh | Atreides | Sardaukar | Corruptique |
| Wildspade | Harkonnen | Fremen | Rebels |
| Rebels | Kleshmersh | Atreides | Mercenary |
| Neutral | Wildspade | Harkonnen | Sardaukar |
| Tharpique | Ordos | Mercenary | Wildspade |
| Corruptique | Tharpique | Ordos | Neutral |

## ArcaKevLite

| Campaign / Campagne | First / Premier | Second / Deuxième | Final |
|---|---|---|---|
| Harkonnen | Ordos | Fremen | Sardaukar |
| Atreides | Fremen | Sardaukar | Mercenary |
| Ordos | Sardaukar | Mercenary | Harkonnen |
| Fremen | Mercenary | Harkonnen | Atreides |
| Sardaukar | Harkonnen | Atreides | Ordos |
| Mercenary | Atreides | Ordos | Fremen |

## Consistency / Cohérence

Each house appears once in each enemy role. ArcaKev's three enemies are disjoint from both full Tornie and Jericho for the same player. Lite has only six houses; its distinct plan changes enemy roles, while completely disjoint three-enemy sets across three Lite mods are mathematically impossible. Region ownership and briefing names follow the same plan.

Chaque maison apparaît une fois dans chaque rôle adverse. Les trois ennemis d’une campagne ArcaKev sont différents de ceux de Tornie et Jericho complets. Les éditions Lite n’ont que six maisons : leur plan change les rôles ; trois ensembles disjoints de trois ennemis sont impossibles. Régions et noms des briefings suivent le même plan.

Solo campaign terrain, economy and forces remain unchanged by 1.0.541. Scenario 22 now actually unlocks engine technology 9 in campaign/skirmish. Co-op draws existing scenarios per new stage; see the bilingual co-op guides. Saved missions retain their stored maps.

Terrains, économie et forces solo sont conservés. La finale 22 débloque le niveau technologique moteur 9 en campagne/skirmish. La coop choisit les scénarios existants à chaque nouvelle étape ; voir les guides bilingues. Les missions sauvegardées conservent leurs cartes.

Source: `config/CampaignPlans.json`; runtime briefings: `mods/<mod>/data/CampaignPlan.ini`. Validate with `scripts/check-campaign-regions.py`, `scripts/check-bundled-mods.py` and `scripts/build-coop-campaign.py --check`. `scripts/build-arcakev.py --write` bootstraps only ArcaKev's solo plans from Jericho; it does not replace source mod campaigns.
