# Recherches du radar / Radar research — préparation après 1.0.544

Base : 1.0.544 corrigée, `d6a29fd82906eb1f4856eced2dfb01fe3e74637e`.
Les paquets publics 1.0.544 ne contiennent pas ces recherches. Aucun nouveau tag
ni publication n'est créé par cette préparation.

## Français

Tornie a validé les réglages ci-dessous le 20 octobre 2026. Le système est
implémenté dans la branche de préparation, pour Tornie, Jericho, ArcaKev et leurs
éditions Lite. Vanilla conserve ses règles sans recherches.

| Recherche | Prix | Durée simulée | Prérequis | Effet |
| --- | ---: | ---: | --- | --- |
| Extension de la base | 500 | 45 s | Niveau 4, radar alimenté | Construction +2 cases |
| Vision élargie | 500 | 45 s | Niveau 4, radar alimenté | Vision +2 cases, sauf aéronefs |
| Spécialisation | 750 | 60 s | Niveau 7, IX, radar alimenté | Bonus de la maison ci-dessous |

| Maison | Spécialisation |
| --- | --- |
| Harkonnen | Combat Tank, explosion du Devastator et missile du palais : dégâts +20 % |
| Atreides | Troopers individuels et escouades : déplacement +15 % |
| Ordos | Véhicules légers : déplacement +15 % |
| Fremen | Troopers individuels et escouades : déplacement +15 % |
| Sardaukar | À la destruction d'un char : 35 % de chance de sauver un Trooper de sa maison |
| Mercenary | Infanterie, véhicules légers et aéronefs : prix −10 % |
| Neutral | Réparation des bâtiments et au chantier : débit +25 %, même coût par PV |
| Rebels | Véhicules armés au sol : portée +1 case |
| Wildspade | Ornithoptères et Carryalls, variantes comprises : PV +25 % |
| Kleshmersh | Incendies persistants : durée +2 s, même cadence de dégâts |
| Corruptique | Chars classiques et variantes : dégâts subis −20 %, déplacement −10 % |
| Tharpique | Éclaireurs et artillerie : vision +2, cumulée au +2 commun |

Les valeurs chiffrées et les concepts sont approuvés. Les listes exactes et les
règles pratiques ci-dessous sont les choix d'implémentation, à distinguer des
réponses explicites de Tornie :

- Une recherche active par maison, paiement au lancement, sans répétition. Tous
  ses radars partagent la progression. Coupure d'énergie ou perte de tous les
  radars : pause ; un radar de remplacement reprend la progression. Un bonus
  terminé demeure jusqu'à la fin de la partie, même sans radar ou IX.
- Les unités présentes et futures en profitent. La maison du propriétaire actuel
  détermine les bonus ; les plans empruntés/capturés en Chaos ne changent pas la
  spécialisation. Déviation et retour recalculent les bonus et conservent le
  pourcentage de santé des aéronefs. Les postes archontes partagent la recherche
  de leur maison. Le contrôle allié autorisé peut commander le radar allié ;
  c'est sa maison propriétaire qui paie.
- Les commandes sont déterministes, autorisées côté moteur et sérialisées. Aucun
  générateur aléatoire n'est ajouté pour la progression. L'IA lance les recherches
  dans l'ordre commun/base, vision, spécialisation lorsqu'elle a au moins 2000
  crédits et remplit les prérequis.
- Véhicules légers : Trike, Raider Trike, Quad, Raider Quad, Rocket Trike, Sonic
  Trike, Flame Quad. Aéronefs : Carryall, Chemical Carryall, Ornithopter, Elite
  Ornithopter ; la Frigate est exclue.
- Chars classiques Corruptique : Combat, Siege, Elite Siege, Chemical Siege,
  Flame, Ranged et Slow Tank. Les chars Sardaukar comprennent aussi Devastator,
  Sonic Tank et Harvestank ; Launchers, Deviator et moissonneuses ordinaires sont
  exclus. Le sauvetage remplace le tirage du Soldier habituel, sans cumul ; une
  unité Trooper, jamais une escouade. Pas de sauvetage lors du nettoyage d'une
  unité vivante en fin de partie.
- Artillerie Tharpique : Launcher, Elite Launcher, Siege, Elite Siege, Chemical
  Siege et Ranged Tank, en plus des véhicules légers. Les bâtiments ont seulement
  le bonus commun ; les aéronefs n'ont aucun bonus de vision de ces recherches.
- Le rabais Mercenary se cumule avec le prix Easy Mode, une seule fois, y compris
  au Starport. Une commande déjà en file conserve son devis ; les commandes
  suivantes utilisent le nouveau tarif. Les bâtiments ne sont pas remisés.
- Le feu persistant couvre les impacts incendiaires et le napalm ; les flammes
  purement visuelles ne sont pas prolongées. Les dégâts directs du lance-flammes
  ne changent pas. Un incendie encore actif au moment de la recherche reçoit le
  bonus, hors phase d'extinction. Sa durée supplémentaire est sauvegardée.
- Les boutons affichent coût, durée et prérequis en infobulle, progression et
  achèvement. Les noms courts sont descriptifs, pas des noms définitifs imposés.

## English

Tornie approved these numbers on 20 October 2026. Research is implemented on the
preparation branch for Tornie, Jericho, ArcaKev and all three Lite editions.
Published 1.0.544 packages do not contain it. Vanilla has no research.

Common research costs **500 credits / 45 simulated seconds / technology 4**.
Specialization costs **750 / 60 seconds / technology 7 + IX**. All require a
powered Radar. Common effects: **+2 build tiles** and **+2 sight, excluding
all aircraft**. Three choices per house, including its specialization:

| House | Specialization |
| --- | --- |
| Harkonnen | Combat Tank, Devastator explosion and Palace missile: damage +20% |
| Atreides / Fremen | Troopers, including squads: movement +15% |
| Ordos | Light vehicles: movement +15% |
| Sardaukar | Destroyed tanks: 35% chance to rescue one same-house Trooper |
| Mercenary | Infantry, light vehicles and aircraft: purchase price −10% |
| Neutral | Building/Repair Yard repair throughput +25%, same credits per HP |
| Rebels | Armed ground vehicles: weapon range +1 tile |
| Wildspade | Ornithopters/Carryalls and variants: health +25% |
| Kleshmersh | Persistent fire lasts 2 seconds longer, unchanged damage cadence |
| Corruptique | Conventional tanks/variants: incoming damage −20%, movement −10% |
| Tharpique | Scouts/artillery: sight +2, stacking with common research |

Implementation choices, rather than separately approved details:

- One prepaid active research per house; Radars share its progress. Power/Radar
  loss pauses it. A replacement Radar resumes it. Completed bonuses last for
  the match, including after losing Radar/IX; saves retain progress and completion.
- Current owner determines the specialization, including deviation/reversion;
  borrowed Chaos technology remains separate. Existing/future units benefit.
  Aircraft retain their damage ratio. Archon players share their house's state.
  Authorized allied control spends the Radar owner's credits.
- AI research order is build range, sight, specialization, starting with at least
  2000 credits. Tick progression uses no randomness and network commands validate
  ownership, permissions, research index, prerequisites and price.
- Light vehicles: Trike, Raider Trike, Quad, Raider Quad, Rocket/Sonic Trike and
  Flame Quad. Aircraft: Carryall, Chemical Carryall, Ornithopter and Elite
  Ornithopter; no Frigate. Non-heavy means infantry, these lights and aircraft.
- Corruptique tanks: Combat, Siege, Elite Siege, Chemical Siege, Flame, Ranged,
  Slow. Sardaukar also includes Devastator, Sonic Tank, Harvestank, excluding
  Launchers/Deviator/ordinary Harvesters. Rescue replaces the normal Soldier roll;
  it produces one Trooper, not a squad, and does not trigger on living-unit cleanup.
- Tharpique scouts are the listed lights; artillery is Launcher, Elite Launcher,
  Siege, Elite Siege, Chemical Siege, Ranged Tank. Buildings receive only common
  sight; aircraft receive neither sight bonus.
- Mercenary discount stacks once with Easy Mode, including Starport. Existing
  production quotes stay fixed; new orders receive the reduction. No building
  discounts. Repair throughput and payment scale together, preserving price/HP.
- Persistent flame impacts and napalm gain the duration bonus; visual flames and
  direct weapon damage do not. Active fires receive it before fading. Additional
  lifetime survives a save made during the extension.

New preparation compatibility: **protocol 21 / saves 9839**, stable item IDs and
readers for old saves. This is not the published 544 protocol 20 / saves 9838.
See [next-release notes](Next-Release-EN.md) and the validation
receipt for executed checks and remaining limits.
