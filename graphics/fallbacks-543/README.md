# Editable vanilla-derived fallback sheets / Planches modifiables

Ces PNG sont des planches complètes issues des graphismes vanilla déjà fournis avec le jeu. Conserver l’attribution d’origine de Dune II / Dune Legacy et celle du projet. Les PNG originaux envoyés par Tornie restent intacts dans les six mods ; `FlameQuadAtlas.png` est une copie dérivée alignée.

| Fichier | Cases (colonnes × rangées) | Taille d’une case | Usage |
| --- | --- | --- | --- |
| Quad-Vanilla.png | 8 × 1 | 16 × 16 | Référence exacte des huit repères |
| FlameQuadAtlas.png | 8 × 1 | 16 × 16 | Quad lance-flammes aligné |
| RaiderQuad.png | 8 × 1 | 16 × 16 | Quad rapide |
| RangedTank-Base.png / RangedTank-Turret.png | 8 × 1 | 16 × 16 | Tank longue portée, pièces superposées sans décalage |
| SlowTank-Base.png / SlowTank-Turret.png | 8 × 1 | 16 × 16 | Tank ralentisseur |
| Harvester-Capacity.png / Harvester-Fast.png | 8 × 1 | 24 × 24 | Moissonneuses |
| EliteOrnithopter.png | 8 × 3 | 24 × 24 | Toutes les orientations et les trois animations |
| TameSandworm.png | 1 × 9 | 24 × 24 | Neuf étapes de l’animation du ver |
| Slowpost.png | 10 × 1 | 16 × 16 | Deux étapes de chantier, huit orientations |
| NapalmRocket.png / SlowRocket.png | 16 × 1 | Voir dimensions du PNG | Toutes les orientations des missiles |

Pour les véhicules et les avions : **E, NE, N, NW, W, SW, S, SE** de gauche à droite. Conserver la taille de la feuille et l’ordre des cases. Les fichiers base/tourelle des tanks partagent exactement le même centre ; ne pas les recadrer indépendamment. Les repères du Quad sont documentés dans `Quad-Alignment.json`.

Copier le PNG modifié portant le même nom dans `mods/<mod>/data/` : le moteur le charge pour cette variante, applique la couleur de maison et construit les zooms 1×/2×/3× par multiples entiers. Utiliser un fond transparent ; le magenta pur (255,0,255) est aussi retiré au chargement. Les nuances rouges de la palette Harkonnen sont les pixels d’équipe : les conserver pour obtenir la teinte de chaque maison. Les étoiles bleues sont dessinées uniquement par l’éditeur ; ne pas les peindre sur les planches ou portraits. Un fichier manquant utilise automatiquement son graphisme vanilla.

These complete sheets use the vanilla graphics already bundled with the project. Preserve original Dune II / Dune Legacy attribution. Edit PNGs at their native size, retaining frame order, dimensions, alpha and red team-colour shades. Copy the edited file under the same name to the selected mod's `data` folder. The engine applies house colours and exact integer zooms. Tank base and turret sheets share a centre and must not be cropped separately. Pure magenta is removed on load; opaque black remains. Editor stars are rendered by the editor only. Missing files fall back to their vanilla atlas.
