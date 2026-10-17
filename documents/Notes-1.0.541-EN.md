# Dune Legacy Tornie 1.0.541 — ArcaKev, co-op and fixes

**Replacement 1.0.541 — allied construction, editor and Rebels mentat**

- **Build on allied land**: a new co-op lobby checkbox, off by default and set by the host before a new mission. It extends construction range from other active allies’ slabs and buildings, including Archon. It is independent of Allied control; each faction keeps its buildings and expenses. Terrain, occupancy and technology rules still apply.
- **Editor**: clicking a unit or building selects its actual owner by stable ID, even when the displayed faction order differs from the internal order. Named/numbered players and reloaded maps are covered in all seven modes.
- **Animated Rebels cat mentat**, using the existing Wildspade/Jericho cat: dark grey in Vanilla, Jericho and ArcaKev, green in Tornie. Its tint follows the faction palette; eyes, mouth, whiskers, shoulder and five animation states retain their anchors. Lite editions do not offer Rebels.
- Previous fixes are retained: the host’s starting budget for all four active allies, +500 once per mission in Easy Mode, and ArcaKev Rebels dark grey / Corruptique yellow with unchanged banners.
- **Download this replacement 1.0.541 on every computer**: protocol **13**, new saves **9833**. Earlier saves remain readable; shared construction defaults off when no setting was recorded. Loading preserves recorded balances and match rules.

- **ArcaKev and ArcaKev Lite**: Jericho-derived mods with custom units and **normal plus green and blue spice**, matching editor and map generation, distinct campaigns and region ownership.
- ArcaKev house/campaign order: **Harkonnen, Atreides, Ordos, Fremen, Sardaukar, Mercenary, Kleshmersh, Wildspade, Rebels, Neutral, Tharpique, Corruptique**. Persistent faction IDs remain stable.
- **Four allied factions in co-op**, **eight players in Archon**, with freely assigned controllers, including all eight in one faction. Closed slots 3/4 spawn no army, MCV or reinforcements.
- Each new mission draws existing scenarios: three variants at stages 2–7 (002–019), two at stage 8 (020/021). Retries and saves retain the selection. Final scenario 22 uses **technology 9** in campaign and skirmish.
- **Easy Mode** grants every active ally **500 credits at every mission start**, retaining the 25-credit purchase discount. Optional **Enemy forces +10** combines with +5 for **15** per present opponent.
- Reinforcements and shared harvest quota follow active allied factions. All humans confirm mission transitions. Lobby, resume and saves support eight controllers; guests can select bonus colours.
- **Mod checkboxes** take effect; metadata no longer accumulates quotes and Save remains visible. **Jericho banners** refresh at startup.
- **Vanilla Kleshmersh is orange**, with ordinary non-flame Troopers. Outside Vanilla, a **Chaos-borrowed Kleshmersh WOR** produces flame Troopers at Kleshmersh’s nominal **200-credit price**, including Lite editions. **Flame Trooper impact damage increases by 20%.**
- **Scoutpost, Flamepost and Chemipost** fire missiles before and after upgrading, at **7 tiles** instead of 5. Flamepost uses the Flame Tank effect and deals **12 damage** instead of 10. Chemipost retains allied healing missiles.
- **Sonic Trike** moves to the next level wherever it previously unlocked at 2. **Wildspade and Sardaukar** receive Light Factory at **level 2** outside Vanilla. **Ordos and Neutral** receive Siege Tank at **level 6, upgrade three**.
- **Doublefinery** unloads Harvesters **15% faster** in both bays, with a solid **5×2** preview inside the dotted **5×3** grid and matching map placement.
- Added turrets on custom vehicles retain their proportions at **1×, 2× and 3×**. Trike graphics are retained.
- **Animated Kleshmersh husky mentat**, from Tornie's original supplied ZIP: five gaze and speech states in Vanilla, Tornie, Jericho and ArcaKev; original PNGs and briefing-driven speech.
- **Editor** team changes work through scrolling dropdowns, with **12 teams**, ArcaKev order and matching custom units/spice.
- **FR/EN website** lists every house's Palace power and recharge by mod, updated technology, new mods, husky and notes; matching offline content.

Every player needs **1.0.541 / protocol 13** and the same mod. New saves use **9833**; older saves remain readable. The Internet host relay without VPN and 1.0.540 AppImage/Mint startup fixes are retained. The host must remain reachable and connected. On Mint use `DuneLegacyTornie-1.0.541-Linux-Mint-Compatible.AppImage`; add `--appimage-extract-and-run` without FUSE. On Windows, extract the entire ZIP into a fresh directory before starting `dunelegacy.exe`.

Automated tests isolate guests in eight-participant UDP sessions and verify commands, start, saves and all seven modes. The validation report records final results and CI links. Human play across independent Internet connections and native Mint 22.3/kernel 6.17 startup remain pending.
