# Correctifs de Dune Legacy Tornie 1.0.544 — préparation

Base : tag `v1.0.544` (`d979cea6ab8d2554ac7ab19d1444dcdf182ee245`). Ces correctifs concernent Tornie, Tornie Lite, Jericho, Jericho Lite, ArcaKev et ArcaKev Lite. Vanilla conserve ses règles et graphismes.

- **Slowpost** : retiré du chantier de construction. Les plans Neutral permettent de convertir un Scoutpost au niveau 7, avec IX, pour **450 crédits**. Le bouton indique le prix dans son infobulle. Les plans capturés ou empruntés en Chaos suivent la même règle. L’éditeur et les anciens postes directement construits restent pris en charge.
- **Animation** : les deux vues du Slowpost forment une boucle, indépendante de sa direction de visée. Le ciblage et les missiles ralentissants restent fonctionnels.
- **Ranged Tank** : violet plus sombre et plus saturé sur la tourelle ; le châssis conserve la couleur de la maison.
- **Worfinery** : les infobulles lisent le même ordre que les portraits et les clics. La moissonneuse reste en dernier ; le nom survolé correspond à l’unité affichée, y compris après défilement et rechargement.
- **Moissonneuses spécialisées** : les deux montages sont reconstruits en mémoire depuis la moissonneuse vanilla. Grande capacité : moitié avant violette, moitié arrière aux couleurs de la maison. Rapide : moitié avant aux couleurs de la maison, moitié arrière cyan. La séparation suit les huit directions. La silhouette, les chenilles et les détails métalliques de base sont conservés, avec zooms entiers 1×/2×/3×. Les PNG originaux restent archivés intacts.
- **Vente et sauvegarde** : la conversion payée en Slowpost conserve l’investissement du Scoutpost pour le remboursement. Les anciens Slowposts directement construits ne reçoivent pas ce supplément. Le marqueur occupe l’octet réservé du bâtiment.

Le correctif utilise le **protocole réseau 20** ; les participants doivent utiliser le même moteur corrigé. Les nouvelles sauvegardes portent **9838**, les formats précédents restent lisibles. Les anciens moteurs refusent les nouvelles sauvegardes. Les téléchargements publics de la 1.0.544 initiale ne contiennent pas encore ces correctifs.

## Vérifications

Résultats réels à consigner dans `Validation-Correctifs-1.0.544.json` après compilation et exécution. Les tests ajoutés couvrent l’amélioration, les permissions de commande, les deux états animés, la valeur de vente sauvegardée, les libellés après défilement et les deux couleurs des huit vues de moissonneuses.
