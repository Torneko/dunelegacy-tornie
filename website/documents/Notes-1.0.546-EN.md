# Dune Legacy Tornie 1.0.546

This version fixes editor CHOAM and the Atreides/Ordos frames, exposes Vanilla campaigns in the editor and revises **Tornie and Tornie Lite campaigns only**.

## Campaigns and co-op

- 18 campaigns, 396 solo scenarios and matching cooperative maps use faction buildings, units and technology, with reinforcements for both sides.
- Each campaign keeps one supporting enemy throughout its missions. Support varies between campaigns; Rebels keeps Harkonnen even when the primary enemy changes.
- Missions 001–019 have two opponents, 020–021 have three and final mission 022 has four, with bases, forces and reinforcements.
- In 002–004, support buildings stay at least ten tiles from player building footprints and allied starting MCV footprints. Two/four-faction co-op follows the same rule.
- Tornie openings have two small enemy bases, 1,500 starting credits and a 2,000 refined-spice quota.
- Tornie Rebels opening: a starport and small economy, without a free construction yard or MCV. Buy the MCV through CHOAM; each co-op partner receives a matching economy.
- Tornie Wildspade opening: loaned Carryall and Ornithopter, with an anti-air threat.
- A guest choosing the enemy support faction no longer replaces that support. Progression and cooperative checkpoints retain additional participants.

**Jericho, Jericho Lite, ArcaKev and ArcaKev Lite campaigns, briefings and co-op maps retain their 1.0.545 baseline**, pending their authors’ feedback. Vanilla campaign archives remain unchanged.

## Editor and technology

- Fixed the CHOAM opening crash: all 31 catalogue units fit in a scrolling list, supporting apply, undo and map reload.
- Separate editor sources for the active mod’s solo/co-op campaigns and Vanilla campaigns. Loaded campaign factions remain selectable, including in Lite editions.
- Golden Atreides and Ordos frames now match their faction on score and region screens.
- Wildspade, wherever the faction is available: Hightech Factory and Carryall at technology 4; Chemical Carryall at 5 after the first upgrade. Ornithopter and IX remain at technology 7.

## Installation and compatibility

Fully extract the Windows package into a new folder, then run `dunelegacy.exe` in its root. The bilingual guide is available online and in `DuneLegacyTornie-Site-Local-1.0.546.zip`; open `dist/index.html` inside that archive.

Start a **new campaign** to use revised missions. Existing checkpoints retain their saved mission state.

Networking requires **protocol 24**, version 1.0.546 and matching mods on every computer. Protocol 23 previews and 1.0.545 cannot join this version. Save format **9839** remains; earlier save readers are retained.

## Validation

Windows validation covers 146 cases / 11,217 engine and network assertions, 5,265 editor map loads, 252 solo starts, 224 cooperative starts, 76 late cooperative checkpoint restores and 28 frame compositions. All 2,320 campaign, briefing and co-op files from other authors match the 545 baseline. Package hashes and corresponding sources accompany the delivery.

Automated checks do not represent a complete human campaign playthrough or human two-PC co-op. The delivery report identifies the platforms actually verified.
