# Co-op and Archon campaigns — 1.0.541

[Guide français](COOP-CAMPAIGN.md)

Every participant needs **1.0.541 / protocol 13**, the same mod and a distinct name. LAN or Internet play uses the host relay when guests cannot communicate directly, without a VPN. The host must remain reachable and connected.

At every new mission start, all active allied factions receive **the same initial budget as the host**, including zero or missing optional-player credit fields. Easy Mode then adds 500 credits once per faction. Loading a save preserves recorded balances. This fix replaces 1.0.541: all participants must download it again (protocol 13).

## Players and factions

A new campaign supports **four allied factions** with separate bases, armies and funds. Slots 3 and 4 start closed: closed slots spawn no MCV, starting forces or reinforcements. Allies may choose the same faction.

Enable **Archon** when creating the campaign to admit **eight players**. The host freely assigns each controller to any of the four allied factions, including all eight in one faction. Controllers in the same faction share its objects and money. Uncontrolled factions may remain closed or use AI. Enemies remain AI-controlled.

Optional **Allied control** also permits commands to other allied factions; their owners pay the costs. The host selects AI types and difficulties. Guests can select bonus colours.

**Build on allied land**, off by default, is set by the host before a new mission. Every active allied faction may build near other allies’ slabs and buildings, including Archon. Buildings and expenses retain their owners; terrain must remain valid and unoccupied. This option is independent of Allied control. Guests receive the host’s setting. Saves retain it, and it is locked when loading a mission already in progress. Older saves without the field keep shared construction disabled.

There are 63 source campaigns and 1,386 co-op maps: Vanilla 9/198; Tornie, Jericho and ArcaKev 12/264 each; Tornie Lite, Jericho Lite and ArcaKev Lite 6/132 each. Nine stages follow the host faction's original campaign, fixed when the session starts.

## Creating and playing

1. Open **Co-op campaign**, select LAN or Internet, create a campaign and choose Archon if desired.
2. Other players join through the browser or address. Configure allied factions, AI and Archon assignments in the lobby.
3. Start after every guest has synchronized version, rules and mod resources.
4. After each mission, all connected humans must confirm readiness. Disconnection never advances progress.

The host retains the original forces. Each other active allied faction receives compatible starting forces and credits, a nearby deployable MCV and an intro WOR when applicable. Host reinforcements are copied to each active ally with compatible units. Harvest objectives sum allied spice and multiply the solo quota by the number of active allied factions, independent of their controller counts.

Enemy forces and roles remain. A faction conflicting with an ally is replaced. With four allies in Lite, multiple enemies may share a faction while retaining separate ownership.

## Variants and difficulty

Each **new mission** draws an existing scenario: stage 1 uses 001; stages 2–7 choose from 002–004, 005–007, 008–010, 011–013, 014–016 and 017–019 respectively; stage 8 chooses 020 or 021; stage 9 uses 022 at technology 9. The shared draw is stored and remains stable on retry or reload. Some original scenarios share terrain seeds but have different forces. Older sessions retain their previous selection.

**Easy Mode**, off by default, starts a new campaign at stage 2. Each active allied faction, human or AI, receives **500 credits at every mission start** and a 25-credit purchase discount, minimum price 1. Enemies receive neither benefit.

Independent **Enemy forces +5** and **Enemy forces +10** switches combine to **15 extra units per present enemy**, at every new mission. Units match the technology and appear near existing forces or at an edge for reinforcement-only enemies. Absent opponents receive none. Checkpoint loads restore forces without adding them again.

## Saves and resuming

Shared progress and checkpoint files are `coop/<session>.ini` and `coop/<session>.dls`, next to `Dune Legacy.ini`, separate from solo progress. Keep both files. Resuming preserves the mod, selected scenario and checkpoint. Controller names can be reassigned; already-started saves retain controller counts per faction.

The host lobby's **Load a save** offers manual `mpsave/*.dls` saves and automatic `coop/*.dls` checkpoints. Loading a compatible save reopens the lobby; all guests rejoin. Cancelling preserves the lobby. New saves use format **9833**; older saves remain readable with their stored maps and rules.

**Together on Arrakis / COOP_CONQUEROR** rewards completing the shared campaign with a human or AI ally, including Easy Mode. It does not complete a solo campaign. The catalogue remains 67 achievements, with local profiles.

## Verification and maintenance

Automated checks cover all seven modes and stages 1–9, closed/active allies, MCV 3/4 deployment, eight controllers in one faction, Easy credits, +5/+10/+15 forces, saves and lobby assignments. The eight-player UDP check filters direct guest links and verifies host relay, start and every participant's commands. Human play across independent Internet connections remains pending. Tornie's historical two-PC confirmation concerns 1.0.538.

Templates live in `data/coop/faction<id>/` for Vanilla and `mods/<mod>/campaign/coop/faction<id>/` for mods, with `-v1`/`-v2` variants. `scripts/build-coop-campaign.py --check` audits all 1,386 variants, 63 legacy root maps and 2,342 protected solo file hashes. Optional ally sections materialize only for active slots; the `[COOP]` context synchronizes map, controllers and mission selection.
