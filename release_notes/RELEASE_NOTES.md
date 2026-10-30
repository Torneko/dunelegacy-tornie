# Dune Legacy Tornie 1.0.546

Cette version corrige le CHOAM de l’éditeur et les cadres Atreides/Ordos, rend les campagnes Vanilla accessibles dans l’éditeur et révise les campagnes **Tornie et Tornie Lite uniquement**.

## Campagnes et coop

- 18 campagnes, 396 scénarios solo et leurs cartes coop adaptés aux bâtiments, unités et technologies de chaque faction, avec des renforts des deux côtés.
- Un adversaire de soutien permanent par campagne, différent d’une campagne à l’autre. Rebels conserve Harkonnen comme soutien, même lorsque l’adversaire principal change.
- Missions 001–019 : deux adversaires. Missions 020–021 : trois. Finale 022 : quatre, avec leurs bases, forces et renforts.
- En 002–004, les bâtiments du soutien se trouvent à au moins dix cases de l’emprise des bâtiments du joueur et des MCV de départ alliés. Les variantes coop à deux ou quatre factions respectent la même règle.
- Intros Tornie : deux petites bases ennemies, 1 500 crédits au départ et objectif de 2 000 crédits raffinés.
- Intro Rebels de Tornie : port stellaire et petite économie, sans chantier ni MCV gratuit. Acheter son MCV au CHOAM ; chaque partenaire coop reçoit une économie équivalente.
- Intro Wildspade de Tornie : Carryall et Ornithoptère prêtés, avec une menace antiaérienne.
- Les invités peuvent choisir la même faction que le soutien ennemi sans remplacer ce dernier. La progression et les sauvegardes coop conservent les participants supplémentaires.

Les campagnes, briefings et cartes coop **Jericho, Jericho Lite, ArcaKev et ArcaKev Lite restent ceux de la référence 1.0.545**. Leur refonte attend l’avis de leurs auteurs. Les archives des campagnes Vanilla sont inchangées.

## Éditeur et technologie

- Correction du crash à l’ouverture du CHOAM : les 31 unités du catalogue tiennent dans une liste défilante, avec application, annulation et rechargement de carte.
- Listes distinctes pour les campagnes solo et coop du mod actif et de Vanilla. Les cartes des campagnes peuvent être ouvertes dans l’éditeur ; les factions chargées restent sélectionnables, y compris dans une édition Lite.
- Cadres dorés Atreides et Ordos réattribués à la bonne maison dans les scores et sur la carte des régions.
- Wildspade, dans tous les mods qui proposent cette faction : usine high-tech et Carryall au niveau 4 ; Carryall chimique au niveau 5 après la première amélioration. Ornithoptères et IX restent au niveau 7.

## Installation et compatibilité

Extraire entièrement le paquet Windows dans un nouveau dossier. Pour le paquet portable, lancer `Lancer-1.0.546.cmd` ; ses préférences et sauvegardes sont dans `profile/`. Le guide français/anglais s’ouvre avec `Ouvrir-le-guide.cmd`.

Commencer une **nouvelle campagne** pour jouer les missions révisées. Une sauvegarde en cours conserve l’état de sa mission.

Réseau : **protocole 24**, version 1.0.546 et même mod sur tous les postes. Les préversions en protocole 23 et la 1.0.545 ne peuvent pas rejoindre cette version. Format de sauvegarde **9839** conservé ; les anciens lecteurs de sauvegardes restent présents.

## Validation

La validation Windows comprend 146 cas / 11 217 assertions moteur et réseau, 5 265 chargements de cartes dans l’éditeur, 252 démarrages solo, 224 départs coop, 76 reprises coop avancées et 28 compositions de cadres. Les 2 320 fichiers de campagnes, briefings et coops des autres auteurs sont identiques à la référence 545. Les empreintes du paquet et les sources accompagnent la livraison.

Ces contrôles automatisés ne constituent pas une campagne complète jouée par une personne ni une partie coop humaine sur deux PC. Le rapport de livraison distingue les plateformes effectivement vérifiées.

---

# Dune Legacy Tornie 1.0.546

This version fixes editor CHOAM and the Atreides/Ordos frames, exposes Vanilla campaigns in the editor and revises **Tornie and Tornie Lite campaigns only**.

## Campaigns and co-op

- 18 campaigns, 396 solo scenarios and matching cooperative maps use faction buildings, units and technology, with reinforcements for both sides.
- Each campaign keeps one supporting enemy throughout its missions. Support varies between campaigns; Rebels keeps Harkonnen even when the primary enemy changes.
- Missions 001–019 have two opponents, 020–021 have three and final mission 022 has four, with bases, forces and reinforcements.
- In 002–004, support buildings stay at least ten tiles from player building footprints and allied starting MCV footprints. Two/four-faction co-op follows the same rule.
- Tornie openings have two small enemy bases, 1,500 starting credits and a 2,000 refined-spice quota.
- Tornie Rebels opening: a starport and small economy, without a free construction yard or MCV. Buy the MCV through CHOAM; each co-op partner receives a matching economy.
- Tornie Wildspade opening: loaned Carryall and Ornithopter, with an anti-air threat.
- A guest choosing the enemy support faction no longer replaces that support. Progression and cooperative checkpoints retain additional participants.

**Jericho, Jericho Lite, ArcaKev and ArcaKev Lite campaigns, briefings and co-op maps retain their 1.0.545 baseline**, pending their authors’ feedback. Vanilla campaign archives remain unchanged.

## Editor and technology

- Fixed the CHOAM opening crash: all 31 catalogue units fit in a scrolling list, supporting apply, undo and map reload.
- Separate editor sources for the active mod’s solo/co-op campaigns and Vanilla campaigns. Loaded campaign factions remain selectable, including in Lite editions.
- Golden Atreides and Ordos frames now match their faction on score and region screens.
- Wildspade, wherever the faction is available: Hightech Factory and Carryall at technology 4; Chemical Carryall at 5 after the first upgrade. Ornithopter and IX remain at technology 7.

## Installation and compatibility

Fully extract the Windows package into a new folder. For the portable package, run `Lancer-1.0.546.cmd`; preferences and saves use `profile/`. Open the bilingual guide with `Ouvrir-le-guide.cmd`.

Start a **new campaign** to use revised missions. Existing checkpoints retain their saved mission state.

Networking requires **protocol 24**, version 1.0.546 and matching mods on every computer. Protocol 23 previews and 1.0.545 cannot join this version. Save format **9839** remains; earlier save readers are retained.

## Validation

Windows validation covers 146 cases / 11,217 engine and network assertions, 5,265 editor map loads, 252 solo starts, 224 cooperative starts, 76 late cooperative checkpoint restores and 28 frame compositions. All 2,320 campaign, briefing and co-op files from other authors match the 545 baseline. Package hashes and corresponding sources accompany the delivery.

Automated checks do not represent a complete human campaign playthrough or human two-PC co-op. The delivery report identifies the platforms actually verified.
