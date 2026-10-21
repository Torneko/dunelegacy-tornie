# Campagne coop et archonte — 1.0.541

[English guide](COOP-CAMPAIGN-EN.md)

Tous les participants doivent installer **1.0.541 / protocole 13**, activer le même mod et utiliser des noms différents. La connexion LAN ou Internet utilise le relais par l’hôte lorsque les invités ne peuvent pas communiquer directement, sans VPN. L’hôte doit rester joignable et connecté.

À chaque nouveau départ de mission, toutes les factions alliées actives reçoivent **le même capital initial que l’hôte**, même si leur champ de crédits est nul ou absent. Le mode facile ajoute ensuite 500 crédits par faction, une seule fois. Un chargement de sauvegarde conserve les soldes enregistrés. Cette correction remplace la 1.0.541 : tous les participants doivent la retélécharger (protocole 13).

## Joueurs et factions

Une nouvelle campagne propose jusqu’à **quatre factions alliées**, avec leurs bases, armées et crédits propres. Les emplacements 3 et 4 sont fermés au départ : tant qu’ils ne sont pas activés, ils ne créent aucune unité de départ, aucun MCV et aucun renfort. Les alliés peuvent choisir la même faction.

Cocher **Archonte** lors de la création permet jusqu’à **huit joueurs**. Dans le lobby, l’hôte attribue librement chaque contrôleur à une des quatre factions alliées ; tous les huit peuvent partager une seule faction. Les contrôleurs d’une même faction partagent ses unités, bâtiments et crédits. Une faction sans contrôleur peut rester fermée ou recevoir une IA. Les ennemis restent contrôlés par l’IA.

**Contrôles alliés**, facultatif, autorise aussi les ordres aux objets des autres factions alliées ; les dépenses restent à la charge du propriétaire. L’hôte choisit les types et difficultés des IA. Les invités ont accès à leurs couleurs bonus.

**Construction sur terrain allié**, désactivée par défaut, est réglée par l’hôte avant une nouvelle mission. Chaque faction alliée active peut construire près des dalles et bâtiments des autres alliés, y compris en archonte. Les bâtiments et dépenses restent à leur propriétaire ; le terrain doit rester valide et libre. Cette option est indépendante des Contrôles alliés. Les invités reçoivent le choix de l’hôte. Une sauvegarde conserve ce choix ; il reste verrouillé lors du chargement d’une mission déjà commencée. Les anciennes sauvegardes sans ce réglage gardent l’option désactivée.

| Édition | Campagnes sources | Variantes coop |
|---|---:|---:|
| Vanilla | 9 | 198 |
| Tornie | 12 | 264 |
| Jericho | 12 | 264 |
| ArcaKev | 12 | 264 |
| Tornie Lite | 6 | 132 |
| Jericho Lite | 6 | 132 |
| ArcaKev Lite | 6 | 132 |

Les 63 campagnes sources fournissent 1 386 cartes coop. La progression suit neuf étapes de la campagne de la faction choisie par l’hôte au départ ; cette source reste fixée dans la session.

## Créer et jouer

1. Ouvrir **Campagne coop**, puis LAN ou Internet, et créer une nouvelle campagne. Choisir le mode archonte si souhaité.
2. Les autres joueurs rejoignent depuis le navigateur ou par adresse. Régler les quatre factions alliées, les IA et les contrôleurs archonte dans le lobby.
3. Lancer après la synchronisation des versions, règles et ressources de tous les invités.
4. Après chaque mission, tous les humains connectés confirment être prêts avant la transition commune. Une déconnexion n’avance pas la campagne.

Le premier joueur conserve les forces d’origine. Chaque autre faction alliée active reçoit les forces et crédits adaptés à sa faction, un MCV déployable proche du chantier et, dans les intros concernées, un WOR. Les renforts du premier joueur sont reproduits pour chaque allié actif avec une unité compatible. Les objectifs de récolte additionnent l’épice des factions alliées et multiplient le quota solo par leur nombre ; plusieurs contrôleurs d’une même faction ne multiplient pas son quota.

Les adversaires conservent leurs forces et rôles. Une faction ennemie en conflit avec un allié est remplacée ; dans les éditions Lite, plusieurs adversaires peuvent partager une faction lorsque quatre alliés occupent le catalogue, mais leur propriété reste distincte.

## Variantes et difficulté

Chaque **nouvelle mission** tire un des scénarios existants, sans modifier les terrains originaux :

| Étape coop | Scénarios possibles |
|---|---|
| 1 | 001 |
| 2–7 | 002–004, 005–007, 008–010, 011–013, 014–016, 017–019 |
| 8 | 020 ou 021 |
| 9 | 022, technologie 9 |

Le tirage est commun aux participants, stocké dans la progression et conservé après une défaite ou un chargement. Certaines cartes existantes ont le même seed de terrain tout en différant par leurs forces. Les anciennes sessions gardent leur choix antérieur.

**Mode facile**, désactivé par défaut : une nouvelle campagne commence à l’étape 2 ; chaque faction alliée active, humaine ou IA, reçoit **500 crédits à chaque départ de mission** et paie ses achats 25 crédits de moins (minimum 1). Les ennemis ne reçoivent aucun bonus.

**Forces ennemies +5** et **Forces ennemies +10** sont indépendantes et cumulables : 5, 10 ou **15 unités supplémentaires par adversaire présent**, à chaque nouvelle mission, adaptées à la technologie et placées près de ses forces ou sur une bordure pour un adversaire arrivant par renforts. Un adversaire absent ne reçoit rien. Les checkpoints restaurent les forces existantes sans les ajouter de nouveau.

## Sauvegardes et reprise

La progression reste indépendante du solo : `coop/<session>.ini` et checkpoint `coop/<session>.dls`, à côté de `Dune Legacy.ini`. Conserver les deux fichiers. Une reprise utilise le mod de la session et le checkpoint existant ; les noms des contrôleurs peuvent être réattribués, avec les effectifs par faction conservés pour une partie déjà commencée.

**Charger une sauvegarde**, dans le lobby hôte, propose les sauvegardes manuelles `mpsave/*.dls` et automatiques `coop/*.dls`. Un chargement compatible rouvre le lobby sur la mission enregistrée ; tous les invités le rejoignent à nouveau. Annuler conserve le lobby. Les nouveaux fichiers utilisent le format **9833** ; les anciens restent lisibles avec leur carte et leurs règles enregistrées.

Le haut fait **Ensemble sur Arrakis / COOP_CONQUEROR** récompense la fin de la campagne commune, avec un humain ou une IA ; le mode facile est accepté. Il ne complète aucune campagne solo. Le catalogue conserve 67 hauts faits et les profils restent locaux.

## Validation et maintenance

Les tests automatisés couvrent les sept modes, les étapes 1–9, les alliés fermés ou actifs, le déploiement des MCV 3/4, les huit contrôleurs dans une faction, les crédits faciles, les forces +5/+10/+15, les sauvegardes et les affectations du lobby. Le transport UDP à huit filtre les communications directes entre invités et vérifie le relais, le démarrage et les commandes de tous les participants. Une partie humaine sur plusieurs connexions Internet reste à confirmer ; la confirmation historique de Tornie sur deux PC concerne 1.0.538.

Templates : `data/coop/faction<id>/` pour Vanilla, `mods/<mod>/campaign/coop/faction<id>/` pour les mods, avec suffixes `-v1` et `-v2`. `scripts/build-coop-campaign.py --check` contrôle les 1 386 variantes, les 63 anciennes cartes racines et les empreintes des 2 342 fichiers solo protégés. Les sections auxiliaires des alliés 3/4 sont matérialisées seulement lorsqu’ils sont actifs ; le contexte `[COOP]` synchronise la carte, les contrôleurs et le tirage.
