# Dune Legacy Tornie 1.0.541 — ArcaKev, coop et corrections

**Révision 1.0.541 — construction alliée, éditeur et mentat Rebels**

- **Construction sur terrain allié** : nouvelle case du lobby coop, désactivée par défaut et réglée par l’hôte avant une nouvelle mission. Elle permet de prolonger la zone de construction à partir des dalles et bâtiments des autres alliés actifs, y compris en archonte. Elle est indépendante des Contrôles alliés ; chacun garde ses bâtiments et ses dépenses. Les règles de terrain, d’occupation et de technologie restent applicables.
- **Éditeur** : cliquer une unité ou un bâtiment sélectionne son vrai propriétaire par identifiant, même lorsque l’ordre visible des factions diffère de l’ordre interne. Les joueurs nommés/numérotés et les cartes rechargées sont couverts dans les sept modes.
- **Mentat chat animé pour Rebels**, issu du chat existant de Wildspade/Jericho : gris foncé dans Vanilla, Jericho et ArcaKev, vert dans Tornie. La teinte suit la palette de la faction ; yeux, bouche, moustaches, épaule et cinq états d’animation restent alignés. Les éditions Lite ne proposent pas Rebels.
- Les corrections précédentes sont conservées : capital initial de l’hôte pour les quatre alliés actifs, +500 une fois par mission en facile, Rebels gris foncé et Corruptique jaune dans ArcaKev, avec leurs bannières inchangées.
- **Retélécharger cette 1.0.541 sur tous les postes** : protocole **13**, nouvelles sauvegardes **9833**. Les anciennes sauvegardes restent lisibles ; l’option de construction alliée y est désactivée si elle n’était pas enregistrée. Le chargement garde les soldes enregistrés et les règles de la partie.

- **ArcaKev et ArcaKev Lite** : dérivés Jericho, unités personnalisées, épice normale **plus verte et bleue**, éditeur et génération de cartes adaptés. Campagnes et régions propres au nouveau mod.
- Ordre ArcaKev : **Harkonnen, Atreides, Ordos, Fremen, Sardaukar, Mercenary, Kleshmersh, Wildspade, Rebels, Neutral, Tharpique, Corruptique**. Les identifiants de sauvegarde restent stables.
- **Coop jusqu’à quatre factions alliées** ; **archonte jusqu’à huit joueurs**, affectation libre des contrôleurs, y compris tous dans une seule faction. Emplacements 3/4 fermés : aucune armée, MCV ou renfort.
- À chaque nouvelle mission, tirage parmi les scénarios existants : trois variantes aux étapes 2–7 (002–019), deux à l’étape 8 (020/021). Tirage conservé après défaite ou chargement. Finale 22 : **technologie 9**, en campagne et skirmish.
- **Facile** : +500 crédits au départ de **chaque mission** pour tous les alliés actifs, réduction d’achat de 25 conservée. **Forces ennemies +10** en option, cumulables avec +5 pour un total de **15** par adversaire présent.
- Renforts et quota commun adaptés aux factions alliées actives. Transition après confirmation de tous les humains. Lobby, reprise et sauvegardes gèrent les huit contrôleurs ; couleurs bonus accessibles aux invités.
- **Cases des mods** correctement appliquées ; sauvegarde des métadonnées sans accumulation de guillemets et bouton Enregistrer accessible. **Bannières Jericho** actualisées au démarrage.
- **Kleshmersh Vanilla orange**, Troopers ordinaires sans feu. Hors Vanilla, une **WOR Kleshmersh empruntée en Chaos** produit les Troopers incendiaires au prix nominal Kleshmersh de **200 crédits**, y compris dans les éditions Lite. Dégâts d’impact du **Trooper lance-flammes +20 %**.
- **Scoutpost, Flamepost et Chemipost** : missiles dès la base et après amélioration, portée **7 cases** au lieu de 5. Flamepost : effet du Flame Tank et **12 dégâts** au lieu de 10. Chemipost conserve le missile de soin allié.
- **Sonic Trike** au niveau suivant lorsqu’il était au niveau 2. **Wildspade et Sardaukar** : Light Factory niveau **2**, hors Vanilla. **Ordos et Neutral** : Siege Tank niveau **6**, troisième amélioration.
- **Doublefinery** : déchargement des récolteurs **15 % plus rapide** dans les deux baies ; aperçu solide **5×2** dans la grille pointillée **5×3**, placement cohérent avec le bâtiment.
- **Tourelles ajoutées aux véhicules personnalisés** : proportions cohérentes aux zooms **1×, 2× et 3×**. Les trikes conservent leurs graphismes.
- **Mentat husky animé de Kleshmersh**, issu du ZIP fourni par Tornie : cinq états de regard et de bouche, dans Vanilla, Tornie, Jericho et ArcaKev. PNG natifs conservés, parole liée au briefing.
- **Éditeur** : changement d’équipe fonctionnel dans les listes défilantes et **12 équipes**. Ordre ArcaKev, unités et épices adaptés.
- **Site français/anglais** : pouvoirs de chaque palais par maison et mod, recharge, technologies, nouveaux mods, husky et notes ; même contenu dans le site local.

Tous les joueurs doivent utiliser **1.0.541 / protocole 13**, avec le même mod. Les nouvelles sauvegardes utilisent **9833** ; les anciennes restent lisibles. Le relais Internet par l’hôte sans VPN et les correctifs AppImage/Mint de 1.0.540 sont conservés. L’hôte doit rester connecté et joignable. Sous Mint, choisir `DuneLegacyTornie-1.0.541-Linux-Mint-Compatible.AppImage` ; sans FUSE, ajouter `--appimage-extract-and-run`. Sous Windows, extraire tout le ZIP dans un nouveau dossier avant de lancer `dunelegacy.exe`.

Les tests automatisés reproduisent des invités isolés avec huit participants UDP, vérifient les commandes, le démarrage, les sauvegardes et les sept modes. Les résultats finaux et liens CI sont consignés dans le rapport de validation. Une partie humaine sur plusieurs accès Internet et le lancement natif sur Mint 22.3 / noyau 6.17 restent à confirmer.
