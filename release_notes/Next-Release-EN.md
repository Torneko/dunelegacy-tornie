# Next release — preparation from corrected 1.0.544

**Unpublished; version number and publication date remain to be chosen.** Public
packages remain corrected 1.0.544 (`d6a29fd`), protocol 20 / saves 9838. This
preparation uses protocol 21 / saves 9839, reads previous saves and preserves
stable item IDs. No tag or release is created by this preparation.

- Raider Quad: all eight directions follow native vanilla Quad ground anchors
  at exact 1×/2×/3× zooms. Supplied PNGs remain intact.
- Editor: clean Slowpost palette and placed sprite, without a blue star.
  Custom-unit editor stars remain.
- Standard non-Vanilla Rebels 2×2 Windtrap: editor-only “Pre-upgraded” checkbox.
  Map round-trip, undo/redo, deletion and game saves retain it. Double output and
  green animation apply immediately without an initial purchase/IX requirement.
  Other and advanced Windtraps do not gain this map option.
- Chaos: standard 2×2 Windtraps exchange technology plans; borrowed Rebels plans
  allow another house to upgrade power. Scoutpost-to-Flamepost/Chemipost/Slowpost
  conversions follow borrowed plans, including Lite, retaining their donor after
  conversion/reload. Advanced Windtraps stay excluded. Old 15-building saved
  tables preserve all existing donor choices.
- Radar: two common research choices plus one house specialization in all six
  non-Vanilla mods, with UI, network commands and saved progress. Common:
  500 credits, 45 s, technology 4. Specialization: 750, 60 s, technology 7 + IX.
  Completed bonuses last for the match, benefiting existing/future units. See
  [all twelve specializations and scope](../docs/RADAR-RESEARCH-NEXT-RELEASE.md).
- Intro: original blue Westwood Studios animation/palette, fades/cadence and
  DUNE0 AdLib track 4 signature. Mute/volume are respected; skipping stops audio.
- Following the campaign finale: original localized Dune II credits, names,
  bitmap fonts, eleven illustrations and DUNE20 track 2 music. Existing project
  attribution is preserved.
- Campaigns: full intermediate/final scenes for added houses. Scene models and
  opponents follow CampaignPlan.ini, including Lite. Names, subtitles and actor
  colours match their roles. Conquered planet and credits use the selected house
  colour, including bonus colours.

Automated audio checks validate decoded tracks, channel/volume and cleanup;
listening remains a separate human check. Native tests exercise actual SDL
rendering, commands and saves. Real UDP and determinism checks do not replace
an Internet match between people on independent connections. These are PC-game
changes; “mobile” referred to another Codex Work conversation, not an Android port.

Preparation validation (20 October 2026): engine source `fbe8b46270efc7635b9f3cc7446df90a3356e9c2`.
**2/2 CTest suites, 143 cases / 10,868 assertions**, including real UDP with guest
direct links blocked and Radar command transport up to eight players. Native
checks: **1,606 scenarios**, achievements, seven rules/editor/Chaos modes, research
for 55 house cases, **63 campaigns with three movies in each of French/English**.
Browser: **120 routes/contexts**, FR/EN desktop/mobile; **161 recovered site PNGs**
remain byte-identical.

[Linux/Mint CI](https://github.com/Torneko/dunelegacy-tornie/actions/runs/39468276567) and [Windows CI](https://github.com/Torneko/dunelegacy-tornie/actions/runs/39468271577) passed on the same engine
source, including installed-game, Radar and cinematic checks in both languages.
Package publication was disabled. [Validation receipt](Validation-Next-Release.json).
Audio checks are programmatic; human listening, physical Mint hardware and a human
Internet match on independent connections remain separate checks.
