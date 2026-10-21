# Une seule branche

Le projet utilise uniquement `dunelegacy-tornie`, à la demande de sa propriétaire.
Le code du jeu, les mods, les rapports et le site y sont réunis.

- Modifier le jeu et sa documentation directement sur cette branche.
- Le site français/anglais est dans `website/`. Le workflow `website.yml` le vérifie
  et le déploie par GitHub Actions, à la même adresse publique.
- Les workflows de construction et de publication restent disponibles depuis
  cette branche. `linux-release.yml` prend le site dans `website/` par défaut ;
  `website_ref` permet aussi de sélectionner un ancien instantané archivé.
- Conserver les tags de release existants : le regroupement ne change pas les
  sources ni les téléchargements déjà publiés de la 1.0.545.

Les derniers commits des anciennes branches sont préservés par des tags
`archive/branches/2026-10-20/...`. L’inventaire exact figure dans
[BRANCH-ARCHIVES.json](BRANCH-ARCHIVES.json). Les anciens essais et diagnostics
restent consultables sans recréer de branche.

Le site a été importé sans modification depuis
`68768fb040375076ae9ca72c083f76df2b97eeb2`, et son historique est un parent du
commit de regroupement. Le code et les reçus proviennent de
`72eb0f07a39167123b95407c9d4edf0d0b35a3a5`, descendant de l’ancienne branche
principale `0274018acd8d78b9b54cc4263116990773c0478f`.
