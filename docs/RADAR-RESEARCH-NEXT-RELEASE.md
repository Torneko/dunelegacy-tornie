# Recherches du radar — prochaine version / Radar research — next release

## Français

**État au 19 octobre 2026 : concepts validés par Tornie pour la prochaine version.**
Ce document consigne le contenu demandé. Le système de recherche n'est pas encore
implémenté ni testé ; les valeurs laissées ouvertes ci-dessous restent à définir.
La 1.0.544 publiée ne contient pas ces recherches.

### Organisation validée

Chaque maison dispose de trois recherches au radar : deux recherches communes et
une spécialisation de maison. Les noms descriptifs ci-dessous sont provisoires.

| Recherche commune | Effet validé |
| --- | --- |
| Extension de la base | **+2 cases de portée de construction** autour de la base. |
| Vision élargie | **+2 cases de champ de vision** pour toutes les unités et tous les bâtiments, **sauf les unités aériennes**. |

### Spécialisations des douze maisons

| Maison | Effet validé |
| --- | --- |
| Harkonnen | Augmenter les dégâts des **Combat Tanks**, de l'**explosion du Devastator** et du **missile du palais**. |
| Atreides | Augmenter la **vitesse de déplacement des Troopers**, escouades comprises. |
| Ordos | Augmenter la **vitesse des véhicules légers**. |
| Fremen | Augmenter la **vitesse de déplacement des Troopers**, escouades comprises. |
| Sardaukar | Chaque **char, lourd ou non**, a **35 % de chances** de faire apparaître **un Trooper de la même maison** à sa destruction. |
| Mercenary | Réduire de **10 % le coût des unités non lourdes**. |
| Neutres | Améliorer l'**efficacité des réparations**. Vitesse, coût ou combinaison des deux à préciser. |
| Rebels | Augmenter de **1 case la portée des armes des véhicules**. |
| Wildspade | Rendre les **unités aériennes plus robustes**, dont les Ornithopters et les Carryalls, variantes comprises. |
| Kleshmersh | Prolonger les **dégâts de feu de 2 secondes**, avec les mêmes dégâts par seconde. |
| Corruptique | Renforcer le **blindage de certains chars**, avec une **légère perte de vitesse**. |
| Tharpique | Augmenter le **champ de vision des éclaireurs et de l'artillerie**. |

**Cumul Tharpique expressément confirmé :** les unités concernées bénéficient à la
fois du bonus propre à leur maison et du **+2 de vision commun**.

### Points à fixer avant intégration

- Valeurs non chiffrées : dégâts Harkonnen, vitesse Atreides/Fremen/Ordos,
  robustesse Wildspade, blindage et ralentissement Corruptique, vision Tharpique.
- Réparation neutre : vitesse, coût ou les deux ; types de réparations concernés.
- Listes exactes des unités et des armes concernées : notamment chars Sardaukar,
  unités non lourdes Mercenary, véhicules Rebels, chars Corruptique, éclaireurs et
  artillerie Tharpique, ainsi que les effets de feu Kleshmersh.
- Coût, durée, niveau technologique et prérequis de chaque recherche.
- Répartition du système entre les mods et leurs éditions Lite.
- Règles d'application et de conservation : unités existantes et futures,
  conversion/capture, disparition du radar, Chaos Mode, Starport et Easy Mode.
- Interaction des 35 % Sardaukar avec les éventuelles apparitions d'infanterie
  déjà prévues par le moteur.

L'interface du radar, les sauvegardes, les commandes réseau déterministes et les
documents FR/EN devront être intégrés et vérifiés avant publication. Les valeurs
et conditions non fixées ne constituent pas des décisions approuvées.

## English

**Status on 19 October 2026: concepts approved by Tornie for the next release.**
This document records the requested content. Research has not been implemented or
tested; the open parameters below still need to be defined. The published 1.0.544
does not contain this system.

### Approved structure

Each house receives three Radar research options: two common upgrades and one
house specialization. The descriptive names below are provisional.

| Common research | Approved effect |
| --- | --- |
| Base expansion | **+2 tiles of building placement range** around the base. |
| Extended vision | **+2 tiles of sight range** for all units and buildings, **excluding aircraft**. |

### Twelve house specializations

| House | Approved effect |
| --- | --- |
| Harkonnen | Increase damage from **Combat Tanks**, the **Devastator explosion** and the **Palace missile**. |
| Atreides | Increase **Trooper movement speed**, including squads. |
| Ordos | Increase **light vehicle movement speed**. |
| Fremen | Increase **Trooper movement speed**, including squads. |
| Sardaukar | Each **tank, heavy or otherwise**, has a **35% chance** of spawning **one Trooper belonging to the same house** upon destruction. |
| Mercenary | Reduce the **cost of non-heavy units by 10%**. |
| Neutral | Improve **repair efficiency**. Speed, cost, or both remain to be specified. |
| Rebels | Increase **vehicle weapon range by 1 tile**. |
| Wildspade | Make **aircraft more durable**, including Ornithopters, Carryalls and their variants. |
| Kleshmersh | Extend **fire damage duration by 2 seconds**, keeping damage per second unchanged. |
| Corruptique | Strengthen the **armour of selected tanks**, with a **small movement speed penalty**. |
| Tharpique | Increase the **sight range of scouts and artillery**. |

**Tharpique stacking explicitly approved:** affected units receive both their
house-specific sight bonus and the **common +2 sight upgrade**.

### Parameters to settle before integration

- Unspecified amounts: Harkonnen damage; Atreides/Fremen/Ordos speed; Wildspade
  durability; Corruptique armour and speed penalty; Tharpique sight.
- Neutral repairs: speed, cost or both, and the repair types affected.
- Exact affected units and weapons, particularly Sardaukar tanks, Mercenary
  non-heavy units, Rebels vehicles, Corruptique tanks, Tharpique scouts/artillery
  and Kleshmersh fire effects.
- Cost, duration, technology level and prerequisites for each research.
- Availability across mods and Lite editions.
- Application and persistence rules: existing and future units, conversion and
  capture, loss of the Radar, Chaos Mode, Starport and Easy Mode.
- Interaction between the Sardaukar 35% roll and any existing engine rules that
  spawn infantry when vehicles are destroyed.

The Radar interface, saves, deterministic network commands and French/English
documentation must be integrated and verified before publication. Unspecified
values and conditions are not approved design decisions.
