# Unités personnalisées — 1.0.543 / Custom units

Spécifications des unités de la 1.0.543. Les remplacements de cette version suivent les demandes de Tornie ; les reçus de publication sont conservés dans WEBSITE.md.

## Français

- Les Troopers lance-flammes sont les Troopers par défaut de Kleshmersh hors Vanilla. En Chaos, leur effet doit suivre la technologie de production tirée au hasard, même pour une autre maison, ainsi que leur prix et leur icône dédiée.
- Harkonnen : conserver les Troopers ordinaires et ajouter une production distincte de Troopers lance-flammes à partir du niveau 7, avec une IX comme prérequis. Extension hors Vanilla, dans les versions complètes et Lite où Harkonnen est disponible. Cette unité séparée ne doit pas remplacer les Troopers ordinaires ni imposer le niveau 7 aux Troopers de Kleshmersh.
- Kleshmersh : Rocket Trike au niveau 3, amélioration 1 de l'usine légère ; Flame Quad au niveau 5, amélioration 2 de l'usine légère ; pas de Sonic Trike dans sa production normale.
- Infanterie à gaz : Ordos et Tharpique ; production dans la WOR et la Worfinery, débloquée par l'IX. Le gaz soigne les alliés et produit l'effet du Deviator sur les ennemis. Hors Vanilla ; Ordos également dans les versions Lite, Tharpique uniquement dans les mods où cette maison existe.
- Fichiers annoncés : `FlameQuadIcon.png`, `FlameQuadTiles.png`, `Gaz_Infantry_sprites.png`, `ProductionTrooperFlames.png`, `ProductionTrooperGazInfantry.png`.
- Les cinq PNG ont été reçus dans `Images.tar.gz` et conservés intacts dans les six mods, avec manifestes SHA-256. Portraits : 91×55 ; Quad : 88×58 ; gaz : 101×42. Le moteur utilise les cinq vues du Quad et les neuf poses individuelles de la première rangée à gaz, puis reconstruit les orientations comme les unités de base. Le fond magenta opaque du gaz devient transparent au chargement ; le noir opaque est conservé. Les trois zooms sont des multiples exacts de la même base.
- Équilibrage initial, modifiable par ObjectData : Flame Quad 300 crédits, 130 PV, deux tirs de dégâts 12, portée 4 ; infanterie à gaz 250 crédits, 45 PV, portée 5, gaz ennemi identique au Deviator, soin 15 par missile ; Trooper lance-flammes Harkonnen 200 crédits et statistiques du Trooper Kleshmersh, avec son bonus de dégâts de feu existant.

## English

Unit specifications for 1.0.543; publication receipts are recorded in WEBSITE.md.

- Outside Vanilla, Kleshmersh's default Troopers use flamethrowers. In Chaos mode, the weapon, price and dedicated production portrait must follow the randomly inherited production technology rather than the receiving faction.
- Harkonnen keeps ordinary Troopers and gains a separate flamethrower Trooper product at level 7, requiring an IX. Apply outside Vanilla, including Lite rosters containing Harkonnen. Do not replace ordinary Troopers or impose this unlock on Kleshmersh's default Troopers.
- Kleshmersh: Rocket Trike at level 3 / Light Factory upgrade 1; Flame Quad at level 5 / upgrade 2; remove Sonic Trike from its normal production.
- Gas Infantry: Ordos and Tharpique, produced by WOR and Worfinery, requiring an IX. Heal allies and apply the Deviator effect to enemies. Outside Vanilla; Ordos is included in Lite, Tharpique only where that faction exists.
- All five original PNG files were received in `Images.tar.gz`, preserved unchanged in the six mods and added to SHA-256 manifests. Portraits are 91×55, Quad 88×58, gas 101×42. Five Quad views and the first nine individual gas poses are normalized into standard orientation/animation atlases in memory. Magenta becomes transparent, opaque black remains visible, and all three zooms use exact integer scales.
- Initial balance is configurable in ObjectData: Flame Quad 300 credits / 130 HP / two 12-damage shots / range 4; Gas Infantry 250 credits / 45 HP / range 5 / Deviator enemy gas / 15 healing per missile; Harkonnen flame Trooper 200 credits using the Kleshmersh Trooper values and existing flame damage bonus.

## Remplacement 1.0.543 / Replacement 1.0.543

Neutral : Saboteur produit à la Worfinery, niveau 7, IX, 500 crédits, campagne et multijoueur, uniquement dans les mods complets Tornie/Jericho/ArcaKev. Les autres factions gardent leurs pouvoirs de palais ; elles ne produisent pas ce Saboteur. Neutral est plus blanc dans Tornie et ArcaKev.

Neutral Saboteur production: Worfinery, level 7, IX, 500 credits, campaign and multiplayer, full Tornie/Jericho/ArcaKev only. Palace Saboteurs remain unchanged. Brighter Neutral white applies to Tornie and ArcaKev.

Le Flame Quad est centré aux trois zooms. Le masque VGA des couleurs de maison est reconnu pour le Quad et l’infanterie à gaz ; le vert du gaz et les détails bruns restent fixes. Les trois unités ont des étoiles bleues dans l’éditeur ; aucune étoile n’est ajoutée sur les portraits. Les marqueurs apparaissent aussi sur les unités placées sur le terrain de l’éditeur aux trois zooms. L’infanterie est exclue des véhicules Special. PNG originaux conservés.

Flame Quad centring and VGA team-colour masks are corrected. Green gas and brown chassis details are preserved. Blue stars identify the three editor units; no blue stars are added to portraits. Placed editor units also show their markers at all three zooms. Infantry is excluded from Special vehicle pools. Original PNGs are unchanged.

Reportés / Deferred: moissonneuses grande capacité et rapide, Raider Quad, tank longue portée, missile au napalm, Slowpost, Slow Tank et effets de ralentissement (déplacement uniquement ; Kleshmersh immunisé, Rebels plus sensible), ver apprivoisé. Attendre les graphismes et préciser factions, déblocages et valeurs avant intégration.

Les icônes originales `ProductionTroopersFlames3.png` et `ProductionTroopersFlames5.png` de `troopers_flames.tar.gz` sont conservées sans modification dans les six mods. Les commandes de Kleshmersh (et la technologie empruntée en Chaos) utilisent les portraits individuels/3/5 adaptés, sans chiffre ni étoile superposés. Le Flame Quad utilise une trajectoire précise à courte portée et son centre natif n’est plus biaisé à gauche sur les vues de largeur impaire.

Original three/five flame-Trooper production icons are preserved unchanged in all six mods. Kleshmersh production and inherited Chaos technology select the correct single/three/five portrait without number or star overlays. Flame Quad uses accurate close-range trajectories and odd-width views no longer have a leftward pixel bias.
