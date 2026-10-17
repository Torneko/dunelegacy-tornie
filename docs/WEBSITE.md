# 1.0.543 — brouillon uniquement

Le site de préparation `docs/1.0.543-draft` est récupéré depuis la révision publiée 1.0.542 `33b9edb` (le ref distant récupéré pointait encore sur `f3c7e36`, plus ancien). Les pages de changements FR/EN décrivent la vente, les permissions alliées et le compteur optionnel ; les nouvelles unités et les cinq PNG reçus sont documentés avec leurs validations locales. Aucun déploiement Pages ni nouveau paquet hors ligne publié : conserver les liens publics 1.0.542 jusqu’à validation complète.

Source du jeu vérifiée : `61baef969125d567cc790f0039a9396ffe974786`. Brouillon FR/EN : `6b3b73ac2048e584ad0072a36fc20911adf6bbec`. Tests locaux : 136 cas / 10 314 assertions, CTest 2/2, sept modes, sept reprises de sauvegardes 1.0.541, six configurations de nouvelles unités, 36 rendus sans fond magenta et 42 parcours/fiches navigateur. Le Flame Quad coûte 300 contre 200 et inflige 12 dégâts par tir contre 7 pour le Quad, avec son effet de flamme. Voir [le rapport de préparation](../release_notes/Validation-1.0.543.json). Aucune CI de publication Linux/Windows déclenchée pour cette préparation.

# 1.0.542 — publication Linux, Windows et site FR/EN

La publication a été autorisée par Tornie après l’ajout du compteur de moissonneuses. Source figée et tag `v1.0.542` : `a514293f98ecffef57def43e1fc6a587f191910f`. Le site FR/EN publié est `33b9edb2631edcf77efe1d5d5b976fadaf2b4cb6` ; récupérer cette révision ou l’archive hors ligne avant la prochaine modification. Les 145 PNG/GIF préexistants sont inchangés, avec un aperçu Windtrap animé supplémentaire.

- Linux : https://github.com/Torneko/dunelegacy-tornie/actions/runs/39057204417 — succès, moteur/réseau, cinq paquets, démarrage FR/EN des deux AppImages avec PulseAudio, repli logiciel et fonctions coop/éditeur/Windtrap dans l’AppImage Mint.
- Windows : https://github.com/Torneko/dunelegacy-tornie/actions/runs/39057206612 — succès, moteur/réseau, sept modes et 1 606 scénarios dans le dossier installé en FR/EN, succès et démarrage des menus.
- Envoi des deux plateformes vérifiées : https://github.com/Torneko/dunelegacy-tornie/actions/runs/39059517108 — succès.
- Pages : https://github.com/Torneko/dunelegacy-tornie/actions/runs/39059647062 — succès.
- Site hors ligne, sources, notes, empreintes et contrôle navigateur : https://github.com/Torneko/dunelegacy-tornie/actions/runs/39059723542 — succès : 72 parcours navigateur publics/hors ligne FR/EN, ordinateur/mobile.

Les tests locaux passent : CTest 2/2, 135 cas / 10 197 assertions, 1 606 scénarios FR et succès, sept modes coop EN, sept modes pour les règles 1.0.542 et les vraies sauvegardes 1.0.541. Le compteur a été vérifié dans le rendu natif : 3 moissonneuses standard + 2 Rebels donnent 5, le total revient à 0 et reste visible si le watermark est masqué. Il lit seulement la maison locale ; les contrôleurs archontes d’une maison partagent ce total. Protocole 14, sauvegardes 9834, anciennes sauvegardes lisibles ; nouvelles palettes et valeurs par défaut pour les nouvelles parties, règles des anciennes sauvegardes conservées. La confirmation native sur Mint 22.3 / noyau 6.17 et la partie humaine entre connexions Internet indépendantes restent à effectuer.

L’audit final confirme les 16 fichiers publics, les empreintes SHA-256, la même source pour Windows/Linux et le tag, et la révision du site déployée.

Voir [la validation](../release_notes/Validation-1.0.542.json) et [l’audit des fichiers publiés](../release_notes/Final-Publication-Audit-1.0.542.json). La source du jeu reste figée ; les reçus de publication sont enregistrés ensuite sur la branche de préparation.

Les comptes rendus suivants sont historiques.

# Remplacement de la 1.0.541 — construction alliée, éditeur et chat Rebels

La 1.0.541 a été remplacée, sans créer de 542. Elle ajoute l’option d’hôte **Construction sur terrain allié**, corrige la sélection du propriétaire dans l’éditeur par identifiant stable et donne à Rebels le chat animé existant de Wildspade/Jericho, teinté selon la faction du mod. Protocole **13**, sauvegardes **9833** ; anciennes sauvegardes lisibles, option désactivée si absente, soldes conservés. Les PNG originaux restent identiques et la teinte est appliquée par le moteur.

Source figée du jeu et tag : `0bf497ede39d6ab48bb0ecc3bb6c84df98a7d319`. Site FR/EN : `f3c7e36b8cd4de20b843ea8ee0b8a66a00c8ffc0`, récupéré depuis le site publié `0e91f751a1d2946e97e4a22d937fd9fec63a50d1`. Récupérer cette nouvelle révision publiée ou son archive hors ligne avant la prochaine modification.

- Linux : https://github.com/Torneko/dunelegacy-tornie/actions/runs/38983299258 — succès, moteur/réseau, cinq paquets, menus FR/EN, repli logiciel et tests fonctionnels de l’AppImage Mint : coop étendue, propriétaires de l’éditeur et chat Rebels.
- Windows : https://github.com/Torneko/dunelegacy-tornie/actions/runs/38983297104 — succès, moteur/réseau, sept modes et 1 606 scénarios dans le dossier installé en FR/EN, hauts faits et menus.
- Publication des deux plateformes vérifiées : https://github.com/Torneko/dunelegacy-tornie/actions/runs/38985579833 — succès. Le run automatique redondant déclenché par le déplacement du tag a été annulé ; les artefacts validés sont publiés avec leur provenance.
- Pages : https://github.com/Torneko/dunelegacy-tornie/actions/runs/38985673832 — succès.
- Site hors ligne, sources, rapports et 72 parcours navigateur publics/hors ligne FR/EN, ordinateur/mobile : https://github.com/Torneko/dunelegacy-tornie/actions/runs/38985742973 — succès.
- Branche source : `fix/1.0.541-allied-building-editor-owner` ; les reçus sont ajoutés après la source figée, sans changement du moteur, puis proposés en PR vers `dunelegacy-tornie`.

CTest 2/2, 132 cas / 10 166 assertions ; relais UDP réel à huit participants dont les invités sont isolés. Les suites locales FR/EN chargent chacune 1 606 scénarios et vérifient les hauts faits. La construction alliée est vérifiée depuis les quatre perspectives, en normal/facile et archonte : propriété, dépenses, terrain, ennemis, transitions, sauvegardes et lobby hôte/invité. L’éditeur vérifie les unités et bâtiments de toutes les maisons disponibles, les joueurs nommés/numérotés, les maisons inactives et le rechargement, dans les sept modes. Le chat Rebels est vérifié dans les vrais briefings des quatre modes : teinte, iris, transparence, décor, nez/épaule et cinq états ; Neutral et le husky restent couverts.

Le site conserve les sept modes, les 67 hauts faits et les 124 PNG de galerie. Les 137 images préexistantes sont inchangées ; huit nouveaux aperçus viennent du rendu réel. Les 36 parcours, 28 contextes précédents et 28 contextes des nouvelles fonctions passent localement dans les deux langues, sur ordinateur/mobile. L’audit final compare les 16 fichiers publics et les 15 empreintes du manifeste, tous les fichiers source de l’archive, les 272 fichiers du site hors ligne et les sept paquets/sources intégrés. Les 5 467 fichiers de mods et sept PNG globaux correspondent à la source dans les deux AppImages et le ZIP Windows. Voir [l’audit final](../release_notes/Final-Publication-Audit-1.0.541.json). Les rapports du remplacement précédent sont conservés avec le suffixe `-coop-funds-r2` et ceux de la publication initiale avec `-initial`.

Linux and Windows replacement 1.0.541, both website languages and the matching offline package are published. All linked CI runs succeeded. Download the replacement on every computer (protocol 13); older saves keep their recorded balances and default shared construction off. Native Mint 22.3/kernel 6.17 and a human match across independent Internet connections remain pending.

Les comptes rendus suivants sont historiques et décrivent les remplacements précédents.

# Remplacement de la 1.0.541 — crédits coop et couleurs ArcaKev

Correctifs de même version demandés par Tornie : capital initial de l’hôte pour chaque faction alliée active, bonus facile appliqué ensuite une fois, Rebels gris foncé et Corruptique jaune dans ArcaKev sans modifier les bannières. Protocole 12, sauvegardes 9832. Retélécharger les paquets sur tous les postes ; les chargements conservent les soldes enregistrés.

La **1.0.541 a été remplacée**, sans créer de 542. Source du jeu et tag existant : `78866fc137383badd4412aded3bc18d6c2f5037c`. Site FR/EN : `0e91f751a1d2946e97e4a22d937fd9fec63a50d1`, préparé depuis le site publié `f835c84832e67a23afa5f06eef1ffa56029e3336`.

- Linux : https://github.com/Torneko/dunelegacy-tornie/actions/runs/38943059028 — succès, moteur/réseau, cinq paquets, menus FR/EN et repli logiciel.
- Windows : https://github.com/Torneko/dunelegacy-tornie/actions/runs/38943055269 — succès, moteur/réseau, sept mods et 1 606 scénarios dans le dossier installé en FR/EN, hauts faits et menus.
- Publication des artefacts vérifiés des deux plateformes : https://github.com/Torneko/dunelegacy-tornie/actions/runs/38945710872 — succès. Le run automatique redondant déclenché par le déplacement du tag a été annulé pour préserver ces paquets et leur provenance.
- Pages : https://github.com/Torneko/dunelegacy-tornie/actions/runs/38945923716 — succès.
- Site hors ligne, sources, rapports et 72 parcours navigateur publics/hors ligne FR/EN, ordinateur/mobile : https://github.com/Torneko/dunelegacy-tornie/actions/runs/38946075656 — succès.
- PR source : https://github.com/Torneko/dunelegacy-tornie/pull/14 ; le run complet https://github.com/Torneko/dunelegacy-tornie/actions/runs/38943143490 a réussi, y compris le job Linux avec la dépendance miniupnpc système et la compilation Windows.

Les 5 467 fichiers de mods correspondent aux sources dans les deux AppImages et le ZIP Windows. Les bannières et PNG du husky sont inchangés. L’AppImage Mint publiée passe aussi les départs coop étendus dans les sept mods. Les 15 empreintes du manifeste correspondent aux digests GitHub des 16 fichiers publics ; les 257 fichiers du site et les sept paquets/sources intégrés au ZIP local sont identiques au site publié et à la release. Voir [l’audit final](../release_notes/Final-Publication-Audit-1.0.541.json). Les rapports de la publication initiale sont conservés avec le suffixe `-initial`.

Les contrôles locaux passent dans les deux langues : CTest 2/2, 130 cas / 10 138 assertions, 1 606 scénarios par langue, crédits normaux/faciles des quatre alliés, budgets optionnels à zéro ou absents, validation et sérialisation du lobby, sauvegardes, couleurs et rechargement de l’éditeur. Le site garde les sept modes, les 67 hauts faits et les 124 PNG ; 36 parcours et 28 contextes de fonctionnalités sont vérifiés localement. Mint natif 22.3/noyau 6.17 et une partie humaine sur plusieurs accès Internet restent à confirmer.

Linux and Windows replacement 1.0.541, both website languages and the matching offline package are published. Every CI run linked above succeeded. Download the replacement on every computer (protocol 12); saves retain recorded balances. Recover the published `origin/gh-pages` revision `0e91f75` or the replaced 1.0.541 offline archive for the next update.

Le compte rendu ci-dessous décrit la publication initiale de 1.0.541 (protocole 11), avant ce remplacement.

# Publication 1.0.541 — 2026-10-16

La [release Linux/Windows 1.0.541](https://github.com/Torneko/dunelegacy-tornie/releases/tag/v1.0.541) est publiée. Source immuable : `1ebb5ec34c5ffd791b0fdde6fce951627d09bcc1`. Site français/anglais : `f835c84832e67a23afa5f06eef1ffa56029e3336`, récupéré depuis le site 1.0.540 `fabd49f` avant modification. Le paquet `DuneLegacyTornie-Site-Local-1.0.541.zip` reprend exactement les 252 fichiers du site publié, avec les paquets Linux/Windows et les sources de cette release.

- Linux, tests moteur/réseau, deux AppImages et menus FR/EN avec PulseAudio : https://github.com/Torneko/dunelegacy-tornie/actions/runs/38928318200 — succès.
- Windows, moteur/réseau, 1 606 scénarios et hauts faits dans les deux langues du dossier installé, menus FR/EN : https://github.com/Torneko/dunelegacy-tornie/actions/runs/38928313974 — succès.
- Déploiement Pages : https://github.com/Torneko/dunelegacy-tornie/actions/runs/38932048127 — succès.
- Site hors ligne, sources, rapports, sommes de contrôle et 72 parcours réels dans un navigateur, publics/hors ligne, FR/EN et ordinateur/mobile : https://github.com/Torneko/dunelegacy-tornie/actions/runs/38932158226 — succès.
- Contrôles locaux : 130 cas moteur / 10 138 assertions, CTest 2/2, 1 606 scénarios par langue sur le code final et 28 contextes de fonctionnalités du site. Le réseau UDP réel vérifie huit participants aux communications directes entre invités bloquées, les identités et cycles de commandes, le lobby et le démarrage via l’hôte.
- Les 5 467 fichiers de mods correspondent exactement aux sources dans le ZIP Windows et les deux AppImages ; les PNG du husky et la bannière orange sont conservés. Les sept paquets/sources intégrés au site local ont les mêmes SHA-256 que la release. Les 15 entrées du manifeste correspondent aux empreintes GitHub ; 16 fichiers sont publics.

Le site documente les sept modes, l’ordre ArcaKev, les campagnes et technologies, la coop à quatre factions et l’archonte à huit contrôleurs, les pouvoirs et recharges des palais, le husky fourni, les 67 hauts faits et les 124 PNG. Les guides d’installation, du mode facile et de la coop sont synchronisés en français/anglais. Tous les participants doivent utiliser 1.0.541 / protocole 11 et le même mod ; nouvelles sauvegardes 9832, anciennes sauvegardes lisibles. Mint 22.3/noyau 6.17 et une partie humaine sur des accès Internet indépendants restent à confirmer.

Linux and Windows 1.0.541, both website languages and the matching offline package are published. All four CI runs above succeeded; 72 offline/live browser routes, 28 local feature contexts, exact package payloads and all 15 manifest digests were verified. The supplied animated Kleshmersh husky is included. Native Mint and human Internet playtesting remain pending. Recover this `origin/gh-pages` revision or the 1.0.541 offline archive for the next update on either PC. Reports are attached to the release and retained under `release_notes/`.

# Windows 1.0.540 published — 2026-10-15

The Windows 64-bit ZIP is now attached to the same 1.0.540 release as Linux. It uses the immutable game source `17c7d375bfdfe0204acd110bea57409e9bbe4896`, protocol 10 and save format 9831, including the host relay for isolated guests. Extract the entire ZIP into a new folder and launch `dunelegacy.exe`. All players need the same version and mod.

- Windows build, engine/network tests, installed FR/EN mod/achievement checks and actual menu startup: https://github.com/Torneko/dunelegacy-tornie/actions/runs/38789679153 — success.
- Public Windows ZIP SHA-256: `807e6aaae7f6632ebd2a46675618cf71b16c58324cf3122e377cbb62984307e1`. All 3,177 mod files match the tagged source exactly; the download returns HTTP 200.
- FR/EN site recovered from `afb742b` and published at `fabd49f`: https://github.com/Torneko/dunelegacy-tornie/actions/runs/38792866861 — success.
- Matching offline package with Windows 1.0.540, notes, reports and checksums: https://github.com/Torneko/dunelegacy-tornie/actions/runs/38792988463 — success; 72 offline/live FR/EN desktop/mobile browser checks passed, including the current Windows links.
- All 15 checksum entries match GitHub's public asset digests. Sixteen release assets are public. The five Linux package digests are unchanged.

Windows 1.0.538 remains historical and cannot join protocol 10 sessions. Three independent human Internet connections and native Mint 22.3/kernel 6.17 still await user confirmation. Recover this published site or the refreshed 1.0.540 offline ZIP for the next update.

# Initial Linux publication 1.0.540 — 2026-10-15

Mint AppImage startup is corrected: the static executable no longer demands shared SDL mixer/TTF copies on the host. The Mint launcher, previous Mint profile, X11/software/PulseAudio and an automatic software fallback are included. The actual published Mint image passed a full menu startup without development-library paths, the UDP regression and an exact comparison of all 3,177 mod files.

- Release: https://github.com/Torneko/dunelegacy-tornie/releases/tag/v1.0.540
- Immutable game source: `17c7d375bfdfe0204acd110bea57409e9bbe4896`.
- Successful package CI, including 122 engine cases, network regression and both AppImages' FR/EN menus with PulseAudio: https://github.com/Torneko/dunelegacy-tornie/actions/runs/38695978990
- Published FR/EN site: `afb742b`, based on the recovered 1.0.539 tree `472b875`; Pages https://github.com/Torneko/dunelegacy-tornie/actions/runs/38696414786 — success.
- Offline site, sources, checksums and 72 offline/live browser checks: https://github.com/Torneko/dunelegacy-tornie/actions/runs/38696548593 — success.
- All thirteen manifest entries match GitHub SHA-256 digests. Fourteen release assets are public.

Use `DuneLegacyTornie-1.0.540-Linux-Mint-Compatible.AppImage` on the affected Mint PC. Native Mint 22.3/kernel 6.17 still awaits user confirmation. Protocol 10, host relay, save format 9831, unit/building rules, factions, achievements and 121 gallery PNGs are retained. Recover this published gh-pages tree or the 1.0.540 offline ZIP for the next update.

# 1.0.540 — Linux Mint startup fix

Recover the current published `origin/gh-pages` tree at `472b875e13fd971447e840f5aca2bf040878f6cd` before this update. Update both languages to 1.0.540 and point the Mint download to `DuneLegacyTornie-1.0.540-Linux-Mint-Compatible.AppImage`. Document X11/software/PulseAudio, preservation of the previous Mint profile, startup logs and automatic renderer fallback. Keep protocol 10, save format 9831, unit/building routes, factions, achievements and original PNGs. Graphical startup is tested with the packaged AppImages in French/English under Xvfb; native Mint 22.3/kernel 6.17 remains for the affected PC to confirm.

# Published 1.0.539 — 2026-10-15

- Linux release: https://github.com/Torneko/dunelegacy-tornie/releases/tag/v1.0.539
- Immutable tagged game source: `12d67c60b889102b419c762694bad6995e953fcb`.
- Linux build, 122 engine cases / 7,412 assertions and real UDP direct/relay regression:
  https://github.com/Torneko/dunelegacy-tornie/actions/runs/38685210586 — success.
- Published FR/EN Pages source: `472b875` (recovered from `973ba9c5` before editing).
  https://github.com/Torneko/dunelegacy-tornie/actions/runs/38686129594 — success.
- Offline site, tagged sources, notes, checksums and 72 real-browser offline/live
  FR/EN desktop/mobile route checks:
  https://github.com/Torneko/dunelegacy-tornie/actions/runs/38686221371 — success.
- The published AppImage passed the network regression again after extraction;
  all 3,177 bundled mod files exactly match the tagged source. Full local runtime
  verification loaded 1,210 scenarios. All eleven manifest digests match GitHub's
  asset digests. Twelve release assets are public.

All participants need 1.0.539 / protocol 10 and the same mod. Save format stays
9831. Windows 1.0.538 remains a clearly labelled previous download and cannot
join 1.0.539. Three independent human Internet connections and native Mint
22.3/kernel 7.0 remain untested. Recover this published gh-pages tree or
`DuneLegacyTornie-Site-Local-1.0.539.zip` for the next update.

# Release 1.0.539 — Internet host relay (Linux)

The 1.0.539 site is based on freshly recovered `origin/gh-pages` commit
`973ba9c5fdad2cf880e802e431ddbb0f2723aaaa`. Both languages document direct
connections followed by host relay when guests cannot communicate, protocol 10,
unchanged save format 9831, the requirement for a reachable host, and the scope
of the UDP regression. Historical two-PC confirmation remains attached to
1.0.538. Windows 1.0.538 downloads remain explicitly versioned and cannot join
1.0.539; Linux downloads use the new CI packages. Unit/building routes, factions,
achievements and the 121 original PNGs are retained. Release notes and the
matching offline website accompany the release.

## Published revision r8 — 2026-10-10

The existing 1.0.538 release now includes the accepted r6 painted-unit portrait
fix, r7 advanced-building editor arrangement and Tornie’s final Doublefinery
line correction. Native indexed PNG: 80×224 pixels, seven 80×32 frames, copied
byte-for-byte without resizing (SHA-256 `d30b94393b5f0f3b9a08a47677ae934285ea8f213016d4ac74aac71d176d572b`).
Protocol 9 and save format 9831 are unchanged. Update both co-op PCs.

- Tagged package source: `40923a6d7bcbd20c9f82759b1034a8eee2af4e73`.
- Published FR/EN Pages: `973ba9c5fdad2cf880e802e431ddbb0f2723aaaa`.
- Linux packages: https://github.com/Torneko/dunelegacy-tornie/actions/runs/38078571904 — success.
- Pages deployment: https://github.com/Torneko/dunelegacy-tornie/actions/runs/38079078502 — success.

All 30 public asset SHA-256 digests match the local publication files. All nine
package URLs returned HTTP 200. Each of the seven game/source archives contains
the exact 3,177 mod files; both PNG packs and the live gallery contain the final
sprite. The offline site embeds the exact published Windows, Mint and source
archives, plus the updated native editor preview. Browser checks passed: 104
local/file/HTTP desktop/mobile checks and 52 live checks across both languages.

The accepted r6/r7 engine and rendering reports remain historical evidence:
122 cases / 7,412 assertions per platform, separate portrait and editor captures.
The final public Windows payload supplied a fresh four-mod palette capture;
full game regression suites were not repeated after the final image correction.
Mint’s published profile, X11/software rendering and PulseAudio defaults are
retained. Native Mint 22.3/kernel 7.0 verification remains on the target PC.
Final publication/package reports are in `release_notes/Publication-Validation-1.0.538.json`
and the release assets. Recover this published gh-pages tree on either PC.

## Local editor arrangement r7 — 2026-10-10

The local preview `work/site-538-editor-r7/dist` extends the accepted local r6
portrait fix, based on published `origin/gh-pages` commit
`d0901d65dd9710de37fa51f95fb2ac6ddce43972` (fetched again before editing).
FR/EN project notes show an actual native-size editor capture of Tornie’s
requested advanced-building arrangement. All four mods use this layout;
Vanilla and gameplay rules are unchanged. Public release/site remain at r5.
New local Windows, Mint AppImage and source packages include both fixes.

## Local selected-unit portrait fix r6 — 2026-10-10

The local preview `work/site-538-portraits-r6/dist` is copied from the freshly
fetched published `origin/gh-pages` tree `d0901d65dd9710de37fa51f95fb2ac6ddce43972`.
Both languages explain restored illustrated selected-unit portraits for every
colour and separate infantry production variants. Public r5 downloads and
historical validation reports remain unchanged until publication.

Local r6 validation: Windows/Linux engine tests pass (122 cases, 7,412 assertions each). FR/EN portrait runtime checks pass with 21 colour slots, 28 painted portraits and separate production variants, including the final Mint AppImage under Ubuntu 22.04/WSL. The local FR/EN site passes 200 checks; native Mint 22.3/kernel 7.0 remains untested.

## Doublefinery artwork revision r5 — 2026-10-10

Publication verified: tagged source `d29b80cb5579d99c9cf40eb3bf9f2a86c9fc13be`, website `d0901d65dd9710de37fa51f95fb2ac6ddce43972`. The revised user-supplied sprite is byte-identical in all four mods, all game packages, both PNG archives and the live gallery. All 28 release asset digests match local files; all nine package URLs returned HTTP 200. Game regression suites were not rerun.

Linux packaging: https://github.com/Torneko/dunelegacy-tornie/actions/runs/38068492379 — success. Pages: https://github.com/Torneko/dunelegacy-tornie/actions/runs/38068770242 — success.

Artwork revision r5: the corrected Doublefinery sheet supplied on October 10 is copied byte-for-byte into all four mods. Native dimensions: 80 × 224 pixels (seven 80 × 32 frames), without resizing. The existing 1.0.538 packages and FR/EN website are refreshed with this image.

## Published 1.0.538 follow-up — 2026-10-10

Publication confirmed: release tag source ed14895dca2063dacb5ebcbcb98f67a81575f0c6, website 9af05f94036a1f4da958f9d291aea4c0b24514aa. All 28 release asset digests match the local publication files; all nine package download URLs returned HTTP 200. Local website checks: 104; live website checks: 52.

Linux packaging: https://github.com/Torneko/dunelegacy-tornie/actions/runs/38028622609 — success; compiled game code matches the tagged source (only the French guide changed afterward). Pages: https://github.com/Torneko/dunelegacy-tornie/actions/runs/38029333585 — success. Both public languages and the offline ZIP include the exact final sprite.

The existing 1.0.538 release and tag now include the Doublefinery follow-up (native
80×224 final sprite, network protocol 9, save format 9831). French/English Pages and
offline downloads are updated together from the recovered gh-pages baseline 2f6ee0b6.
Both co-op PCs need the replaced packages. Prior game validation is retained as
historical evidence; game suites were not rerun after the final sprite replacement.
Publication/package and website validation reports accompany the release.

## Published 1.0.538 baseline — 2026-10-09

The French/English website was updated from published `origin/gh-pages`
`ec822ddbb4a195c864d2658fb4dfe6b2b62b31d6`. Tornie confirmed the latest build and
two-PC co-op work and authorized publication. The public site, exact release
downloads and offline ZIP now identify 1.0.538.
The obsolete LOCAL labels in both HTML entries were removed after publication,
including the header, sidebar and search description. The corrected public site
and matching offline ZIP passed 16 additional desktop/mobile label checks.
Game packages and tagged source are unchanged.

- Release: https://github.com/Torneko/dunelegacy-tornie/releases/tag/v1.0.538
- Tagged/tested source: `c39a276e1bb4a8749b055d6283df3e067dec7e71`; merged through PR #10.
- Website: `2f6ee0b62a5d8b4bdf6fe9ed9cd440a848dde8ff` on `gh-pages`.
- Windows/Linux build, engine, installed FR/EN runtime and locale checks: https://github.com/Torneko/dunelegacy-tornie/actions/runs/37981715769 — success.
- GitHub Pages deployment: https://github.com/Torneko/dunelegacy-tornie/actions/runs/38005836548 — success.
- Browser checks: 180 local/file/HTTP desktop/mobile checks, 198 local feature checks, 60 live route checks and 66 live feature checks, with no browser errors.
- All twenty release downloads are publicly accessible and match GitHub SHA-256 digests. Four published guides and eight new PNGs were checked against the local site.
- Windows ZIP SHA-256: `dce9fc42e20178218d17bf46b161eac6cc7d1a4fddeabb6cdfe488478cd3fd80`.
- Offline website ZIP SHA-256: `4ed9d75ba7a8daab2144b3363afef46127b2e0f5f3bc6bd49c49c8c02257903d`.

Both languages cover optional allied Easy Mode (mission 2, +500 credits at that
mission start, -25 purchase prices), five optional extra units per present enemy
per mission, reinforcements for both allies and protocol 7. There are 67 achievements
and 119 gallery PNGs. Six supplied production portraits distinguish 1, 3 and 5
infantry in Barracks/WOR/Worfinery, with white price text, in Tornie, Jericho and
their Lite editions. Selected-unit portraits retain their existing images. The
Worfinery displays Harvester last while preserving production queues and saves.
Wildspade/Tornie and Rebels/Jericho use their supplied banners and updated colors.

The release uses the artifacts from the successful build above. The duplicate tag
build was cancelled after its version gate passed to preserve those audited files.
Standard Linux formats, a separate Mint compatibility AppImage, exact tagged
sources, bilingual notes, PNG pack and offline site accompany the release.
The compatibility AppImage passed FR/EN runtime and PulseAudio checks under
Ubuntu 22.04/WSL. Mint 22.3/kernel 7.0 still needs testing on the target PC.

Details and publication hashes are in `release_notes/Publication-Validation-1.0.538.json`
and the accompanying release reports. Use this published `origin/gh-pages` tree
or the matching 1.0.538 offline ZIP as the website baseline on either PC.

## Published 1.0.537 baseline — 2026-10-08

Recovered the current `origin/gh-pages` baseline at `a39cecfa` before updating the local bilingual site. Document allied control (default off), selectable enemy AI, an AI guest, guest intro WOR, enemy faction/color collision handling, revised in-game help and refined Wildspade rendering. Unit technology and prices, solo campaign maps and the 47-achievement catalogue are unchanged. New co-op layouts require a new campaign or next mission; checkpoints retain existing positions. Protocol 6 requires both players to use 1.0.537. Tornie has tried co-op on two PCs and confirmed lobby save loading. Extended playtesting and the Mint 22.3/kernel 7.0 compatibility check remain open. The French/English website and offline ZIP now identify 1.0.537 and embed the exact audited Windows release archive.

- Release: https://github.com/Torneko/dunelegacy-tornie/releases/tag/v1.0.537
- Tagged/tested source: `2e54898202b13bd238064d6920c8bbea5253342d`; merged through PR #9.
- Website: `ec822ddbb4a195c864d2658fb4dfe6b2b62b31d6` on `gh-pages`, recovered from the previous `a39cecfa` baseline.
- Windows/Linux build, engine and installed FR/EN runtime checks: https://github.com/Torneko/dunelegacy-tornie/actions/runs/37870615482 — success.
- GitHub Pages deployment: https://github.com/Torneko/dunelegacy-tornie/actions/runs/37872686746 — success.
- Browser checks: 180 local/file/HTTP desktop/mobile checks plus 60 live checks, including achievement search, filters and language switching.
- Windows ZIP SHA-256: `02d4c20cab6a4366773300326967240c279ece56c0526b2dc1b5268eb05ce9d9`.
- Offline website ZIP SHA-256: `dbb83ec7facd1b7ce68f67e23ad38190c3db7230d18c9419975dd7657fc3049e`.
- Standard Linux CI formats, a separately identified Mint compatibility AppImage, exact tagged sources, bilingual notes and 111 PNGs accompany the release.

The release uses the artifacts from the successful build above. Repeated merge/tag builds of the same code were cancelled after verifying the source tree and tag version, to preserve the audited archives. Publication details are in `release_notes/Publication-Validation-1.0.537.json`. Tornie confirmed lobby save loading works. Extended co-op playtesting and the friend's Mint 22.3/kernel 7.0 check remain open. Use this `origin/gh-pages` tree or the matching 1.0.537 offline ZIP as the website baseline on either PC.

# Documentation website maintenance

Tornie requests that every game update also update the French and English website. This applies to work from either PC.

## Release 1.0.536

The French/English 1.0.536 website was updated from the published `origin/gh-pages` 1.0.535 tree. It covers Easy Mode, Jericho Lite, revised intro opposition, Ornithopter behavior and Wildspade's price, darker mentat overlays, 47 achievements and the independent nine-mission cooperative campaign. Raider Trike and Rocket Trike prices use black numbers on their light portraits; added backgrounds/borders are removed for all prices and the other prices retain their usual color. Jericho Lite enables the four Jericho spice families by default with matching colors and effects; its six opening missions copy Tornie Lite except for their remapped opponents. New co-op campaigns follow the host faction's original campaign, with 405 variants from 45 sources, a nearby deployable MCV and matching starting forces for the guest, original opponents (up to five in Vanilla) and shared harvest objectives. Older sessions keep the 45 previous layouts. Guides retain identical faction choices, separate ownership/progress/checkpoints and protocol 5. Solo Commander/Chaos Conqueror awards are distinguished from faction victories that also support multiplayer.

The local/offline site passed 180 desktop/mobile browser checks across both languages, source files, the ZIP extraction and HTTP preview, with no JavaScript, missing-image or overflow failures. The release package is `DuneLegacyTornie-Site-Local-1.0.536.zip`. Published French/English entries and download links now identify 1.0.536. A human co-op match between two computers remains pending; the release includes co-op for playtesting. Publication details and package hashes are recorded below and in `release_notes/Publication-Validation-1.0.536.json`.

## Published 1.0.536 baseline — 2026-10-07

- Release: https://github.com/Torneko/dunelegacy-tornie/releases/tag/v1.0.536
- Tagged/tested source: `4c26d6cc6887ab9fd0735269366829b057a52a63`; merged into `dunelegacy-tornie` through PR #8.
- Website: `a39cecfa007131e107b184eec5092b84960599d8` on `gh-pages`.
- Windows/Linux build, engine, installed FR/EN runtime and locale checks: https://github.com/Torneko/dunelegacy-tornie/actions/runs/37697974701 — success.
- GitHub Pages deployment: https://github.com/Torneko/dunelegacy-tornie/actions/runs/37700593303 — success.
- Offline/site browser verification: 180 local/file/HTTP desktop/mobile checks and 24 live FR/EN checks, plus seven live download checks.
- Windows ZIP SHA-256: `18efee50d0de8e7817ea1ba19e1e01c2f97d959761e6694aa98a64e79f28c669`.
- Offline website ZIP SHA-256: `594cd784b6bdac5c2413b7e26725632cf69223c5f9aebb701f3082de76091729`.

The release uses the archives from the successful build above. The website and offline ZIP embed that exact Windows archive. Creating the tag started a second build of identical code; its version check passed, then the duplicate compilation/upload was cancelled to preserve these audited files. Source tags, release assets and their GitHub SHA-256 digests were checked. Reports accompany the release and are tracked under `release_notes/`.

A human two-computer co-op match remains pending, as accepted by Tornie for this publication. Both players need 1.0.536 and the same mod. The release documents co-op as available for playtesting.

Use this `origin/gh-pages` tree or the 1.0.536 offline package as the next website baseline on either PC.

## Current locations

Local follow-up to 1.0.538: the working preview is `work/site-538-followup/dist`
beside this source checkout. Its baseline is published `gh-pages` commit
`2f6ee0b6`. It documents the 5×2 Doublefinery, local protocol 9/save format 9831,
121 PNGs and the test downloads. This local patch has not been published.
Use the follow-up preview when preparing its eventual publication, and recheck
the public branch before merging website changes from another PC.

- Published site: https://torneko.github.io/dunelegacy-tornie/
- English entry: https://torneko.github.io/dunelegacy-tornie/index-en.html
- GitHub Pages source: the root of the `gh-pages` branch, separate from the game source branch `dunelegacy-tornie`.
- Offline package: `DuneLegacyTornie-Site-Local-<version>.zip` in the matching GitHub release. Its website files are under `DuneLegacyTornie-Site-Local/dist/`.

Before editing, fetch both branches and read the matching release notes and validation report. Preserve unrelated local changes. Retrieve the latest site from `origin/gh-pages` or the release package; previous PCs may retain older website generators outside this repository.

## Updating the content

Derive the documentation from the actual engine rules and active-mod configuration. Include special runtime gates, building upgrades, prerequisite chains and faction-specific overrides; copying `ObjectData.ini` alone is insufficient.

Update both entries and renderers (`index.html`, `index-en.html`, `assets/app.js`, `assets/app-en.js`), shared data (`assets/site-data.js`), English data (`assets/site-en-data.js`) and the accompanying downloadable documents as needed. Preserve the FR/EN switch and its selected page, faction and mod.

For each game update, revise the displayed version, release links, affected unit/building descriptions and technology progression. Keep the faction/campaign roster, achievement catalogue and gallery consistent with the game. Use the corresponding original game PNGs for icons, sprites and banners, retaining mod-specific variants. Achievement pages describe rules; they do not display invented personal unlock progress.

Keep the offline package functional through `file://`, including its local downloads and launchers. The published site uses public release/download links where appropriate. Preserve both French and English documents and asset catalogues.

## Verification and release

Check both languages in a real browser on desktop and mobile widths: navigation, affected fiches/technology routes, language switching, images, search/filter controls and downloads. Check offline access and browser errors. Validate that the displayed version and download targets match the release, and that the gallery images exist.

When releasing the game, include the updated offline site ZIP and publish the corresponding site through the configured `gh-pages` source. Verify the resulting Pages deployment and the live French and English entries. Report the checks actually performed and any unresolved game issues; a previous validation report does not validate a later build.

Historical synchronized release: **1.0.535**, tag `v1.0.535`, revised tagged game source `9ca89b3394a0190cf8db05c0538c771fa2e6e78c`; published website `66bcd8722e58cc757573c9220fb0dfb5a8d5d19c`. The existing 1.0.535 was replaced at Tornie's explicit request on 2026-10-06; no new version was created. The initial campaign rebuild was published through merged PRs #6 and #7; the revised opening balance is commit `9ca89b33`. Both public entries and the offline site ZIP match this revision: 30 unit entries, 29 buildings, 46 achievements and 111 PNGs.

The site documents all 24 rebuilt Tornie/Jericho campaigns and 528 scenarios, the balanced twelve-faction opponent plans and the corrected animated cat mentat. Every first mission now has two bonus Tanks relative to the original 1.0.534 forces, three Troopers squads (nine infantry) and two Special Unit Spawns. The earlier Wildspade support was replaced by this common policy. First-mission opposition is reduced to twelve enemies with Area Guard replacing initial Hunt orders; ambush encounters remain. Ordinary vehicles, terrain, economy and buildings are preserved; later mission forces are unchanged. See [campaign plans](CAMPAIGNS.md) and [mentat sources/prompts](WILDSPADE-MENTAT.md). Redownload 1.0.535 and start a new campaign for these forces. Existing saves remain readable with their stored maps. Vanilla and Tornie Lite campaign plans are preserved.

Chaos Mode and the Wildspade technology gates from 1.0.534 remain documented. The 2×3 and 3×2 advanced Windtrap entries use the original `Tornie_AdvancedWindtrap_icon.png` game portrait; sprites and editor previews remain in the gallery. The original Fremen house-confirmation banner is verified in the actual game rendering.

Release: https://github.com/Torneko/dunelegacy-tornie/releases/tag/v1.0.535. Downloads include `DuneLegacyTornie-1.0.535-Windows-x64.zip`, four Linux package formats, tagged sources, French/English notes, 111 PNGs and `DuneLegacyTornie-Site-Local-1.0.535.zip`. The offline website embeds the exact revised Windows CI release archive (SHA-256 `ae491377231f8fec51ac597a251883f64f984350d65afb39c320f64c16055175`). Website scripts and the Windows link carry the revision query `rev=9ca89b33` to refresh cached copies. Prior previews are retained locally for reference. Release 1.0.534 remains available separately.

Validation: revised release CI https://github.com/Torneko/dunelegacy-tornie/actions/runs/37529197044 succeeded; revised Pages deployment https://github.com/Torneko/dunelegacy-tornie/actions/runs/37531119434 succeeded. Windows/Linux engine tests pass: 97 cases and 6,867 assertions. The extracted revised Windows CI archive passed French/English runtime checks, each loading 838 scenarios, including all 528 rebuilt maps and a save from the original 1.0.535. Runtime checks verify all 24 first-mission policies: two bonus Tanks, nine actual Troopers, two resolved Special spawns and twelve enemies without an initial rush. All 2,206 mod files match tagged sources exactly in Windows and Linux payloads. A profile containing the original 1.0.535 managed mods was automatically reseeded to the revised maps despite the unchanged version; its personal campaign file and previous save bytes were preserved.

The published site passed 36 desktop/mobile routes, eight campaign views and twelve Windtrap portrait cases, in addition to 54 offline/local routes, eight affected fiches and 24 portrait checks. All 111 PNG hashes, five public website downloads and nineteen release asset hashes were verified. Reports attached to the release include `Validation-1.0.535.json`, `Campaign-Opening-Balance-1.0.535.json`, `Same-Version-Profile-Update-1.0.535.json` and `Publication-Validation-1.0.535.json`. Tornie authorized publication of this replacement; the new opening balance has automated checks and still benefits from human playtesting. Extended all-faction campaign balance and a live two-PC multiplayer match remain broader playtesting work.

For the next update on either PC, use this published `origin/gh-pages` tree or the matching offline ZIP as the website baseline. Fetch both source branches explicitly if the local clone has a single-branch fetch refspec; do not recover the site from an older preview generator. Historical campaign tooling intentionally uses immutable baseline commits: initial 1.0.535 source `fa2a5de3d23507f95332e243d060eeb419ce8fd5` and original 1.0.534. The published tag now points to `9ca89b33`.

Local artwork revision 538-followup-r4 uses Tornie’s final 80×224 Doublefinery sheet in all four mods and the FR/EN gallery/downloads. Gameplay, network protocol and save format are unchanged; remaining game tests were not rerun at Tornie’s request.
