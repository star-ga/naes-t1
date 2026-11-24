# ft09 official-engine floor certificate

Shorter wins: none.

Every recorded floor is the minimum counted action length on the official engine. The six official replays win at 4, 7, 14, 16, 21 and 13. In each replay the completing action is the last action of that route, so the prefix one action shorter does not complete the level. Sum of the floors is 75.

| level | recorded floor | recorded method | verdict | nodes | seconds | evidence |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | 4 | lemma-cond | CONFIRMED | 296 | 137.498 | proofs/certificates/floor_cert/ft09/artifacts/L1.json |
| 2 | 7 | lemma-cond | CONFIRMED | 30940 | 267.507 | proofs/certificates/floor_cert/ft09/artifacts/L2.json |
| 3 | 14 | lemma-cond | CONFIRMED | 6144 | 90.831 | proofs/certificates/floor_cert/ft09/artifacts/L3.json |
| 4 | 16 | lemma-cond | CONFIRMED | 39366 | 189.082 | proofs/certificates/floor_cert/ft09/artifacts/L4.json |
| 5 | 21 | lemma-cond | CONFIRMED | 49152 | 144.304 | proofs/certificates/floor_cert/ft09/artifacts/L5.json |
| 6 | 13 | lemma-cond | CONFIRMED | 4096 | 193.641 | proofs/certificates/floor_cert/ft09/artifacts/L6.json |

Official game SHA-256 `aa5b54f48a29da9f276c3df0dc04de728002f523384ab868c85d56d819cd0783`. Engine versions arc-agi 0.9.9 and arcengine 0.9.3. Raw log `proofs/certificates/floor_cert/ft09/logs/certify.txt`, SHA-256 `e8578dbcd7c6bfaa8fc705ef4b4fba43dc11dceae44c690c72b25a667e30fb6f`.

Artifact SHA-256:

| level | sha256 |
| --- | --- |
| 1 | `7f7f09c2f5a97783a9b7b21793186679b0f66693f6e6c4efc7fc446f92fb0de7` |
| 2 | `a2e0d9ba5246d3d2cfce1f8ff9f7f87a106478e078361f8a5dfd3951bb823ed2` |
| 3 | `a3ce622bcd19ba4b67842f4fcaf6e2010d94f9422f904aa28c6d6bba52447f29` |
| 4 | `cce1f0f35be79f1da47b33b3626611f5e33695928d2266b90ab89cd40ae97467` |
| 5 | `aa653b92e3ad19325326ae7877b6e10b34fed2e0aba5597be1eb755010511fab` |
| 6 | `58655bdbe4904055d1975263d517bb54e54ec104bfbe31a9d66fb89bdca566b0` |

## Engine action model

`Ft09.__init__` builds `Camera(0, 0, 16, 16, 4, 4, [self.lpw])` with `available_actions=[6]` (the official game program, lines 2306). `set_level` resizes the camera to the level grid (the engine program, lines 160-164). Every level measured here has camera origin (0, 0) and size 32 by 32, so `display_to_grid` (the camera program, lines 318-350) uses scale 2 and padding 0. Each of the 64 by 64 display pixels lands on a grid cell.

`perform_action` (the engine program, lines 189-260) does not drop ids outside `available_actions`. It loops `step` until the action completes, so a multi-frame animation counts as one action. `_set_action` (the engine program, lines 278-279) increments `_action_count` for every id except RESET. `handle_reset` (the engine program, lines 305-316) full-resets when `_action_count` is 0 or the state is WIN, and otherwise level-resets. `set_level` (the engine program, lines 160) sets `_action_count` back to 0, which is what happens when a non-final level advances.

`step` (the official game program, lines 2350-2434) completes id 0 immediately. Id 6 maps the click through `display_to_grid`, then `get_sprite_at` for tag `Hkx`, then tag `NTi` (the official game program, lines 2369-2384). A hit sets `kMO` to 5. A miss on level 0 with the flash sprite present and no `bsT` under the click starts the flash and returns without completing (the official game program, lines 2386-2390); the same `perform_action` finishes the flash. Any other miss completes with no center change (the official game program, lines 2391-2392). Ids 1, 2, 3, 4, 5 and 7 leave the click coordinate unset, skip the toggle, then call `cgj` and the timer (the official game program, lines 2426-2434).

The toggle (the official game program, lines 2410-2424) adds one modulo the palette length to each selected center. An `Hkx` click uses the level kernel `irw`. An `NTi` click uses the sprite's pixels equal to 6 as the mask. `color_remap` recolors every pixel of the old center color on that sprite. The measured invariant is that an `Hkx` sprite stays uniform, and an `NTi` sprite keeps its 6s and keeps every other pixel equal to the center. Palette sizes are 2, 2, 2, 3, 2, 2. None contains 6. The win test `cgj` (the official game program, lines 2436-2520) reads only those centers and runs before the timer decrement (the official game program, lines 2426-2432). Timers at level start are 32, 32, 96, 96, 128, 128, each strictly above that level's floor.

## Account used here

From a clean level, all 4096 display pixels and actions 1, 2, 3, 4, 5 and 7 are applied on the official game object. Hit pixels are 288, 468, 828, 648, 1080 and 792, which is one opaque 3 by 3 sprite times 4 display pixels for 8, 13, 23, 18, 30 and 22 controls. The other pixels are center-noops. No pixel wins immediately. No pixel breaks the center invariant. Actions 1, 2, 3, 4, 5 and 7 do not change centers and do not win at the start (`cgj` is false on every level start). RESET does not increment the counted action total, and it restores a level the search already prices, so it cannot undercut the minimum.

The pixel partition is scanned again after one generator: 0 mismatches on every level. Every generator column is rechecked after every other generator, and a double click advances by 2 modulo the palette size. Those checks pass (pair counts 56, 156, 506, 306, 870, 462). The kernel in `step` does not depend on the current center colors, and the mask is invariant under the recolor, so the columns stay fixed on every reachable coloring.

`cgj` is encoded by the same neighbor offsets and the same `get_sprite_at` tags, then checked by painting. A satisfying assignment makes `cgj` true. Each forbidden center makes it false. Flipping an unconstrained center leaves it true. Constrained cells are 8, 13, 23, 18, 29 and 22. Level 5 has one unconstrained control. Skipped neighbor slots, which `cgj` ignores, are 0, 0, 0, 0, 21 and 8. The played arrival state matches an isolated `set_level` on every level.

The minimum number of clicks is the minimum coefficient sum, each coefficient in `0 .. k-1`, of those columns. Adding a full cycle of `k` clicks cannot reduce the sum. Levels 1 and 2 also brute-force every coefficient vector (256 and 8192) and agree: costs 4 and 7. Meet-in-the-middle costs are 4, 7, 14, 16, 21 and 13. Level 4 splits 3^9 by 3^9 and checks all 128 admissible targets. Each algebra route replays on `env.step` at exactly that length, after the recorded prefixes for the earlier levels.

Levels 1 and 2 also ran engine breadth-first search to depth floor-1. The state is the tuple of center indices. The actions are the distinct pixel effects. Level 1: depth cap 3, 93 states, 296 edges, not incomplete, no win. Level 2: depth cap 6, 4096 states, 30940 edges, not incomplete, no win. Those balls are the full sets of states at distance at most the cap. Levels 3-6 are certified by the column minimum above. Their depth balls are the radius-13 ball in 23 binary controls, the radius-15 ball in 18 ternary controls, the radius-20 ball in 30 binary controls, and the radius-12 ball in 22 binary controls.

## Command

```
<interp> <withheld certifying program> --out <out> --levels 1,2,3,4,5,6 --engine-bfs 1,2
```

The log ends with:

```
LEVEL 1 CONFIRMED nodes=296 time=137.498s
LEVEL 2 CONFIRMED nodes=30940 time=267.507s
LEVEL 3 CONFIRMED nodes=6144 time=90.831s
LEVEL 4 CONFIRMED nodes=39366 time=189.082s
LEVEL 5 CONFIRMED nodes=49152 time=144.304s
LEVEL 6 CONFIRMED nodes=4096 time=193.641s
DONE_LEVELS
```

The withheld certifying program (SHA-256 in the Withheld section of `proofs/certificates/floor_cert/MANIFEST.md`) is the certifying program; `<out>` is this directory. Claim code: Cl. Levels 1 and 2 are breadth-first searches on the official engine over the center tuple, and levels 3-6 are the exact column minimum; both rest on the measured pixel partition and center invariant above.

Path tokens: `<interp>` is the interpreter the `.py` programs are written for, `<path>` is a local checkout path. In files that carry a host path or an interpreter name, these tokens replace them; `proofs/certificates/floor_cert/MANIFEST.md` lists the digest of each original and of each included file, and the artifact digests in the table above are of the original files.

Claim codes are defined in proofs/certificates/floor_cert/MANIFEST.md.
