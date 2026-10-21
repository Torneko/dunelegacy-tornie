# 1.0.545 — ajustements d’interface

Les cadres des scores et régions réutilisent la vraie texture du menu ; leur
silhouette conserve le cadre gravé et les quatre rivets. La bordure extérieure
se fond progressivement dans ce fond, sans rectangle importé ni bande noire.
Les douze PNG originaux et les huit portraits sont préservés octet pour octet.

Les boutons du radar affichent le nom en haut, la progression ou « OK » en bas
à gauche et le prix blanc à ombre noire en bas à droite. Les noms prennent la
couleur de la maison, comme les autres commandes de bâtiments. Trois prix :
500 / 500 / 750 ; aucune règle de recherche n’a été modifiée.

Les menus principaux et leurs sections utilisent des couleurs fixes par famille,
Lite compris : Tornie (20,68,28), Jericho (52,52,52), ArcaKev (176,64,24).
Le changement de mod rafraîchit aussi les widgets déjà créés ; Vanilla restaure
son style par défaut. L’orange d’ArcaKev est assombri et plus rouge à la demande
de la propriétaire pour faciliter la lecture.

Validation : deux CTest, 145 cas et 10 891 assertions ; FR/EN natifs dans les sept
modes, 63 identités de campagne et 54 maisons de recherche plus le cas Vanilla
sans recherche. Prix testés dans les états disponible, en cours et terminé.
Les deux CI exécutent les mêmes contrôles ciblés sur les paquets, vérifient leur
démarrage et auditent tous les mods et les 20 PNG fournis avant publication.
Les suites exhaustives de scénarios de la publication précédente restent
consultables dans les rapports archivés ; elles ne sont pas revendiquées pour
ce lot de présentation. Aucun changement de version, protocole ou sauvegarde.

English: original artwork is unchanged. Runtime blending removes the rectangular
frame backing. Research prices are separate from names/progress; labels use house
colours. Fixed dark green, grey and reddish orange menu colours apply to full
and Lite mods. The 1.0.545 packages are replaced under the same version.

## Complément : bord droit et portraits

- Cadre droit des scores : alignement corrigé d’un pixel pour effacer jusqu’au
  dernier bord de l’ancien logo. Le contrôle vérifie désormais cette colonne.
- Troopers Fremen et Sardaukar : six icônes de production distinctes (1/3/5
  soldats par faction) et deux portraits dédiés à la sélection des unités,
  selon les noms de Troopers.zip. Les huit PNG sont conservés sans modification,
  sans teinte ni étoile ajoutée. WOR, Worfinery et plans Chaos suivent ces usages.

- Right score frame: corrected one-pixel alignment clears the old logo through
  its final inclusive edge. The regression now checks that column explicitly.
- Fremen/Sardaukar Troopers: six separate production icons (1/3/5 soldiers per
  faction) and two dedicated selected-unit portraits, following Troopers.zip
  filenames. All eight PNGs remain unchanged, without added tint or stars.
  WOR, Worfinery and Chaos blueprints use the corresponding production artwork.
