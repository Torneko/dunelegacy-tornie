# Jouer à Dune Legacy Tornie — 1.0.546

[English guide](HOWTOPLAY.md) · [Coop](docs/COOP-CAMPAIGN.md) · [Campagnes](docs/CAMPAIGNS.md)

Extraire tout le ZIP Windows dans un nouveau dossier, puis lancer `dunelegacy.exe`. Sous Linux/Mint, utiliser le paquet correspondant ou l’AppImage compatible Mint ; la rendre exécutable et ajouter `--appimage-extract-and-run` si FUSE manque. Le site bilingue est à https://torneko.github.io/dunelegacy-tornie/ ; dans son archive hors ligne, ouvrir `dist/index.html`.

Le menu **Mods** propose Vanilla, Tornie, Tornie Lite, Jericho, Jericho Lite, ArcaKev et ArcaKev Lite. Vanilla comporte neuf factions, les mods complets douze et chaque Lite six. Choisir le mod avant une campagne ou le chargement de sa sauvegarde.

## Base et campagne

Construire sur la roche. Chantier, éoliennes et raffinerie forment l’économie habituelle. Protéger les moissonneuses, alimenter la base, stocker l’épice et améliorer les usines. Les escarmouches sont dans **Joueur seul → Partie personnalisée**.

Les campagnes **Tornie/Tornie Lite** révisées utilisent des bâtiments, unités, technologies et renforts adaptés aux factions. Un soutien ennemi demeure pendant chaque campagne. Les missions 001–019 ont deux adversaires, 020–021 trois et 022 quatre. L’intro commence avec 1 500 crédits et un quota de 2 000 crédits raffinés. Rebels dispose d’un port stellaire et d’une petite économie : acheter le MCV, sans chantier/MCV gratuit. Wildspade a des aéronefs prêtés et une menace antiaérienne. Commencer une **nouvelle campagne** pour les scénarios révisés. Les campagnes Jericho/ArcaKev, Lite compris, gardent la référence 545 en attendant l’avis des auteurs. Les archives Vanilla sont conservées.

L’infanterie de capture peut prendre un bâtiment ennemi en santé rouge. **Sabotage**, sous **Retraite**, fait chercher des bâtiments ennemis capturables aux Soldiers/Troopers et escouades ; une sélection mixte ne l’applique qu’à cette infanterie. Sinon, elle endommage le bâtiment et est consommée. Le déploiement sain d’un MCV et la consommation d’une unité de capture suivent des règles différentes pour les succès sans pertes.

## Coop, archonte et sauvegardes

Tous les postes doivent utiliser **1.0.546 / protocole 24** et le même mod. La coop suit la faction de l’hôte sur neuf étapes, jusqu’à quatre factions alliées. Archonte répartit jusqu’à huit joueurs entre elles. Les contrôleurs d’une maison partagent unités et crédits. Un emplacement allié fermé ne crée ni armée, ni MCV, ni renforts.

Chaque allié actif reçoit le capital de départ de l’hôte à chaque nouvelle mission. Le mode facile ajoute 500 crédits et commence à l’étape 2. Contrôles alliés et construction sur terrain allié sont distincts. Les forces ennemies +5/+10 se cumulent à +15 par adversaire présent. L’intro Rebels de Tornie donne une économie commerciale aux partenaires ; les adversaires Tornie gardent leur identité si un invité choisit la même maison.

**Charger une sauvegarde**, dans le lobby hôte, propose les manuelles `mpsave/*.dls` et automatiques `coop/*.dls`. Les invités rejoignent le lobby rouvert. Garder ensemble `coop/<session>.ini` et `coop/<session>.dls`. La version écrit le format **9839** ; anciens lecteurs et état des missions enregistrées sont conservés. Garder le profil local, dont `achievements.ini`, lors d’un transfert.

## Éditeur et succès

L’ouverture de l’éditeur distingue les campagnes solo/coop du mod actif et de Vanilla, dont 198 scénarios solo. Enregistrer une copie personnelle pour les modifier ; les propriétaires chargés restent sélectionnables en Lite. Le CHOAM de l’éditeur défile sur 31 unités, avec annuler/rétablir et rechargement.

Le catalogue comporte **73 hauts faits locaux**, avec statistiques et succès acquis conservés. Les succès de sauvetage coop sont attribués à la victoire de l’équipe ; le sabotage exige un palais ennemi détruit par un Saboteur du joueur. Replays, triche et immortalité solo n’accordent pas de progression.

Les contrôles automatisés Windows/Linux et du site passent. L’équilibrage après des campagnes complètes, la coop Internet humaine pour la 546 et le PC Mint physique restent à confirmer. Voir le [suivi actuel](docs/SUIVI-1.0.546-FR.md).
