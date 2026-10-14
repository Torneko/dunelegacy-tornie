# Dune Legacy Tornie 1.0.538 — essai rétro Linux Mint 22.3

**Statut : candidat expérimental, non validé sur le portable ciblé.**
Ordinateur visé : Intel Core i5 (vers 2013), NVIDIA GeForce GT 540M,
8 Go de RAM, Linux Mint 22.3, noyau 6.17.x.
Cette construction **n'est pas une preuve de compatibilité** : le pilote Nouveau,
la carte graphique hybride, l'audio et les bibliothèques système peuvent
encore empêcher le démarrage.

## Correctif du 14 octobre 2026 : bibliothèques SDL2 faussement manquantes

Le journal de test du PC indiquait :
`Required library missing or incompatible: libSDL2_mixer-2.0.so.0`
et `libSDL2_ttf-2.0.so.0`.
Le contrôle du moteur appelait `dlopen()` sur des bibliothèques partagées,
alors que vcpkg peut fournir SDL2_mixer/SDL2_ttf liés **statiquement**.
Cela peut produire un faux diagnostic de bibliothèques manquantes.

Cette branche retire ce précontrôle redondant (les dépendances dynamiques
réellement indispensables sont déjà chargées par l'éditeur de liens Linux),
ajoute `usr/lib/dunelegacy` au chemin du lanceur, et demande à la CI de
contrôler les dépendances ELF de l'exécutable effectivement extrait de
l'AppImage (`ldd`). La révision candidate antérieure doit être remplacée
par **l'artefact du nouveau workflow**. Le jeu reste à retester sur le PC.

Pour essayer uniquement l'ancienne AppImage en attendant le correctif,
sur Mint 22.3 il est possible d'installer les versions partagées avec :
`sudo apt install libsdl2-mixer-2.0-0 libsdl2-ttf-2.0-0`.
Cela contourne le diagnostic, mais ne confirme pas que le reste du jeu
fonctionne. Cette installation ne doit plus être nécessaire si les
bibliothèques utilisées par l'AppImage sont correctement liées/embarquées.

## Pourquoi un binaire distinct ?

La version 1.0.538 d'origine demandait systématiquement à SDL2 un
renderer avec `SDL_RENDERER_ACCELERATED`, et le moteur remettait à zéro son
choix de renderer sur Linux. Régler uniquement `SDL_RENDER_DRIVER=software`
dans un script ne suffisait donc pas à sélectionner fiablement le mode logiciel.

Cette branche ajoute une sélection explicite du renderer logiciel **avec
prise en charge des textures cibles** uniquement lorsque
`DUNELEGACY_RETRO_SOFTWARE=1`. Le renderer accéléré reste utilisé par les
paquets classiques ; en cas d'échec sur Linux, le moteur tente désormais le
renderer logiciel. Aucun changement de campagne, sauvegarde, réseau ou
équilibrage n'est voulu.

## Construire le candidat

Le workflow `.github/workflows/linux-retro.yml` utilise Ubuntu 22.04
pour garder une base glibc ancienne, construit Dune Legacy avec vcpkg,
utilise linuxdeploy et un AppRun personnalisé et vérifie le contenu
de l'AppImage avant de mettre le candidat dans les artefacts de GitHub Actions.
Le fichier doit s'appeler :

`DuneLegacyTornie-1.0.538-Linux-Mint-22.3-Retro-x86_64.AppImage`

La publication sur la release stable est **désactivée par défaut** :
elle nécessite l'exécution manuelle du workflow avec
`publish_release=true`, après vérification sur le PC.

## Premier essai sur Linux Mint

```bash
chmod +x DuneLegacyTornie-1.0.538-Linux-Mint-22.3-Retro-x86_64.AppImage
./DuneLegacyTornie-1.0.538-Linux-Mint-22.3-Retro-x86_64.AppImage
```

L'application démarre en fenêtre, utilise SDL2 en **rendu logiciel** et
privilégie X11 lorsque DISPLAY existe. Elle garde le pilote audio normal
sauf si celui-ci est explicitement neutralisé. Le journal est créé dans
`~/.local/state/DuneLegacyTornie/retro-*.log` (ou dans
`$XDG_STATE_HOME/DuneLegacyTornie/`).

Si le jeu se ferme après un clic ou lors de l'initialisation du son,
tester **temporairement sans audio** :

```bash
DUNELEGACY_RETRO_NO_AUDIO=1 ./DuneLegacyTornie-1.0.538-Linux-Mint-22.3-Retro-x86_64.AppImage
```

Si le renderer logiciel est incapable d'afficher les textures, comparer
avec le chemin accéléré (diagnostic, pas une correction garantie) :

```bash
DUNELEGACY_RETRO_SOFTWARE=0 ./DuneLegacyTornie-1.0.538-Linux-Mint-22.3-Retro-x86_64.AppImage
```

Si le runtime AppImage réclame FUSE, son contenu peut être extrait **sans FUSE** :

```bash
./DuneLegacyTornie-1.0.538-Linux-Mint-22.3-Retro-x86_64.AppImage --appimage-extract
./squashfs-root/AppRun
```

## Diagnostic de la machine (sans modifier les pilotes)

```bash
uname -r
inxi -Gxx
free -h
```

Ne pas imposer l'ancien pilote NVIDIA 390 à Mint 22.3 / noyau 6.17 ;
utiliser la configuration graphique opérationnelle et vérifier
l'accélération du pilote Nouveau / PRIME. Si aucun des modes ne démarre,
récupérer les dernières lignes du journal `retro-*.log` et préciser si
l'échec survient à l'ouverture, au premier clic ou au lancement d'une carte.

## Validation avant publication

- Démarrage du menu et changement entre Vanilla, Tornie, Tornie Lite,
  Jericho, Jericho Lite
- Chargement d'une carte, déplacement et zoom, puis sortie propre
- Démarrage avec audio normal, puis comparaison avec le mode audio neutralisé
- Sauvegarde/chargement d'une partie et, si nécessaire, essai coopératif
- Résultat des commandes `inxi -Gxx` et `uname -r`

**Limites** : une AppImage ne remplace pas le pilote du noyau. Cette variante
ne règle pas automatiquement un conflit Nouveau/NVIDIA, un problème SDL
de son, une bibliothèque système ou un problème de données du jeu.
