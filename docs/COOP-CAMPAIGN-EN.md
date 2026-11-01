# Co-op and Archon campaigns — 1.0.547

[Guide français](COOP-CAMPAIGN.md)

Every participant needs **1.0.547 / protocol 25**, the same mod and a distinct name. LAN or Internet play uses the host relay when guests cannot communicate directly, without a VPN. The host must remain reachable and connected.

At every new mission start, all active allied factions receive **the same initial budget as the host**, including zero or missing optional-player credit fields. Easy Mode then adds 500 credits once per faction. Loading a save preserves recorded balances. Older protocol 13, 22 or 23 clients cannot join this version.

## Players and factions

A new campaign supports **four allied factions** with separate bases, armies and funds. Slots 3 and 4 start closed: closed slots spawn no MCV, starting forces or reinforcements. Allies may choose the same faction.

Enable **Archon** when creating the campaign to admit **eight players**. The host freely assigns each controller to any of the four allied factions, including all eight in one faction. Controllers in the same faction share its objects and money. Uncontrolled factions may remain closed or use AI. Enemies remain AI-controlled.

Optional **Allied control** also permits commands to other allied factions; their owners pay the costs. The host selects AI types and difficulties. Guests can select bonus colours.

**Build on allied land**, off by default, is set by the host before a new mission. Every active allied faction may build near other allies’ slabs and buildings, including Archon. Buildings and expenses retain their owners; terrain must remain valid and unoccupied. This option is independent of Allied control. Guests receive the host’s setting. Saves retain it, and it is locked when loading a mission already in progress. Older saves without the field keep shared construction disabled.

There are 63 source campaigns and 1,386 co-op maps: Vanilla 9/198; Tornie, Jericho and ArcaKev 12/264 each; Tornie Lite, Jericho Lite and ArcaKev Lite 6/132 each. Nine stages follow the host faction's original campaign, fixed when the session starts.

## Tornie and Tornie Lite campaigns in 1.0.547

The revision applies to **Tornie and Tornie Lite only**: 18 campaigns and 396 solo scenarios with matching co-op maps. **Jericho, Jericho Lite, ArcaKev and ArcaKev Lite campaigns, briefings and co-op maps** retain their 1.0.545 baseline, pending their authors’ feedback. Vanilla archives are preserved.

- Missions 001–019: two opponents; 020–021: three; final mission 022: four. Additional opponents have bases, forces and reinforcements.
- Each campaign keeps one **persistent support opponent**, even when the primary enemy changes. Support varies between campaigns: Rebels keeps Harkonnen. See [Campaigns](CAMPAIGNS.md) for the full table.
- In 002–004, support buildings stay at least **ten tiles** from allied building footprints and starting MCV footprints, including four-allied-faction co-op.
- Openings: two small enemy bases, **1,500 credits** per active allied faction before maintenance and a **2,000 refined-credit quota per active allied faction**. Archon controllers do not multiply the quota.
- Tornie Rebels opening: each ally receives a **starport and small economy**, without a free construction yard or MCV. Buy the MCV through CHOAM.
- Tornie Wildspade opening: loaned Carryall and Ornithopter, with anti-air threats. Wherever Wildspade is available, Hightech/Carryall unlock at 4 and Chemical Carryall at 5 after upgrade one; Ornithopter and IX remain at 7.

Start a **new campaign** to use these recipes. Existing checkpoints retain their saved map and state.

## Creating and playing

1. Open **Co-op campaign**, select LAN or Internet, create a campaign and choose Archon if desired.
2. Other players join through the browser or address. Configure allied factions, AI and Archon assignments in the lobby.
3. Start after every guest has synchronized version, rules and mod resources.
4. After each mission, all connected humans must confirm readiness. Disconnection never advances progress.

The host retains the scenario forces. In ordinary openings, each other active allied faction receives compatible starting forces and credits, a nearby deployable MCV and an intro WOR when applicable. Tornie’s Rebels opening follows the trading start above. Host reinforcements are copied to each active ally with compatible units. Harvest objectives sum allied spice and multiply the solo quota by the number of active allied factions, independent of their controller counts.

New Tornie/Tornie Lite campaigns preserve enemy identities even when a guest chooses the same faction; ownership and teams remain distinct. Vanilla, Jericho/ArcaKev and their Lite campaigns retain the historical enemy-faction replacement rule. Multiple enemies may share a faction when allies occupy the Lite roster, with distinct owners.

## Variants and difficulty

Each **new mission** draws an existing scenario: stage 1 uses 001; stages 2–7 choose from 002–004, 005–007, 008–010, 011–013, 014–016 and 017–019 respectively; stage 8 chooses 020 or 021; stage 9 uses 022 at technology 9. The shared draw is stored and remains stable on retry or reload. Some original scenarios share terrain seeds but have different forces. Older sessions retain their previous selection.

**Easy Mode**, off by default, starts a new campaign at stage 2. Each active allied faction, human or AI, receives **500 credits at every mission start** and a 25-credit purchase discount, minimum price 1. Enemies receive neither benefit.

Independent **Enemy forces +5** and **Enemy forces +10** switches combine to **15 extra units per present enemy**, at every new mission. Units match the technology and appear near existing forces or at an edge for reinforcement-only enemies. Absent opponents receive none. Checkpoint loads restore forces without adding them again.

## Saves and resuming

Shared progress and checkpoint files are `coop/<session>.ini` and `coop/<session>.dls`, next to `Dune Legacy.ini`, separate from solo progress. Keep both files. Resuming preserves the mod, selected scenario and checkpoint. Controller names can be reassigned; already-started saves retain controller counts per faction.

The host lobby's **Load a save** offers manual `mpsave/*.dls` saves and automatic `coop/*.dls` checkpoints. Loading a compatible save reopens the lobby; all guests rejoin. Cancelling preserves the lobby. New saves use format **9840**; older saves remain readable with their stored maps and rules. New Tornie campaign checkpoints retain the third and fourth enemy slots.

**Together on Arrakis / COOP_CONQUEROR** rewards completing the shared campaign with a human or AI ally, including Easy Mode. It does not complete a solo campaign. The catalogue has **73 achievements**, with local profiles. Cooperative rescue awards follow allied faction elimination and team victory; Archon controllers share their house’s state.

## Opening maps in the editor

In the opening dialog, choose **Campaign – mod name**, **Campaign COOP – mod name**, **Campaign – Vanilla** or **Campaign COOP – Vanilla**. Vanilla provides 198 solo scenarios; cooperative sources also include nine legacy shared maps per edition. Save a personal copy when editing. Loaded owners remain selectable, including in Lite editions. Editor CHOAM displays all 31 units in a scrolling list.

## Verification and maintenance

Automated checks cover all seven modes and stages 1–9, closed/active allies, MCV 3/4 deployment, eight controllers in one faction, Easy credits, +5/+10/+15 forces, saves and lobby assignments. The eight-player UDP check filters direct guest links and verifies host relay, start and every participant's commands. Human play across independent Internet connections remains pending. Tornie's historical two-PC confirmation concerns 1.0.538.

Native Windows and Linux/Mint FR/EN checks for 1.0.547 cover 252 solo starts, 224 cooperative starts and 76 late cooperative restores per language. Complete human campaign playthroughs, balance sign-off and human Internet co-op across separate connections remain unconfirmed. See [Release follow-up](SUIVI-1.0.547-EN.md).

Templates live in `data/coop/faction<id>/` for Vanilla and `mods/<mod>/campaign/coop/faction<id>/` for mods, with `-v1`/`-v2` variants. `scripts/build-coop-campaign.py --check` audits all 1,386 variants and 63 legacy root maps. `scripts/check-campaigns-r2.py` checks support, distances and 2,320 foreign campaign/briefing/co-op files identical to the baseline. Optional ally sections materialize only for active slots; the `[COOP]` context synchronizes map, controllers and mission selection.
