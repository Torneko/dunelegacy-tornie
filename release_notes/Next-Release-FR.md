# Prochaine version — préparation depuis la 1.0.544 corrigée

**Non publiée ; numéro et date de publication à confirmer.** Les téléchargements
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
