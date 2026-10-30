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
