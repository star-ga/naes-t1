# Live scorecard

Public demo set: 100.00%. Scorecard https://arcprize.org/scorecards/46dc8e53-0718-4182-84a9-88ffe31e282e reports 100.00%, 183 of 183 levels, 25 of 25 games and 5,499 actions. It is a live Competition Mode play through the ARC Prize API on the public demo set, opened 2026-11-19 14:18:39 UTC and closed 2026-11-19 14:36:08 UTC. Measured by STARGA, not yet verified by ARC Prize.

The player was the NAES-T1 GPU decision server of 2026-11-19 with the parameters of the released weight file `model/naes_t1.bin`, driven by the scorecard client of that date. The server pins the digest of that weight file and admits only the GPU program emitted from it. It was not the release executable. The 5,499 replies of the play equal the decision stream of the release executable: SHA-256 `4d2d98f711a1209eace8f3841952e39d4d75870d48079c3175b361bba9746396` (`release_evidence/model_20261119/decisions.records.bin`; the run totals report `records_match` 5499 of 5499).

## Closed-loop replay by the release executable

`mode_a.json` is the record of a closed-loop replay. The release executable (its SHA-256 is in the record) plays the 25 public demo games through the gateway of the official ARC-AGI-3 toolkit (`arc-agi` 0.9.9, `arcengine` 0.9.3), offline, in Competition Mode, one play of each game. Result: 25 of 25 games, 183 of 183 levels, 5,499 actions, every level at its floor (`data/demo25_levels_mmae.csv`). The record holds the result line of each game.

## Files

- `scorecard.json`: the scorecard report that the paper's release gate reads (`scorecardurl`, `scorecardscore`, `scorecardactions`, `scorecardsha`; rule: Competition Mode, 25 of 25 games, 183 of 183 levels, `total_actions` equal to the 5,499 actions of the release run). Its fields come from the API close response and the card page; `field_sources` names the source of each field.
- `scorecard_tags.json`: the tag line of the scorecard, from `scorecard.json`.
- `api_close.txt`: lines copied byte for byte from the play log of the run: card open and close, the API close response (`competition_mode`, totals, score), the verdicts and the run totals (`replies_sha256` equal to the release decision stream). The log lines with timings, worker numbers and per-game progress are left out. In the run-totals line (`CARD_TOTAL`) three digests of private files read `<private>`: `weights_sha256` (a weight file that is not included), `stream_sha256` and `records_sha256` (two private run files). `release_evidence/SHA256SUMS` gives the digest of this copy. `scorecard.json` gives the digest of the original line set (`original_sha256`), which STARGA keeps.
- `card_page.txt`: the visible text of the public card page, fetched 2026-11-19 14:37 UTC, one text node per line. The name of one chat product in the page footer reads `<word withheld>`. It shows the score, the levels, the environments, the total actions and the 25 environment rows.
- `mode_a.json`: the record of the closed-loop replay by the release executable (section above).
- `SHA256SUMS`: the SHA-256 of each file of this directory. Verify: `cd release_evidence/play && sha256sum -c SHA256SUMS`.

The action count leaves out the opening RESET of each game, as the card page does (its column Resets reads 0 for every game) and as rule 6 of the play protocol of the paper does.
