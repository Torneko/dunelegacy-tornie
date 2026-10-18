# Unités personnalisées — 1.0.543 / Custom units

Cette spécification décrit le remplacement étendu. Les graphismes définitifs pourront remplacer les planches vanilla modifiables sans changer les règles ni les identifiants. / Final artwork may replace editable vanilla-derived sheets independently of rules and stable IDs.

## Extension du remplacement 1.0.543

- Vente activée par défaut dans **tous les mods hors Vanilla**. Remboursement de **50 %**, ou **75 % pour les maisons alliées du mode facile**, calculé sur le prix de construction après réduction ; aucun stockage nécessaire. Les règles enregistrées dans les sauvegardes restent conservées.
- Option « Béton requis » déplacée à gauche des options générales. Contrôle des alliés et construction sur leurs terres disponibles séparément dans les lobbies personnalisés, multijoueurs et coop.
- Flame Quad : nouvelle feuille complète **128×16**, huit cases de **16×16** alignées sur les huit repères du Quad vanilla, sans rééchantillonnage. Les PNG fournis restent intacts. Portée de l’infanterie à gaz : **8** (+3) ; vision **5**, comme le Deviator.
- Neutral : éolienne verticale **2×3**, hors Vanilla, avec les prérequis existants de cette éolienne.
- **Raider Quad** : Ordos/Sardaukar, usine légère niveau **4**, amélioration **1**, 300 crédits, vitesse +25 % par rapport au Quad.
- **Tank longue portée** : Atreides/Fremen, usine lourde niveau **5**, amélioration **2**, comme le Launcher ; 450 crédits, portée 7, vision 5.
- **Slowpost Neutral** et **Slow Tank Corruptique** : niveau **7 + IX**, respectivement chantier et usine lourde. Prix 450/750 crédits, portée 7. Le missile ralentit uniquement le déplacement, y compris les alliés touchés : vitesse −50 % pendant 4 s ; Rebels −75 % pendant 8 s ; Kleshmersh immunisé. Pas de dégâts directs.
- Moissonneuse **rapide** : Atreides/Kleshmersh, vitesse +25 %. Moissonneuse **grande capacité** : Wildspade/Mercenary, **1 050** au lieu de 700. Toutes deux à **350 crédits, usine lourde niveau 7 + IX**. Le **Harvestank passe également au niveau 7 + IX**. Compteur, plafonds, transport, déchargement, interface et sauvegardes reconnaissent les variantes.
- Palais Kleshmersh hors Vanilla : **missile au napalm**, délai Harkonnen **10 minutes**, dégâts directs réduits de moitié, traînée et zone d’impact incendiaires persistantes, conservées exactement après sauvegarde/reprise. Remplace l’ancien appel de Fremen.
- **Ver apprivoisé** : mode Chaos uniquement, tirage déterministe unique à la création de chaque Worfinery, **10 %** de chance de proposer sa production à **950 crédits**. Le tirage et la file sont conservés à la reprise. Ver contrôlable, limité au sable, n’attaque pas les alliés et ne disparaît pas après trois repas.
- **Ornithoptère élite Wildspade** : niveau **8**, **troisième amélioration de l’usine high-tech + IX**, 825 crédits, **32 PV et 56 dégâts** (environ +25 %, arrondis entiers), vitesse/portée normales.
- Planches de remplacement complètes issues des graphismes vanilla, modifiables dans `graphics/fallbacks-543/` et chargées depuis les fichiers de chaque mod. Transparence et couleurs de maison conservées, zooms entiers 1×/2×/3×. Étoiles bleues dans les boutons et sur le terrain de l’éditeur ; aucun ajout sur les portraits.
- Aide « How to Play » FR/EN actualisée pour les **sept modes**, la coop à quatre factions, l’archonte à huit, les épices et les règles d’alliance/vente. ArcaKev et Lite figurent dans les mentions des mods.
- Succès **Maître d’ArcaKev** (12 campagnes) et **Maître d’ArcaKev Lite** (6 campagnes) : **70 succès** au total. L’arsenal personnalisé compte les nouvelles unités dans les trois familles de mods. Les invités coop 3 et 4 peuvent aussi valider la campagne coop. Les succès précédemment obtenus restent acquis ; le 100 % suit le catalogue complet.
- Même numéro **1.0.543**, **protocole 18**, nouvelles sauvegardes **9836**. Tous les participants devront installer le même remplacement ; anciennes sauvegardes lisibles, unités/offres nouvelles non ajoutées rétroactivement.



## Expanded 1.0.543 replacement

- Selling defaults **on in every non-Vanilla mod**. Refund **50%**, or **75% for Easy Mode allied houses**, of the discounted construction price, without storage. Saved match rules remain preserved.
- Move Concrete Required to the left of general rules. Custom, multiplayer and co-op lobbies expose independent allied-control and allied-land-building rules.
- Flame Quad: complete **128×16** atlas, eight **16×16** cells aligned to the exact vanilla Quad direction anchors without resampling. Original supplied PNGs stay intact. Gas Infantry range **8** (+3), sight **5**, matching Deviator sight.
- Neutral gains the vertical **2×3 Windtrap** outside Vanilla, retaining its existing prerequisites.
- **Raider Quad**: Ordos/Sardaukar, Light Factory level **4**, upgrade **1**, 300 credits, +25% speed over the Quad.
- **Ranged Tank**: Atreides/Fremen, Heavy Factory level **5**, upgrade **2**, matching Launcher progression; 450 credits, range 7, sight 5.
- Neutral **Slowpost** and Corruptique **Slow Tank**: level **7 + IX**, Construction Yard / Heavy Factory, 450/750 credits, range 7. Missile slows movement only, including allies caught by it: −50% speed for 4 s, Rebels −75% for 8 s, Kleshmersh immune. No direct damage.
- **Fast Harvester**: Atreides/Kleshmersh, +25% speed. **Capacity Harvester**: Wildspade/Mercenary, **1,050** cargo instead of 700. Both cost **350**, Heavy Factory **level 7 + IX**. **Harvestank also moves to level 7 + IX**. HUD, limits, transport, unloading, interface and saved cargo recognize the variants.
- Non-Vanilla Kleshmersh Palace: **Napalm missile**, Harkonnen **10-minute** recharge, half direct damage with persistent fire along its path and impact area, preserved exactly across save/load; replaces the old Fremen call.
- **Tame Sandworm**: Chaos only. Each new Worfinery gets one deterministic draw, a **10%** chance of offering production at **950 credits**. Save/load retains the offer and queue. Controllable, sand-bound, does not attack allies or disappear after three meals.
- Wildspade **Elite Ornithopter**: **level 8**, Hightech Factory **upgrade 3 + IX**, 825 credits, **32 HP / 56 damage** (about +25%, rounded to integers), ordinary speed/range.
- Complete editable vanilla-derived fallback sheets in `graphics/fallbacks-543/`, loaded from each mod's files; transparency, house colours and exact 1×/2×/3× scales. Blue stars in editor buttons and on editor terrain only; no portrait overlays.
- Update French/English How to Play for all **seven modes**, four allied factions, eight Archon controllers, spice palettes, sale/alliance rules and ArcaKev mentions.
- **Master of ArcaKev** (12 campaigns), **Master of ArcaKev Lite** (6): **70 achievements** total. Custom arsenal includes new units across the three mod families. Co-op guests 3/4 can earn campaign completion too. Existing unlocks remain; 100% tracks the complete catalogue.
- Keep **1.0.543**, network protocol **18**, new saves **9836**. Every participant needs the same replacement; earlier saves remain readable, without retroactively adding new offers or units.



## Unités et graphismes déjà intégrés / Retained units and graphics



- Vente : explosions et son de destruction, puis béton appartenant au propriétaire sur toute l’emprise, sans texture de ruine. Bouton `$` après l’emplacement d’amélioration, de la taille de Réparer. Évacuation des unités contenues, remboursement séparé des productions non livrées, succès « Liquidités » et statistiques conservés.
- Troopers lance-flammes Kleshmersh par défaut ; technologies et prix suivent le donneur Chaos. Harkonnen conserve ses Troopers ordinaires et produit séparément la version lance-flammes au niveau 7 + IX. Cette infanterie ne compte pas comme véhicule Special.
- Kleshmersh : Rocket Trike niveau 3 / usine légère amélioration 1 ; Flame Quad niveau 5 / amélioration 2, sans Sonic Trike. Flame Quad à 300 crédits, deux tirs de 12 dégâts, précision normale à courte portée et effet de feu ; Quad normal à 200 crédits et deux tirs de 7 dégâts.
- Infanterie à gaz Ordos/Tharpique : WOR et Worfinery, IX, 250 crédits. Soigne les alliés (15 par missile), effet Deviator sur les ennemis ; portée 8, vision 5.
- Les sept PNG fournis restent intacts dans les six mods, dont les portraits lance-flammes individuels/3/5. Magenta retiré au chargement, noir opaque conservé, teintes par maison et zooms entiers. Aucune étoile ni chiffre 5 ajouté aux portraits.
- Neutral : blanc plus clair dans Tornie et ArcaKev ; Saboteur à la Worfinery, niveau 7 + IX, 500 crédits, campagne et multijoueur dans les mods complets. Les pouvoirs Saboteur des autres palais sont conservés.
- Jericho/Lite et ArcaKev/Lite : couleur bonus vert foncé identique aux Rebels de Tornie. Les couleurs par défaut des factions sont conservées.
- Compteur de moissonneuses local optionnel, désactivé par défaut dans Tornie/Lite ; inclut les nouvelles variantes.
- Linux : prise en charge XRandR et PulseAudio contrôlée dans les AppImages ; dimensions limitées au moniteur de la fenêtre, résolutions plus petites rétablies, taille choisie conservée et repli logiciel disponible.
