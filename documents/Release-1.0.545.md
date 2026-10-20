# Préparation 1.0.545 / 1.0.545 preparation

La publication précédente reste la 1.0.544 corrigée, source `d6a29fd`,
site public `0e8430d`. Cette préparation comprend le lot radar/Chaos/cinématiques
déjà vérifié sur `fbe8b46`, puis les ajouts autorisés le 20 octobre.
Aucun tag ni publication ne doit être créé sans instruction de publication.

## Ressources et interface

- Quatre portraits originaux : Fremen 1/5 ; Sardaukar 3/5. Utilisés dans
  les fiches d’unités et les commandes de production correspondantes, par identité
  de faction (plans en production, provenance pour les unités sélectionnées).
  Aucune étoile d’éditeur ni chiffre supplémentaire n’est ajouté aux portraits.
- Douze cadres dorés de campagne. Sélection et confirmation utilisent l’identité
  réelle de la maison, indépendamment du slot, du mod et de la couleur bonus.
  Mise à l’échelle en mémoire, cadre entier sans découpe, originaux inchangés.
- Rejouer l’intro : bouton à droite, immédiatement au-dessus de la coop ;
  réutilise la vraie introduction puis restaure la musique du menu.
- Bannière Fremen du site Tornie : recadrage visuel du PNG original conservé.
  Le site FR/EN de préparation annonce les ajouts sans changer les liens 544.

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

Unpublished 1.0.545 preparation. Original artwork is preserved byte-for-byte.
Four requested Fremen/Sardaukar portraits, twelve campaign-only golden frames,
Replay Intro and three achievements are integrated after the previous validated
Radar/cinematics work. Current public downloads remain at 1.0.544.
Co-op rescue achievements resolve at team victory and support all local
perspectives with two, three or four allied factions; Archon controllers share
their house outcome. Sabotage requires a local Saboteur destroying an enemy Palace.


Validation finale : CTest 2/2, 145 cas / 10 891 assertions ; 1 606 scénarios et suites natives FR/EN. Les deux CI de la source ba003c2cc87e0cc2dbe43b36b235e4889641cf9f ont réussi. Publication autorisée. Voir Validation-1.0.545.json.
