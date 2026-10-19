# Brouillon archivé — devenu 1.0.544 / Archived draft

Les portraits originaux ont été reçus et la publication 1.0.544 est autorisée. Les mentions d’attente ci-dessous décrivent les étapes préparatoires précédentes ; voir [la spécification finale](RELEASE-1.0.544.md). / Original portraits are now received and publication is authorized; the earlier pending notes below are historical.

# Prochaine release — brouillon / Next release draft

Liste ouverte. Attendre les autres précisions et l’instruction explicite de publication. Aucun numéro choisi ; la 1.0.543 publiée reste inchangée.

## Français

- Graphismes reçus pour Slowpost, Raider Quad, moissonneuse rapide et moissonneuse grande capacité : huit PNG originaux conservés dans `graphics/next-release/originals/`. Adaptation des planches et portraits à finaliser.
- **Kleshmersh : retirer le Harvestank de sa production normale** dans Tornie, Jericho et ArcaKev. Conserver la moissonneuse standard et la rapide. La rapide reste au niveau 7 + IX. Les autres factions et la technologie empruntée en Chaos conservent leurs règles. Les unités déjà présentes dans les sauvegardes restent présentes.
- **Slow Tank : tourelle blanc clair**, palette identique au blanc Neutral de Tornie/ArcaKev. **Tank longue portée : tourelle violette**. Les châssis gardent la couleur de leur propriétaire ; teintes fixes également dans l’éditeur et aux zooms 1×/2×/3×. Les fichiers PNG originaux restent intacts.
- Les couleurs fixes s’appliquent aussi au repli vanilla si une planche manque ; désactiver les graphismes personnalisés conserve le repli standard.
- Aperçus des deux icônes reçus : violette pour le tank longue portée, blanche pour Slow Tank. Les fichiers PNG originaux restent à recevoir (les aperçus du message ne sont pas montés dans les pièces jointes). Attendre ensuite la confirmation de publication.
- **Moissonneuses en récolte + Area Guard** : corriger la fausse détection de blocage lorsqu’un prochain pas ou une recherche de chemin est encore en attente. La moissonneuse standard, la rapide et la grande capacité peuvent écraser un fantassin ennemi sur une tuile d’épice et reprendre la récolte. Le Harvestank garde son tir normal ; le même correctif de navigation s’applique à sa récolte et aux retours en raffinerie. Les règles d’écrasement allié et le relief restent inchangés.
- **Scoutpost** : boutons d’amélioration nommés uniquement « Flamepost » / « Chemipost ». Le prix passe dans l’infobulle, avec les explications, pour éviter le débordement du cadre.
- **Vente des bâtiments améliorés** : appliquer 50 % (75 % pour les alliés en facile) au prix du bâtiment et au coût cumulé des améliorations terminées. Inclure les niveaux des usines, les 150 crédits de la Windtrap Rebels et le Scoutpost initial avec le coût de sa conversion en Flamepost/Chemipost. La réduction d’achat du mode facile s’applique à la construction, pas une deuxième fois aux améliorations. L’avancement payé d’une amélioration en cours est annulé et remboursé séparément, sans double compte. L’infobulle et les statistiques utilisent ce total.
- **Slowpost dans l’éditeur** : bouton dans la case libre directement sous Flamepost ; aperçu d’une tuile 16×16, comme Scoutpost/Flamepost/Chemipost, avec étoile bleue conservée. Ne plus utiliser une toile 32×32 qui agrandit le bouton.
- **Napalm du palais** : supprimer les incendies déposés pendant le vol depuis le palais. Conserver les neuf feux persistants de la zone d’impact 3×3 et les dégâts directs réduits.
- **Palais aléatoires** : ajouter le missile au napalm au tirage étendu de Tornie/Jericho/ArcaKev, avec ciblage, recharge habituelle du palais aléatoire et pouvoir conservé en sauvegarde. Les identifiants des cinq pouvoirs existants restent stables ; les pouvoirs vanilla restent classiques.
- Préparer les pages FR/EN correspondantes sans déploiement. Vérification complète du moteur, des graphismes et de la documentation à terminer quand la liste sera stabilisée.

## English

- Receive eight original artwork files for Slowpost, Raider Quad, Fast Harvester and Capacity Harvester. Preserve originals; atlas and portrait adaptations remain pending.
- Remove Harvestank from Kleshmersh’s ordinary production in Tornie/Jericho/ArcaKev. Keep normal and Fast Harvesters; Fast stays at level 7 + IX. Other factions, Chaos donor technology and units already present in saves retain their rules.
- **Slow Tank: light white turret**, matching the bright Neutral ramp. **Ranged Tank: violet turret**. Chassis retain owner colours, including editor previews and all zoom levels; supplied PNGs remain unchanged.
- Fixed turret colours also apply when a sheet is missing; disabling custom graphics preserves the ordinary vanilla fallback.
- Two portrait previews received: violet for Ranged Tank, white for Slow Tank. Original PNG files remain pending; inline previews are unavailable as workspace files. Continue awaiting explicit publication confirmation.
- **Harvesting in Area Guard**: do not treat an empty remaining path as a blockage while the next step or a path request is pending. Normal/Fast/Capacity Harvesters can crush an enemy infantry blocker on spice and resume collection. Armed Harvestank retains its weapon; its harvesting and refinery-return navigation receives the same fix. Allied crush and terrain rules remain unchanged.
- **Scoutpost**: upgrade buttons show only “Flamepost” / “Chemipost”; cost and explanations remain in tooltips.
- **Upgraded-building sales**: apply 50% (75% for Easy allies) to construction plus completed upgrades. Include factory levels, the Rebels Windtrap's 150-credit upgrade and original Scoutpost cost plus Flamepost/Chemipost conversion cost. Apply the Easy purchase discount to construction only. Paid progress of an unfinished upgrade is cancelled and refunded separately, without double counting. Tooltips and statistics use the resulting refund.
- **Editor Slowpost**: keep the free cell directly below Flamepost and a native 16×16 preview, matching the other three posts. Retain the blue star without a 32×32 canvas enlarging its button.
- **Palace napalm**: flight no longer ignites a line from the Palace. Retain the persistent 3×3 impact fire and reduced direct damage.
- **Random Palaces**: add napalm to the expanded Tornie/Jericho/ArcaKev power pool, with targeting, the normal random-Palace cooldown and saved selection. Existing power IDs remain stable; Vanilla keeps its classic powers.
- Prepare matching FR/EN documentation without deploying. Complete engine, graphics and documentation checks once the request list is settled.

## Vérifications du brouillon / Draft checks

Compilation moteur/tests et **CTest 2/2** réussis. Contrôles natifs : sept modes de règles/production et six modes de graphismes, y compris pixels blancs/violets des tourelles pour trois couleurs de maison ; rendu éditeur aux trois zooms. Les configurations et leurs manifestes de mods sont cohérents. Ces résultats concernent le retrait du Harvestank et les teintes préparées ; l’intégration des huit nouveaux PNG et des deux portraits de tanks reste à finaliser. Aucune CI de publication lancée, aucun tag créé ou déplacé.

Engine/test build and CTest 2/2 pass. Native checks pass in seven rule modes and six graphics modes, including fixed turret pixels across three house colours and editor rendering at all zooms. Artwork integration and final publication checks remain pending the complete user list and original portrait files. No release CI or tag operation was performed.

La régression de navigation est reproduite avant correctif sur un champ d’épice, puis vérifiée après correctif : **25 scénarios dans les sept modes** (moissonneuse standard en Vanilla ; standard, rapide, grande capacité et Harvestank dans les six mods). Les trois moissonneuses non armées écrasent le fantassin puis continuent la récolte ; le Harvestank conserve son tir. Compilation et CTest **2/2** réussis après ce correctif et les libellés Scoutpost.

The navigation regression is reproduced before the fix on a spice field, then passes **25 native scenarios across all seven modes**. Unarmed Harvesters crush the infantry blocker and resume collecting; Harvestank retains its normal weapon. Engine/test build and CTest **2/2** pass after the navigation and Scoutpost label changes.

Avant publication : choisir la version et revoir le protocole réseau et la garde de compatibilité des sauvegardes pour les nouvelles règles et le sixième pouvoir aléatoire, puis exécuter la CI sur la source figée. La régression de moissonneuse fait partie des vérifications natives installées de Linux/Windows préparées ; aucune de ces CI n’a été déclenchée.


Dernière passe native du brouillon : **52 cas de vente dans les sept modes** (normal/facile coop), coûts réellement débités pour les améliorations, niveaux cumulés, annulation en cours, bâtiment endommagé, sauvegarde/reprise et statistiques. **Six palettes d’éditeur** vérifient Slowpost sous Flamepost et les quatre aperçus 16×16. Le napalm vérifie le tirage complet des six pouvoirs dans les trois mods complets, l’exclusion de Vanilla, le ciblage sérialisé, la sauvegarde du pouvoir choisi, l’absence de feu pendant le vol et les neuf feux à l’impact. Les contrôles existants de vente, alliances, unités, graphismes et incendies persistants passent également. Compilation et **CTest 2/2** réussis. Documentation FR/EN et syntaxe des futures CI vérifiées ; aucune publication ni CI distante lancée.

Final native draft pass: **52 sale cases across seven modes**, covering normal/Easy co-op, actual upgrade charges, cumulative levels, unfinished upgrades, damage, reloads and statistics. **Six editor palettes** verify Slowpost below Flamepost and four native 16×16 post previews. Napalm checks cover the full six-power pool in the three full mods, Vanilla exclusion, serialized targeting, saved selection, no ground fire in flight, nine impact fires and persistent damage after reload. Existing sale/alliance/unit/graphics/expansion checks pass; build and **CTest 2/2** pass. Bilingual draft documentation and future CI syntax are checked without remote execution or publication.
