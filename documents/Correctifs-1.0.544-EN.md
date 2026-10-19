# Dune Legacy Tornie 1.0.544 fixes

Base: tag `v1.0.544` (`d979cea6ab8d2554ac7ab19d1444dcdf182ee245`). Applies to Tornie, Tornie Lite, Jericho, Jericho Lite, ArcaKev and ArcaKev Lite. Vanilla retains its rules and graphics.

- **Slowpost**: removed from Construction Yard production. Neutral technology converts a Scoutpost at level 7 with IX for **450 credits**. The button shows the conversion price in its tooltip. Captured or Chaos-borrowed plans use the same rule. Editor placement and older directly built posts remain supported.
- **Animation**: the two supplied Slowpost states now loop independently of aiming direction. Targeting and slowing missiles remain functional.
- **Ranged Tank**: darker, more saturated violet turret; chassis retains house colour.
- **Worfinery**: hover labels use the same order as portraits and clicks. Harvester stays last, and labels remain aligned after scrolling and save/reload.
- **Specialist Harvesters**: both liveries are rebuilt in memory from the vanilla Harvester. Capacity: violet front half and house-coloured rear. Fast: house-coloured front half and cyan rear. The split follows all eight directions. Original silhouette, tracks and metal details are retained, with exact 1×/2×/3× zooms. Original supplied PNGs remain archived unchanged.
- **Sale/save state**: paid Slowpost conversions retain the Scoutpost investment for refunds. Older directly purchased posts do not receive an unpaid extra refund. Conversion provenance occupies the reserved structure byte.

Uses **network protocol 20**; every participant needs the same corrected engine. New saves use **9838**; earlier formats remain readable. Older engines reject new saves. Download the corrected 1.0.544 again on every computer.

## Verification

Linux engine and tests build successfully: **142 cases / 10,647 assertions**, **CTest 2/2**, including real UDP. `--verify-correctifs544` passes in all seven modes, covering upgrade gates, authorization, payment, animation and saves; six custom modes also pass artwork and Worfinery-label checks. All eight Harvester silhouettes match vanilla, with complementary colour masks. Website: 36 routes and 96 affected FR/EN desktop/mobile views. Public Windows/Linux packages must pass automated engine, network and installed-runtime checks in French/English before replacement; see the [report](Validation-Correctifs-1.0.544.json).
