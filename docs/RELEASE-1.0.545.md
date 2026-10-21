# Correctifs 1.0.545 — soins, compteurs et introduction

Source : `e8cf3e3cbc21be1b77dc05efc80d9f003b6bc469`. Source précédente : `ba4c6255a8e96e7c88c348a77e464b17514c9867`, conservée sous
`archive/releases/v1.0.545-trooper-portraits`. Protocole 22 ; sauvegardes 9839.

- Tharpique : délai entre les soins augmenté de 50 % pour l’infanterie à gaz
  et le Chemical Siege Tank (1,92 → 2,88 s), ainsi que le Chemipost (2,4 → 3,6 s).
  Les soins restent de 15 PV ; les attaques de déviation et les autres factions
  conservent leur cadence. La règle suit la provenance des plans en Chaos.
- Sans sélection, les boutons Ornithopter et Chemical Carryall affichent le nombre
  d’aéronefs sélectionnables entre parenthèses, dans la couleur de la faction ou
  couleur bonus du joueur. Les ornithoptères élite et le contrôle allié sont pris
  en compte. Les compteurs disparaissent avec ces boutons lors d’une sélection.
- Recherches radar : boutons communs visibles à partir du niveau 4, spécialisation
  au niveau 7 ; l’IX reste requise pour lancer cette dernière.
- Signature Westwood : son renforcé d’environ 6 dB, déclenché au premier mouvement
  du logo après une seconde de fondu. Volume utilisateur, silence et arrêt lors
  du passage de l’introduction respectés.
- Réseau : tous les participants doivent retélécharger cette 1.0.545 corrigée
  (protocole 22). Sauvegardes 9839 inchangées, anciennes toujours lisibles.

- Tharpique: healing recharge increased by 50% for Gas Infantry and Chemical
  Siege Tank (1.92 → 2.88 s), and Chemipost (2.4 → 3.6 s). Healing stays at 15 HP;
  enemy deviation attacks and other factions keep their cadence. Chaos follows
  the production blueprint’s faction.
- With nothing selected, Ornithopter and Chemical Carryall shortcut buttons show
  the selectable aircraft count in parentheses, using the player’s faction/bonus
  colour. Elite Ornithopters and allied control are included. These counters
  disappear with their buttons when an object is selected.
- Radar research: common buttons appear at level 4, specialization at level 7;
  the IX remains required to start specialization.
- Westwood signature: approximately +6 dB, starting with the logo’s first animated
  frame after its one-second fade-in. User volume, mute and skip cleanup remain
  respected.
- Network: every participant must download this corrected 1.0.545 (protocol 22).
  Save format remains 9839, with earlier saves still readable.


# 1.0.545 corrigée / Corrected 1.0.545

La publication remplace les paquets initiaux de la 1.0.545, sans nouveau numéro.
Source du jeu : `ba4c6255a8e96e7c88c348a77e464b17514c9867`. Protocole 21, sauvegardes 9839.
La publication initiale est conservée sous `archive/releases/v1.0.545-initial`.

## Ressources et interface

- Huit portraits originaux : Fremen et Sardaukar 1/3/5 en production,
  plus un portrait de sélection dédié par faction. Identité des plans pour les
  achats ; provenance de l’unité pour la sélection, indépendamment de sa couleur.
  Aucune étoile d’éditeur ni chiffre supplémentaire n’est ajouté aux portraits.
- Douze cadres dorés dans les scores et les régions : identité réelle de la
  maison, indépendamment du slot, du mod et de la couleur bonus. Les bannières
  habituelles restent dans les choix et confirmations de campagne.
  Mise à l’échelle en mémoire ; cadre gravé et rivets conservés, fond extérieur
  fondu dans la texture du menu par transparence progressive. Originaux inchangés.
- Radar : coûts blancs en surimpression séparés des noms et de la progression,
  libellés aux couleurs de la maison, états disponible/en cours/terminé lisibles.
- Menus et sections, Lite compris : Tornie vert foncé, Jericho gris foncé,
  ArcaKev orange foncé rougeâtre (176, 64, 24), plus lisible.
- Rejouer l’intro : bouton à droite, immédiatement au-dessus de la coop ;
  réutilise la vraie introduction puis restaure la musique du menu.
- Bannière Fremen du site Tornie : recadrage visuel du PNG original conservé.
  Le site FR/EN et son archive hors ligne accompagnent les paquets 545 corrigés.

[Provenance et empreintes des ressources](RELEASE-1.0.545-assets.json).
Les documents de l’archive fournie servent uniquement de références de provenance.
Les huit PNG nommés de Troopers.zip sont actifs et intacts. Les fichiers 3 Fremen
et Sardaukar seul sont désormais utilisés ; les portraits de sélection sont distincts.

## Succès

Catalogue : 73. Les trois nouveaux compteurs locaux sont `CoopVictoriesRescued`,
`CoopVictoriesRescuing` et `PalacesSabotaged`. Les profils existants et leurs
succès restent lisibles. L’étoile 100 % suit automatiquement les 73 entrées.

- Éliminé en coop et équipe victorieuse grâce à un allié encore en vie.
- Encore en vie et victoire coop après l’élimination d’au moins un allié.
- Un Saboteur du joueur détruit un palais ennemi. Ni simple dégât, ni allié,
  ni autre bâtiment ou destruction par un autre joueur ne qualifie.

La coop se résout à la victoire de l’équipe : deux, trois ou quatre factions.
Chaque contrôleur archonte utilise l’état de sa maison. Un abandon, une
déconnexion ou une défaite d’équipe n’accorde aucun de ces succès de victoire.
Aucun hasard, commande réseau ou identifiant de contenu n’est ajouté.
Protocole 21 / nouvelles sauvegardes 9839 conservés pour cette préparation.

## Vérifications

La validation du lot précédent est historique dans
[Validation-Next-Release.json](../release_notes/Validation-Next-Release.json).
Les ajouts disposent de tests de critères positifs/négatifs et d’un contrôle
natif `--verify-release545` : sept mods, cadres et portraits exacts,
confirmations complètes, coop à 2/3/4 factions depuis chaque joueur, vraie
détonation de Saboteur et bouton de relecture de l’intro.
Le contrôle natif est aussi inclus dans la vérification complète des mods en CI.
Les résultats de cette révision sont enregistrés après exécution.

---

Corrected 1.0.545. Original artwork is preserved byte-for-byte.
Four requested Fremen/Sardaukar portraits, twelve golden score/region frames,
Replay Intro and three achievements are integrated after the previous validated
Radar/cinematics work. Corrected Windows/Linux/Mint downloads retain version 1.0.545.
Co-op rescue achievements resolve at team victory and support all local
perspectives with two, three or four allied factions; Archon controllers share
their house outcome. Sabotage requires a local Saboteur destroying an enemy Palace.

Les ajustements d’interface sont vérifiés en FR/EN dans les paquets Windows et
Linux/Mint, avec CTest moteur/réseau et contrôles de démarrage. Leurs tests ciblés
ne répètent pas les 1 606 scénarios historiques. [Validation](UI-545-REFINEMENTS.json).

UI-only replacement: frame background blending, separate Radar prices and house
colours, and dark green/grey/reddish-orange mod menu text, including Lite.

Cadre droit des scores : le logo hérité occupe aussi la colonne finale cx+317.
Le nettoyage et le nouveau cadre partent de cx+212, alignés sur ses 106 pixels.
Le test a échoué avant correction et passe avec la bonne limite inclusive.

English: six production icons and two dedicated selected-unit portraits follow
Troopers.zip names. Right-score cleanup includes the former logo’s final column.
