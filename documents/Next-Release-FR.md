# 1.0.545 — préparation depuis la 1.0.544 corrigée

**Non publiée ; ajouts du 20 octobre en cours de validation.** Les téléchargements
publics restent la 1.0.544 corrigée (`d6a29fd`), protocole 20, sauvegardes 9838.
Cette branche de préparation utilise le protocole 21 et écrit des sauvegardes
9839 ; elle lit les anciennes sauvegardes. Aucun ID d'unité n'est déplacé.

- Raider Quad : les huit orientations sont ancrées sur les silhouettes du Quad
  vanilla, sans décalage aux zooms 1×/2×/3×. Les PNG fournis sont conservés.
- Éditeur : aucune étoile bleue sur le Slowpost, dans la palette ou sur le terrain.
  Les étoiles des unités personnalisées restent présentes.
- Éolienne Rebels standard 2×2 hors Vanilla : case « Améliorée au départ » dans
  les détails de l'éditeur. Carte, annuler/rétablir, suppression et sauvegarde
  conservent ce réglage ; double production et animation verte dès le lancement,
  sans achat initial ni IX nécessaire pour cette propriété de carte. Les autres
  éoliennes et les éoliennes avancées ne reçoivent pas cette case.
- Chaos : l'éolienne 2×2 entre dans l'échange des plans. Des plans Rebels peuvent
  donner son amélioration à une autre maison. Les conversions Scoutpost vers
  Flamepost, Chemipost et Slowpost suivent leurs plans empruntés, éditions Lite
  comprises, et gardent le bon donneur après conversion/reprise. Les éoliennes
  avancées restent en dehors de cet échange. Les anciens tirages à 15 bâtiments
  sont relus sans modifier leurs donneurs.
- Radar : deux recherches communes et une spécialisation par maison, dans les
  six mods hors Vanilla, avec interface, commandes réseau et sauvegarde.
  Communes : 500 crédits, 45 s, niveau 4 ; spécialisation : 750, 60 s, niveau 7
  + IX. Bonus permanents pour la partie, unités présentes et futures ; détail
  des douze maisons et des listes d'unités dans la
  [spécification FR/EN](../docs/RADAR-RESEARCH-NEXT-RELEASE.md).
- Introduction : animation Westwood Studios bleue, palette originale,
  fondus/cadence d'origine et signature AdLib DUNE0 piste 4. Les réglages de
  volume et de silence sont respectés ; le son s'arrête au saut de la scène.
- Après la finale de campagne : générique Dune II avec textes localisés,
  vrais noms, polices originales, onze illustrations et musique DUNE20 piste 2.
  Les attributions existantes du projet sont conservées.
- Campagnes : séquences intermédiaires et finales complètes pour les maisons
  ajoutées. Le modèle de scène et ses intervenants suivent CampaignPlan.ini,
  y compris en Lite ; noms, sous-titres et couleurs suivent ces rôles. La planète
  conquise et le générique utilisent la couleur choisie, couleurs bonus comprises.

Les essais automatisés de son vérifient pistes décodées, canal, volume et arrêt ;
ils ne remplacent pas une écoute humaine. Le rendu SDL et les commandes/sauvegardes
sont vérifiés par les contrôles natifs. Une partie humaine Internet entre réseaux
indépendants reste distincte des tests UDP et de déterminisme. Les demandes
concernent le jeu PC : « mobile » désignait une autre conversation Codex Work.

Validation du lot précédent, avant les ajouts ci-dessous (20 octobre 2026) : source moteur `fbe8b46270efc7635b9f3cc7446df90a3356e9c2`.
CTest **2/2**, **143 cas / 10 868 assertions**, dont UDP réel avec liens directs
entre invités bloqués et transport des commandes radar jusqu'à huit joueurs.
Contrôles natifs : **1 606 scénarios**, succès, sept modes de règles/éditeur/Chaos,
recherches pour 55 cas de maisons, **63 campagnes et trois cinématiques par langue**
FR/EN. Navigateur : **120 parcours/contextes** FR/EN ordinateur/mobile ; **161 PNG**
du site récupéré conservés à l'identique.

[CI Linux/Mint](https://github.com/Torneko/dunelegacy-tornie/actions/runs/39468276567) et [CI Windows](https://github.com/Torneko/dunelegacy-tornie/actions/runs/39468271577) réussies sur cette même source,
avec tests du jeu installé, radar et cinématiques FR/EN. Publication des paquets
désactivée. [Rapport de validation](Validation-Next-Release.json). Les vérifications
audio sont programmatiques ; écoute humaine, PC Mint physique et partie Internet
humaine entre réseaux indépendants restent des contrôles distincts.

## Ajouts du 20 octobre — 1.0.545

- Portraits fournis : Fremen seul et groupe de cinq ; Sardaukar groupes de trois
  et cinq. Originaux conservés, sans étoile ni nombre superposé. Les portraits
  de production suivent la maison des plans, y compris en Chaos.
- Douze cadres dorés pour les choix et confirmations de campagne. Le cadre
  entier reste visible, avec la bonne identité après un changement de mod ;
  les bannières ordinaires des escarmouches restent disponibles.
- Bouton **Rejouer l’intro**, à droite du menu principal, au-dessus de la coop.
  Même introduction que celle du démarrage, avec retour au menu et respect
  des réglages audio.
- Trois nouveaux succès, soit **73** : être éliminé en coop et gagner grâce
  à un allié survivant ; survivre et gagner pour au moins un allié éliminé ;
  détruire un palais ennemi avec un Saboteur. Statistiques associées. Les succès
  coop couvrent deux, trois et quatre factions alliées, depuis chaque poste ;
  les contrôleurs archontes partagent l’état de leur maison. Abandon ou
  déconnexion ne suffit pas.
- Site FR/EN de préparation : ressources et succès annoncés séparément des
  téléchargements 544. Bannière Fremen de Tornie recadrée à l’affichage,
  original PNG inchangé.

La validation Windows/Linux ci-dessus concerne le lot précédent, pas ces ajouts.
Le [suivi 1.0.545](../docs/RELEASE-1.0.545.md) précise leur état. Aucun tag,
paquet public ni déploiement du site n’est créé à cette étape.
