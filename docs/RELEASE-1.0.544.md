# 1.0.544 — graphismes et compatibilité / artwork and compatibility

Les dix originaux reçus sont conservés dans `graphics/next-release/originals/` et copiés exactement dans les six mods. Aucun portrait fourni n’est recoloré ou marqué. Les adaptations de planche, les teintes de maison et le redimensionnement sont faits en mémoire. / All ten originals are preserved and copied exactly into the six mods. Runtime layout conversion, house tint and portrait fitting do not rewrite source PNGs.

| Original | Fichier chargé dans le mod / loaded file |
| --- | --- |
| `RaiderQuadTiles.png` | `data/RaiderQuad.png` |
| `Slowpost.png` | `data/Slowpost.png` |
| `HarvesterSpeed.png` | `data/Harvester-Fast.png` |
| `HarvesterCapacity.png` | `data/Harvester-Capacity.png` |
| `RaiderQuadIcon.png` | `data/RaiderQuadIcon.png` |
| `SlowpostIcon.png` | `data/SlowpostIcon.png` |
| `SpeedHarvesterIcon.png` | `data/SpeedHarvesterIcon.png` |
| `CapacityHarvesterIcon.png` | `data/CapacityHarvesterIcon.png` |
| `SlowTank.png` | `data/SlowTankIcon.png` |
| `RangedCombatTankIcon.png` | `data/RangedCombatTankIcon.png` |

Raider Quad: cinq vues 88×58, découpées aux origines x=0/18/36/53/71 et y=21. Ordre des huit vues : 2/1/0/1/2/3/4/3 ; vues 3/4/5 miroir horizontal. Les limites visibles sont centrées dans les cases 16×16, sans décalage aux zooms. Slowpost: deux vues 16×16 verticales ; construction vanilla conservée, vues actives répétées dans les huit orientations. Les atlas natifs déjà modifiés à la main restent acceptés. / Existing native-sized custom atlases remain supported.

Les anciens identifiants d’unités et les cinq premiers pouvoirs de palais restent stables. Le napalm aléatoire est ajouté en sixième position. Protocole 19, nouvelles sauvegardes 9837 ; lecture des anciens formats conservée. / Stable unit IDs and earlier random powers are retained; the sixth saved power requires the new compatibility gate.

Les conditions de production, prix et capacités des nouvelles unités de la 1.0.543 sont conservés, hormis le retrait normal du Harvestank Kleshmersh. Voir [les spécifications 1.0.543](UNITS-1.0.543.md), [les notes FR](../release_notes/1.0.544-FR.md) et [EN](../release_notes/1.0.544-EN.md).

## Correctifs en préparation / pending fixes

Voir [FR](../release_notes/Correctifs-1.0.544-FR.md) et [EN](../release_notes/Correctifs-1.0.544-EN.md). Les deux vues actives du Slowpost sont animées, indépendamment de la visée. Les moissonneuses sont désormais recolorées depuis la planche vanilla en deux moitiés complémentaires ; les montages fournis ne définissent plus leurs pixels de rendu. / Slowpost states are animated independently of aim. Both specialist Harvesters are recoloured from the vanilla atlas into complementary halves; supplied montages no longer define their rendered pixels. Original PNGs remain unchanged.
