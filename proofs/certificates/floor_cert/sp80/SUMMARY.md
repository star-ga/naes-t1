# sp80 official-engine floor certificate

No shorter win is found on any of the six levels.

Level 6: the official engine wins on the 28-action route and does not win on its 27-action prefix. No cheaper placement wins. The fixed-instance record of level 6 (withheld, sha256 `9736ca64f85128787fe0d61fc49004405ae336dc19a85fdcae1ee0231c564e39`) reaches the same floor by a separate enumeration.

| level | floor | verdict | placements tested | wins | unresolved | wall seconds |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | 4 | CONFIRMED | 12 engine spills | 0 | 0 | 0.028 |
| 2 | 7 | CONFIRMED | 610 | 0 | 0 | 0.001 |
| 3 | 10 | CONFIRMED | 20772 | 0 | 0 | 0.997 |
| 4 | 20 | CONFIRMED | 109006271 | 0 | 0 | 60.002 |
| 5 | 17 | CONFIRMED | 3532949 | 0 | 0 | 10.000 |
| 6 | 28 | CONFIRMED | 89202557 | 0 | 0 | 50.001 |

Official game program SHA-256 `2306d62311a3cc027250217d86cb4e012898edc7647a7c291ade23f36c9b9715`. Scanner source SHA-256 `2989062ee065340b819fef26699c9d86b849090c7f05a677f2e8ca5399d2d1fa` (withheld; see the Withheld section of `proofs/certificates/floor_cert/MANIFEST.md`).

## Account used here

`perform_action` (the engine program, lines 222-226) runs `step` until the action completes, and raises when the frame count passes 1000. ACTION5 arms the spill and returns without completing (the official game program, lines 716-724). Each later step moves the water once (the official game program, lines 749-821). The win check is the step after the frontier empties (the official game program, lines 728-747), which calls `next_level` (the engine program, lines 412-418). On levels 1-5 that raises the score and leaves the state unfinished. Level 6 sets the win state. A spill that is still running at movement tick 999 cannot be checked before the frame cap, so it is not a win.

Moves are one cell, after the rotation remap (the official game program, lines 695-714). If the only collisions are other movable pieces, the move is forced through (the official game program, lines 710-711). Walls, receptors, leaks and the border still block. Distances are breadth-first counts of official actions 1-4 on each piece alone. A second movable parked on the landing cell does not change the landing in 12 trials on levels 2-6 (0 mismatches). Level 1 has one movable.

A selection change is ACTION6 and does not move a piece. The initially selected piece can move with no click. Every other piece that moves costs one click. A failed spill costs one action, does not move pieces, and reselects one determined piece, so it is not a cheaper way to reach a placement. Reset restarts the level. Step budgets are 30, 45, 100, 120, 100 and 120, all above the floors, so a candidate shorter than the floor cannot time out.

Therefore a win shorter than floor F would be a placement whose move-plus-click cost is at most F-2, followed by one spill. Names are not unique on levels 2, 3 and 4. Graphs and positions use the sprite list index.

The spill decision for levels 2-6 is that rule transcribed into the withheld scanner source, including the dangling else on the corner branch (the official game program, lines 787-811). A droplet that walks past every solid in its travel direction cannot come back, so the frontier never empties and the frame cap raises. Those placements are non-wins, not unresolved. A frontier that overflows the droplet buffer would be unresolved. None does.

The transcription matches `get_sprite_at` on the origin grid of every level (0 mismatches on x, y in [-2, 24)). It agrees with an official ACTION5 on the origin (not a win) and on the placement under each recorded winning route (a win). The same program returns win on those six recorded placements and not on the six origins (`proofs/certificates/floor_cert/sp80/raw/crosscheck.txt`).

Level 1 does not use the transcription for the lower bound. Every display pixel at the start selects the only piece and moves nothing (4096 pixels, 0 position changes). The official engine then spilled all 12 cells at distance 0, 1 or 2. None won (`proofs/certificates/floor_cert/sp80/raw/L1.json`).

## Routes

`proofs/certificates/floor_cert/sp80/raw/engine_replay.txt`, SHA-256 `a2f529ec68d3d54ead44421a426f5546292ac816f92ff41f5cf8bdd6795c69a8`. Each certificate route wins at its recorded length. Each prefix does not.

| level | route length | win at | prefix score |
| --- | --- | --- | --- |
| 1 | 4 | 4, score 1, not finished | 0 |
| 2 | 7 | 7, score 1, not finished | 0 |
| 3 | 10 | 10, score 1, not finished | 0 |
| 4 | 20 | 20, score 1, not finished | 0 |
| 5 | 17 | 17, score 1, not finished | 0 |
| 6 | 28 | 28, state WIN, score 1 | 0 |

## Commands

Level 1 lower bound is the official-engine cell spill recorded in `proofs/certificates/floor_cert/sp80/raw/L1.json`. The scanner inputs `proofs/certificates/floor_cert/sp80/inputs/level2.bin` to `proofs/certificates/floor_cert/sp80/inputs/level6.bin` are packed by the withheld packer (SHA-256 in the Withheld section of `proofs/certificates/floor_cert/MANIFEST.md`) from `proofs/certificates/floor_cert/sp80/inputs/level<N>_model.json` (per-piece motion graphs built with the official step by the withheld certificate program, SHA-256 in the Withheld section of `proofs/certificates/floor_cert/MANIFEST.md`). The scanner is built from the withheld scanner source (C11 with POSIX threads). The second argument is the worker thread count; it does not change the tested set or the verdict.

Levels 2 and 3:

`./scan_sp80 proofs/certificates/floor_cert/sp80/inputs/level2.bin 1` and `./scan_sp80 proofs/certificates/floor_cert/sp80/inputs/level3.bin 1`

Both exit 0. Verbatim lines are in `proofs/certificates/floor_cert/sp80/raw/crosscheck.txt`:

`level 2 tested 610 wins 0 capped 0 best_cost -1 seconds 0.001`

`level 3 tested 20772 wins 0 capped 0 best_cost -1 seconds 0.997`

Levels 4, 5 and 6, one process each, several worker threads:

`./scan_sp80 proofs/certificates/floor_cert/sp80/inputs/level5.bin <threads>` then `./scan_sp80 proofs/certificates/floor_cert/sp80/inputs/level6.bin <threads>` then `./scan_sp80 proofs/certificates/floor_cert/sp80/inputs/level4.bin <threads>`

Exit 0 on each. Verbatim:

`level 5 tested 3532949 wins 0 capped 0 best_cost -1 seconds 10.000`

`level 6 tested 89202557 wins 0 capped 0 best_cost -1 seconds 50.001`

`level 4 tested 109006271 wins 0 capped 0 best_cost -1 seconds 60.002`

The printed seconds for levels 4-6 include up to 10 seconds spent waiting for a progress tick to exit. The progress log for level 6 (`proofs/certificates/floor_cert/sp80/raw/scan6.err`) advances through 89202557 placements across four ticks.

Scope: on levels 2-6 not every candidate is spilled on the official engine. The lower bound is the cost account above plus the transcribed spill, anchored to the engine on the rasters, the origins, and the six winning placements. Claim code: Cl for level 1 (official-engine spills under the action account), Cm for levels 2-6 (the transcribed spill is an explicit model of the water rule).

Path tokens: `<interp>` is the interpreter the `.py` programs are written for, `<threads>` is the worker thread count, `<path>` is a local checkout path. In files that carry a host path or an interpreter name, these tokens replace them; `proofs/certificates/floor_cert/MANIFEST.md` lists the digest of each original and of each included file.

Claim codes are defined in proofs/certificates/floor_cert/MANIFEST.md.
