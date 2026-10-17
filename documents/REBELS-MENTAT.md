# Mentat chat Rebels / Rebels cat mentat — 1.0.541

Rebels reprend les trois PNG originaux du chat Wildspade/Jericho fourni par Tornie. Les fichiers restent identiques ; ils sont également installés comme ressources globales pour Vanilla. Les éditions Lite ne proposent pas Rebels. L’attribution et les [ancrages Wildspade](WILDSPADE-MENTAT.md) sont conservés.

Le moteur applique `Faction Tint = true` au portrait et aux atlas personnalisés des yeux/bouche de Rebels. Il résout d’abord l’identité de la faction, notamment les anciens emplacements de Jericho, puis lit sa couleur par défaut dans la palette active. La fourrure et le vêtement magenta suivent cette teinte ; la transparence, les iris verts, les détails dorés et le décor gardent leurs valeurs. Le repos utilise le fond déjà teinté ; l’épaule est extraite de ce même fond après coloration.

- Vanilla, Jericho et ArcaKev : gris foncé.
- Tornie : vert Rebels.
- Cinq états de regard et de parole ; mêmes positions, dimensions et chronologie que le chat existant.

Les tests capturent le vrai BriefingMenu de chaque faction, y compris la résolution d’identité de Jericho. Ils vérifient le fond, les iris, les dimensions des cinq états, le nez fixe, l’épaule superposée à la vidéo et le démarrage de la parole. Wildspade, Neutral, Paul et le husky Kleshmersh restent couverts par leurs tests existants.

Rebels reuses Tornie’s original Wildspade/Jericho cat PNGs and animation anchors without repainting the files. Runtime faction tint follows the active mod’s default Rebels palette: dark grey in Vanilla, Jericho and ArcaKev; green in Tornie. The background, fitted gaze/speech atlases and shoulder use the same tint while preserving alpha, green irises, gold details and the room. All five animation states and actual faction briefings are checked. Lite editions do not offer Rebels.
