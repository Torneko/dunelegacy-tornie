# Dune Legacy Tornie 1.0.540 — lancement Linux Mint

Le démarrage de l’AppImage statique exigeait à tort les bibliothèques SDL2_mixer et SDL2_ttf partagées sur le PC. Sur un système sans ces paquets, le jeu quittait avant toute fenêtre. Ce contrôle redondant est supprimé : les dépendances réellement liées sont déjà vérifiées par le chargeur système.

La 1.0.539 ne fournissait que l’AppImage standard et avait perdu le lanceur de l’ancienne édition compatible Mint. Cette version rétablit **DuneLegacyTornie-1.0.540-Linux-Mint-Compatible.AppImage** : X11, rendu logiciel, son PulseAudio et réutilisation du profil Mint précédent (sauvegardes et hauts faits conservés). Un profil existant n’est pas réécrit.

L’AppImage standard reconnaît également Linux Mint. Le moteur respecte le choix explicite du rendu logiciel et l’utilise automatiquement si le rendu accéléré est indisponible. Les journaux du lanceur indiquent les pilotes et le code de sortie ; leur chemin est affiché lors du lancement dans un terminal.

La CI vérifie désormais le **menu réellement dessiné**, après initialisation des ressources, sur les deux AppImages en français et en anglais, avec PulseAudio sous Xvfb. Un test distinct vérifie le repli logiciel sans pilote graphique accéléré. Les tests moteur et le test UDP à trois participants restent exécutés. Ces vérifications ne remplacent pas l’essai sur le PC Mint 22.3 / noyau 6.17 signalé.

Le relais multijoueur de 1.0.539 est conservé, sans VPN. Protocole **10**, sauvegardes **9831**, mêmes règles de jeu et mêmes ressources. Utilisez la même version et le même mod sur tous les participants. Si FUSE n’est pas disponible : `./DuneLegacyTornie-1.0.540-Linux-Mint-Compatible.AppImage --appimage-extract-and-run`.
