# 1.0.543 — Vente, coopération et nouvelles unités

Linux, AppImage Mint et Windows partagent la même source. Tous les participants doivent utiliser **1.0.543 / protocole 17** ; nouvelles sauvegardes **9835**, anciennes sauvegardes lisibles.

## Derniers ajustements

- Icônes originales des Troopers lance-flammes de 3 et 5 fournies dans `troopers_flames.tar.gz`, conservées intactes dans les six mods et utilisées selon la technologie Kleshmersh, y compris en Chaos ; aucune étoile ni chiffre superposé.
- Flame Quad : précision directe à courte portée, sans dispersion ni détonation retardée des missiles à longue portée. Correction du biais d’un pixel à gauche sur les vues de largeur impaire, dont la vue orientée vers la droite.
- Vente : effet et son de destruction du bâtiment, puis dalles de béton sur toute son emprise, sans texture de ruine. Le propriétaire du béton, le remboursement de 50 %, l’évacuation des unités et les statistiques de vente sont conservés.
- Bouton de vente après l’emplacement d’amélioration, sans chevauchement.
- Jericho/Lite et ArcaKev/Lite : couleur bonus vert foncé identique aux Rebels de Tornie ; couleurs de faction par défaut conservées.
- Protocole réseau 17 pour éviter de mélanger les anciennes trajectoires et ventes avec ces nouveaux comportements ; version 1.0.543 et sauvegardes 9835 conservées.

## Correction de l’affichage Linux

- PulseAudio et XRandR sont compilés explicitement et contrôlés avant création des paquets ; le détail des erreurs audio est conservé.
- Détection des moniteurs physiques par XRandR dans les paquets Linux ; bibliothèque incluse dans les deux AppImages.
- Une fenêtre conserve la résolution choisie au lieu de passer au seul mode du bureau annoncé par Linux. Les anciennes dimensions dépassant le moniteur sont ramenées à cet écran.
- Les résolutions plus petites sont de nouveau proposées dans les options, même quand Linux n’annonce que la résolution native. Plein écran limité au moniteur de la fenêtre.
- Remplacement de la 1.0.543 uniquement ; sauvegardes 9835 conservées.

## Remplacement de la 1.0.543

- Teinte de maison du Flame Quad et de l’infanterie à gaz corrigée malgré la différence de conversion VGA ; le vert du gaz reste intact.
- Étoiles bleues des trois nouvelles unités dans les boutons et sur le terrain de l’éditeur aux zooms 1×/2×/3×. Aucune étoile bleue ajoutée sur les portraits ; PNG sources conservés.
- Neutral : blanc plus clair dans Tornie et ArcaKev.
- Neutral produit le Saboteur à la Worfinery au niveau 7 avec IX, pour 500 crédits, en campagne et multijoueur (mods complets Tornie, Jericho et ArcaKev). Production réservée à Neutral ; pouvoir de palais existant conservé.
- Aucun chiffre « 5 » superposé sur les portraits des super escouades.
- Les autres nouvelles unités, le napalm, le ralentissement et le ver apprivoisé restent reportés, dans l’attente de leurs graphismes et spécifications.

- Flame Quad : chaque direction est centrée sur sa tuile à partir des pixels visibles, aux zooms 1×/2×/3×, en jeu et dans l’éditeur. PNG originaux inchangés.
- Le Trooper lance-flammes Harkonnen est exclu du tirage de véhicule « Special » ; sa production niveau 7 + IX est conservée. L’infanterie à gaz est également classée comme infanterie.
- Protocole réseau 16 : retélécharger les paquets sur tous les postes ; le contrôle réseau refuse les éditions précédentes de la 1.0.543 (protocoles 15 et 16). Sauvegardes 9835 conservées.

## Modifications validées localement

- Vente des bâtiments à 50 % de leur prix de construction, même endommagés, sans besoin de stockage pour le remboursement. Bouton `$` de la taille de Réparer, après l’emplacement d’amélioration. Activée par défaut dans Tornie, Tornie Lite, ArcaKev et ArcaKev Lite ; réglable dans les règles de partie.
- Les unités contenues dans une raffinerie ou un centre de réparation sont évacuées avant la vente. Une livraison Starport en route doit se terminer avant la vente. La production prépayée non livrée est remboursée séparément du prix du bâtiment.
- Premier bâtiment vendu : succès « Liquidités ». Nombre de ventes et crédits remboursés dans les statistiques, conservés en sauvegarde.
- Hors coop multijoueur : contrôle allié et construction sur terrain allié sont deux permissions indépendantes de l'hôte, limitées aux membres d'une même équipe. Les fonds et bâtiments gardent leur propriétaire.
- Option de compteur de moissonneuses, désactivée par défaut dans Tornie/Tornie Lite et activée dans les autres modes.
- Protocole 17 et des sauvegardes 9835 ; conservation de la lecture des anciennes sauvegardes.

## Nouvelles unités intégrées et validées localement

- Troopers lance-flammes Kleshmersh conservés et accessibles via les technologies tirées en Chaos ; icône dédiée selon le donneur technologique.
- Harkonnen : Troopers lance-flammes séparés au niveau 7 avec IX, sans retirer les Troopers ordinaires ; hors Vanilla.
- Kleshmersh : Rocket Trike niveau 3/amélioration 1 ; Flame Quad niveau 5/amélioration 2 ; plus de Sonic Trike en production normale.
- Infanterie à gaz Ordos et Tharpique : WOR et Worfinery, IX ; soigne les alliés, gaz du Deviator sur les ennemis.
- Cinq PNG reçus, conservés intacts dans les six mods. Magenta retiré au chargement du sprite à gaz, noir opaque préservé ; découpage natif et zooms entiers 1×/2×/3×.
- Les nouvelles unités sont ajoutées à l’éditeur, aux renforts et aux tables de sauvegarde avec des identifiants ajoutés après les anciens.

Voir les [spécifications et valeurs initiales](https://github.com/Torneko/dunelegacy-tornie/blob/v1.0.543/docs/UNITS-1.0.543.md). Le site FR/EN et son archive hors ligne accompagnent la release Linux/Windows 1.0.543.

Flame Quad : **300 crédits et 12 dégâts par tir**, contre **200 crédits et 7 dégâts par tir** pour le Quad ; effet de flamme en plus. Les six configurations ont été contrôlées.
