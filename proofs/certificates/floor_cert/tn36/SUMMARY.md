# tn36 floor certification

The search finds no shorter win. All seven recorded floors are confirmed on the official engine.

Game sha256 `43c30052cdeb230017eb1ec23877050d96297756b86071417868ec61138c9fb7`.

| Level | Floor | Verdict |
| --- | --- | --- |
| 1 | 7 | CONFIRMED, engine BFS plus opcode DP |
| 2 | 9 | CONFIRMED, exact math |
| 3 | 9 | CONFIRMED, exact math |
| 4 | 12 | CONFIRMED, exact math |
| 5 | 16 | CONFIRMED, exact math |
| 6 | 16 | CONFIRMED, exact math |
| 7 | 16 | CONFIRMED, exact math |

## Action model

The engine exposes only action 6 (`available_actions=[6]`). `step` handles that action by mapping the click through `camera.display_to_grid` (`uihpdltbxp`); a miss becomes `(0, 0)`. The controller tries a program button, then the left panel, then the right panel (`rawolunrbj`). The left panel is a reference and returns without editing when `rpqwgvzwdv` is set. A right-panel click toggles a program bit or presses play. Play copies the right-panel words and runs every opcode (`mmmdksslmq`, `olsmwehruj`). The win flag is the right panel's piece match on x, y, scale, rotation, and color (`vklyonlcrw`).

The recorded proofs are program-cost lemmas or a checkpoint relaxation. The certificates below are checked against this handler.

Opcode step is 4. The only color writers are opcode 14 (color 9), opcode 15 (color 8), and opcode 63 (color 15). Nothing writes color 11. A missing opcode is a no-op.

## Levels 1-5

These levels have no collectible and no portal, and the reset flag is set. A failed run restores position, rotation, scale, color, and alive (`olrpupaury`). The home therefore never changes, and the words are the only memory. Hamming distance obeys the triangle inequality, so the toggles along any chain of programs are at least the hamming distance from the initial words to the last program, and at least one play is required. One program is optimal: its length is the minimum opcode hamming plus one play.

The exhaustive DP finds a program of that length equal to the recorded floor, with 0 mismatches against the engine on the per-slot opcode sample. The official-engine replay of each route wins at the floor, and the prefix does not (`proofs/certificates/floor_cert/tn36/logs/replay.json`).

| Level | Program | Length | Samples | DP seconds |
| --- | --- | --- | --- | --- |
| 1 | 3,3,3,3,3 | 7 | 41 | 0.572 |
| 2 | 33,33,33,33 | 9 | 278 | 3.751 |
| 3 | 0,0,33,10,10,33 | 9 | 404 | 5.652 |
| 4 | 1,3,9,3,3,3 | 12 | 404 | 5.399 |
| 5 | 3,3,3,5,8,63 | 16 | 404 | 6.356 |

Level 1 is also closed by breadth-first search on the engine: 6449 nodes, stopped false, closed through depth 5, no shorter win (`proofs/certificates/floor_cert/tn36/logs/L1.json`). The same search does not close levels 2-7; those certificates are the argument above and, for 6 and 7, the run enumeration below.

## Levels 6 and 7

Both recorded routes replay as wins at 16 clicks. The 15-click prefix does not win. There is no shorter win.

A failed run of six slots flips portal visibility after instruction index 2, when the index modulo 3 equals 2 (`jmwdvdqntf`), and the failure path flips every portal once more before reset. The phase of the next run is the original phase. Win is tested at the end pose before that failure-only flip. Level 6 has no portal. Level 7 has two.

The home changes only when the end pose overlaps a collectible of equal scale (`mnvoffrbex`). Every collectible is scale 1 and a 4 by 4 box on the same move lattice as the start, so a scale-1 piece overlaps one only when the origins coincide. A run cannot land on its own origin. The candidate homes are the four rotations of each collectible, at scale 1 and color 11.

Color opcodes cannot help. The target color is 11, and the only writers store 9, 8, or 15. A color change on the winning run misses the target. A color change that is then collected stores a color that no opcode can return to 11.

Scale is not excluded. The chain search includes nop, move, rotation, and scale.

Clicks are toggles plus the number of runs. A shorter win needs toggles + runs <= 15. The max edge popcount plus the run count is a lower bound, and deleting a cycle cannot reduce that quantity, so simple paths bound every path. The bound, extended until a zero-pop continuation can no longer finish under 16 clicks:

- Level 6: runs 2..9 cost at least 11, 12, 13, 14, 15, 16, 17, 18. Only runs 2..6 can be shorter than 16.
- Level 7: runs 2..8 cost at least 13, 11, 12, 13, 14, 15, 19. Only runs 2..7 can be shorter than 16. No longer simple path finishes under 16.

Those run counts are searched exactly. Runs 2 and 3 cannot return to the start (the first run cannot land on its own origin, and a three-run win cannot be rooted at the start because the start has no winning program). Runs 4 and above allow a home to be reused after a different home. The chain table matches an independent reference on runs 2 and 3 (144 and 1728 keys, 0 mismatches, 0 extra keys). A partial chain whose hamming already exceeds the toggle limit is dropped; every added run contributes a nonnegative hamming.

| Level | Runs | Tuples | Nodes | Capped | Best | Seconds |
| --- | --- | --- | --- | --- | --- | --- |
| 6 | 2 | 8 | 113 | 0 | none | 0.013 |
| 6 | 3 | 44 | 4868 | 0 | none | 0.333 |
| 6 | 4 | 294 | 31267 | 0 | none | 1.277 |
| 6 | 5 | 1454 | 69674 | 0 | none | 9.181 |
| 6 | 6 | 3656 | 61820 | 0 | none | 54.675 |
| 7 | 2 | 9 | 53 | 0 | none | 0.009 |
| 7 | 3 | 26 | 1180 | 0 | none | 0.210 |
| 7 | 4 | 320 | 13222 | 0 | none | 2.811 |
| 7 | 5 | 2400 | 31511 | 0 | none | 28.295 |
| 7 | 6 | 10512 | 58282 | 0 | none | 77.810 |
| 7 | 7 | 25052 | 46644 | 0 | none | 161.465 |

No tuple is truncated. The engine differential on both levels covers 87 programs with 0 mismatches, and the known two-program routes win in the model and on the engine at 16.

## Artifacts

- `proofs/certificates/floor_cert/tn36/logs/L1.json` sha256 `ae7cf9c80b752303bf596db18af5a84f5012f63e1716678d6795decf633173ed`
- `proofs/certificates/floor_cert/tn36/logs/exact2.json` sha256 `9674f8aeee9cd431dd9e132e20c64698ac2ec740b8d16d0091e1999fff4213d3`
- `proofs/certificates/floor_cert/tn36/logs/replay.json` sha256 `288134e4059897984a9a94dded74d86824aa060935c6c03fc672eff2aa7aa64a`
- `proofs/certificates/floor_cert/tn36/logs/k3.json` sha256 `7840cf0e66638c2cac728b3b0e968833c45d3057a359446feb4f627dba5294df`
- `proofs/certificates/floor_cert/tn36/logs/k5.json` sha256 `d0ef390c18abea5843aae6858a3c550d458a2534505c58a8261f682e3e46c449`
- The full search log is withheld. Its SHA-256 is `cf39d277e667e28758ee37aae5379cf08c39219d346e67c6b6eb33e96d54fcbd` (Withheld section of `proofs/certificates/floor_cert/MANIFEST.md` and of `SOURCE_SHA256.tsv`).
- `proofs/certificates/floor_cert/tn36/logs/bound.json` sha256 `9bb87068631129ed56775e88b69c5cad6ae6852c71317f36c1c9499d203674a9`
- `proofs/certificates/floor_cert/tn36/logs/k5_marks.txt` holds the marker lines of the withheld full search log.
- `proofs/certificates/floor_cert/tn36/logs/probe.json` sha256 `13310ec4da4a04416a44f0a0afae33e93b84e9e0a1eb34c64684cfeb4dfd7a8b`

The programs that produce these records are withheld. Each digest is in the Withheld section of `proofs/certificates/floor_cert/MANIFEST.md` and in `SOURCE_SHA256.tsv`.
Path tokens: `<interp>` is the interpreter the `.py` programs are written for.
