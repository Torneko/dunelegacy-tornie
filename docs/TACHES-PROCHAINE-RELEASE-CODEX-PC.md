# Tâches pour Codex PC — prochaine release Dune Legacy Tornie

Demandes validées par Tornie les 19 et 20 octobre 2026.

## Projet et point de départ

- Dépôt : https://github.com/Torneko/dunelegacy-tornie ; projet indépendant de DuneCity.
- Base publiée : **1.0.544 corrigée**, tag/source
  `d6a29fd82906eb1f4856eced2dfb01fe3e74637e`, protocole 20, nouvelles sauvegardes 9838.
- Les correctifs Slowpost/Scoutpost, animation du Slowpost, tourelle du Ranged Tank,
  libellés Worfinery et moissonneuses bicolores sont déjà publiés dans cette 544.
- Lire `AGENTS.md`, `README.md` et `docs/WEBSITE.md` avant de poursuivre.
- Reprendre la branche distante `feature/original-westwood-intro`. Elle contient
  ce passage de relais, le cahier des charges du radar et des **ébauches non
  validées** de l'ouverture Westwood et du générique original. Ce n'est pas un
  build prêt à publier.

## 1. Recherches au radar

Créer un système de recherches : **deux recherches communes et une spécialisation
par maison**, soit trois recherches accessibles par maison. Ces concepts sont
approuvés ; leurs noms d'affichage restent provisoires.

### Deux recherches communes

| Recherche | Effet validé |
| --- | --- |
| Extension de la base | **+2 cases de portée de construction** autour de la base. |
| Vision élargie | **+2 cases de champ de vision** pour toutes les unités et tous les bâtiments, **sauf les unités aériennes**. |

### Douze spécialisations

| Maison | Effet validé |
| --- | --- |
| Harkonnen | Plus de dégâts pour les **Combat Tanks**, l'**explosion du Devastator** et le **missile du palais**. |
| Atreides | **Troopers plus rapides**, escouades comprises : vitesse de déplacement. |
| Ordos | **Véhicules légers plus rapides**. |
| Fremen | **Troopers plus rapides**, escouades comprises : vitesse de déplacement. |
| Sardaukar | Chaque **char, lourd ou non**, a **35 % de chances** de faire apparaître **un Trooper de la même maison** à sa destruction. |
| Mercenary | **−10 % sur le coût des unités non lourdes**. |
| Neutres | Amélioration de l'**efficacité des réparations**. Vitesse, coût ou les deux restent à définir. |
| Rebels | **+1 case de portée** pour les armes des véhicules. |
| Wildspade | **Unités aériennes plus robustes**, dont les Ornithopters, les Carryalls et leurs variantes. |
| Kleshmersh | Les dégâts de feu durent **2 secondes de plus**, avec les mêmes dégâts par seconde. |
| Corruptique | **Blindage renforcé sur certains chars**, avec une **légère perte de vitesse**. |
| Tharpique | **Meilleure vision des éclaireurs et de l'artillerie**. |

**Confirmation explicite : le bonus Tharpique se cumule avec le +2 de vision
commun.** Les unités concernées profitent des deux recherches.

### Détails restant à définir

- Pourcentages ou valeurs non chiffrés : dégâts, vitesses, robustesse aérienne,
  blindage/ralentissement Corruptique et vision Tharpique.
- Nature et portée de la réparation neutre : vitesse, coût ou les deux ; types
  de réparations concernés.
- Listes exactes des unités et armes : chars Sardaukar, unités non lourdes
  Mercenary, véhicules Rebels, chars Corruptique, éclaireurs/artillerie Tharpique
  et effets de feu Kleshmersh.
- Prix, temps, niveau technologique et prérequis de chaque recherche.
- Mods concernés et fonctionnement des éditions Lite.
- Application aux unités existantes et futures ; capture/conversion, perte du
  radar, Starport, Easy Mode et Chaos Mode.
- Cumul des 35 % Sardaukar avec l'éventuelle apparition d'infanterie déjà présente
  dans le moteur. Le Trooper demandé est une unité, pas une escouade.

Ne pas présenter un réglage encore ouvert comme un choix confirmé par Tornie.
Le but est de différencier les maisons et de vérifier l'équilibre en partie.
Une réduction générale des forces Corruptique/Tharpique n'a pas été décidée.

Prévoir l'interface du radar, les commandes déterministes et la persistance des
recherches en sauvegarde/multijoueur. Le radar actuel n'a pas encore de système de
recherches. Voir aussi `docs/RADAR-RESEARCH-NEXT-RELEASE.md` pour la version FR/EN.

## 2. Intro Westwood fidèle à l'original

- Ajouter le **logo animé Westwood Studios bleu** avant l'intro actuelle.
- Le rapprocher le plus possible de l'original : vrais graphismes, palette,
  cadence, fond et fondus. La fidélité demandée porte sur le logo Westwood, pas
  sur la création d'un logo Tornie de remplacement.
- **Inclure le son d'origine**, synchronisé avec l'animation.
- Ressources déjà trouvées dans le jeu : `WESTWOOD.WSA`, `WESTWOOD.PAL`
  (`INTRO.PAK`) et **DUNE0.ADL, piste 4** (`SOUND.PAK`).
- Référence technique consultée : OpenDUNE, commit
  `ebea79aeba025be2fe9004570e9255044aacc0be`, `src/cutscene.c`,
  `Gameloop_Logos()` et `src/table/sound.c`. Son 0x24 ; fondu de 60 ticks,
  animation tous les 6 ticks, maintien jusqu'à 360 ticks, puis fondu.
- Garder les couleurs du jeu indépendantes de la palette du logo ; vérifier le
  mode muet, le volume, le saut de l'intro et l'arrêt de son audio.

## 3. Crédits originaux après la conquête

- Après la conquête finale de la planète et la cinématique de fin de campagne,
  afficher le **générique original de Dune II**.
- Conserver les vrais noms, l'ordre et les textes originaux disponibles dans la
  langue du jeu, ainsi que le défilement, les images et la musique d'origine.
- Ressources disponibles : `CREDITS.ENG`, `CREDITS.FRE`, `CREDITS.GER`,
  polices `NEW6P.FNT`/`NEW8P.FNT`, images `CREDIT1.SHP` à `CREDIT11.SHP`
  utilisées par le générique original, palette `IBM.PAL`.
- La musique originale 33 correspond à **DUNE20, piste 2**, actuellement associée
  à `MUSIC_GAMESTAT` dans les lecteurs ADL/XMI.
- Vérifier les caractères français, le cadrage, le défilement et la sortie du
  générique. Préserver les attributions du projet existantes.

## 4. Cinématiques et cohérence des campagnes

Tornie signale que des cinématiques manquent et que les textes/couleurs ne sont
pas associés aux maisons de la campagne.

- Rétablir les cinématiques intermédiaires et finales pour les campagnes
  concernées, y compris les maisons ajoutées.
- Choisir les séquences d'origine correspondant au **modèle réel de campagne**,
  puis associer les noms, sous-titres, interlocuteurs et couleurs aux maisons
  jouées et à leurs adversaires.
- Vérifier les couleurs des intervenants et de la planète conquise, ainsi que
  les images où apparaît une couleur de maison.
- Préserver les dialogues et l'enchaînement de l'histoire autant que possible.
  Les adaptations doivent suivre les rôles de campagne existants.
- Vérifier les mods et leurs éditions Lite, les langues FR/EN et les couleurs
  personnalisées, sans confondre les IDs techniques et l'identité des factions.

Pistes déjà identifiées :

- `src/sand.cpp` ne déclenche `Meanwhile` aux niveaux 4 et 8 que pour
  Harkonnen/Atreides/Ordos. La finale est déclenchée au niveau 9.
- `Meanwhile` refuse les autres maisons et calcule ses dialogues/images à partir
  de l'ID historique de la maison.
- `Finale` ne charge les scènes de palais complètes que pour ces trois maisons ;
  les autres n'ont que la séquence de conquête de la planète.
- `config/CampaignPlans.json` et `mods/<mod>/data/CampaignPlan.ini` fournissent
  `Template` / `vanillaTemplate` et les trois adversaires. Les maisons historiques
  elles-mêmes ont parfois un autre modèle dans les mods : ne pas supposer que le
  nom de la maison détermine directement la scène.
- `TextManager` sait déjà adapter les noms des briefings selon les plans des mods
  complets ; cette logique n'est pas raccordée aux cinématiques. Les fichiers
  `CampaignPlan.ini` existent également dans les éditions Lite.

## 5. État exact des ébauches transmises

- `WestwoodLogoVideoEvent.h/.cpp` : première implémentation de l'animation bleue
  et de sa signature AdLib ; ajoutée au début de `Intro.cpp` et à CMake.
- `OriginalCredits.h/.cpp` : première implémentation des polices/texte d'origine,
  des images et du défilement ; ajoutée à CMake. **Pas encore raccordée à la fin
  de campagne.**
- **Aucune compilation ni validation réussie de ces ébauches.** Relire, corriger
  et tester avant de les conserver. Le premier essai de compilation n'a pas pu
  démarrer car les dépendances temporaires de l'environnement précédent avaient
  disparu ; ce n'est pas une validation du code.
- La correction des associations de cinématiques/textes/couleurs **n'est pas
  implémentée**. Les recherches au radar sont **spécifiées, pas implémentées**.
- Les paquets publics de la 1.0.544 et le site publié n'ont pas reçu ces travaux.

## 6. Validation et livraison de la prochaine version

- Compiler le jeu et les tests Windows/Linux ; exécuter les contrôles requis par
  `AGENTS.md`, dont CTest pour les changements moteur.
- Vérifier en rendu réel le logo bleu, le son, les sous-titres, les couleurs et
  le générique, ainsi que le saut des scènes.
- Tester les recherches : coûts/prérequis, cumul Tharpique, mort Sardaukar,
  application des bonus, sauvegarde/rechargement et synchronisation réseau.
- Préserver les IDs stables, les sauvegardes lisibles et les ressources d'origine.
  Adapter les versions de protocole/sauvegarde si nécessaire.
- Mettre à jour les notes et la documentation **français/anglais**, puis garder
  le site GitHub Pages, les téléchargements et le site hors ligne alignés sur la
  version effectivement publiée. Récupérer le site actuel avant toute édition.
- Utiliser `scripts/bump-version.sh` au moment de préparer le numéro de version.
