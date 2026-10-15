# Dune Legacy Tornie 1.0.539 — multijoueur Internet

Publication Linux, protocole réseau **10**. Tous les participants doivent utiliser **1.0.539** et le même mod. Les exécutables 1.0.538/protocole 9 sont incompatibles en réseau. Le format de sauvegarde reste **9831** et les règles déterministes du jeu sont conservées.

- Le troisième joueur n'est plus expulsé lorsque l'hôte est joignable mais que les invités ne peuvent pas échanger directement. Après l'essai de connexion directe (cinq secondes), la session bascule automatiquement vers un relais via l'hôte, sans VPN ni serveur de relais supplémentaire.
- L'hôte transmet les messages, commandes et sélections avec l'identité de leur auteur. Une seule route est utilisée après la bascule. Les listes de joueurs, départs et reconnexions restent cohérents.
- Le lobby, les réglages d'équipe, le démarrage, les informations de mod et la négociation de configuration continuent de passer par l'hôte. Les commandes gardent leurs cycles et leur identifiant de partie ; les paquets d'une ancienne partie sont rejetés. Le budget de latence tient compte des deux trajets du relais.
- L'hôte doit rester joignable par chaque invité. Le relais ne résout pas un hôte inaccessible et s'arrête lorsque l'hôte quitte.

Vérifications : tests moteur CTest ; régression avec les vrais NetworkManager et sockets UDP à trois participants, liens entre invités ouverts puis filtrés ; réception du lobby, chat sans écho, vingt cycles de commandes de synchronisation, sélections, démarrage, anciennes époques, refus des enveloppes forgées, quatrième joueur, départ/reconnexion et perte de l'hôte. La CI Linux exécute ces contrôles avant de fabriquer DEB, RPM, TAR.GZ et AppImage (Ubuntu 22.04).

Ces tests automatisés utilisent le loopback et un filtrage des datagrammes ; une partie humaine entre trois accès Internet distincts reste à vérifier. Les tests historiques à deux PC concernent 1.0.538. Aucun changement d'unité, de bâtiment, de technologie, de faction ou de haut fait.
