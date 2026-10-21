# 1.0.545 corrigée / Corrected 1.0.545

La publication remplace les paquets initiaux de la 1.0.545, sans nouveau numéro.
Source du jeu : `89f8fee66ba0abdc2555d696455b4805e6aae15c`. Protocole 21, sauvegardes 9839.
La publication initiale est conservée sous `archive/releases/v1.0.545-initial`.

## Ressources et interface

- Quatre portraits originaux : Fremen 1/5 ; Sardaukar 3/5. Utilisés dans
  les fiches d’unités et les commandes de production correspondantes, par identité
  de faction (plans en production, provenance pour les unités sélectionnées).
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
Les deux portraits supplémentaires Fremen 3 et Sardaukar seul sont conservés dans
l’archive reçue ; les quatre variantes demandées sont les ressources actives.

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
