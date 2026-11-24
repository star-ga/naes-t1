# Complete-key floor certificates: s5i5 L1, L2, L5 and vc33 L1, L2

These five floors carry claim code C (a current-engine minimum with no lemma or model condition). Each certificate is an exhaustive breadth-first search on the official engine whose state key and action alphabet rest on no premise: every field the key leaves out and every click the search merges is justified by the read-site certificate of the official files (`READSITES.json`) and by checks made during the search.

## The search

- Transition: the public `ARCBaseGame.perform_action` of the pinned engine (arcengine 0.9.3; file digests in `engine_SHA256SUMS`), unchanged, applied to a deep copy of the whole game object. `engine_SHA256SUMS` names `__init__.py`, `base_game.py`, `camera.py`, `enums.py`, `interfaces.py`, `level.py` and `sprites.py`. Those engine files are not in this pack. The list holds their digests.
- Start state: a fresh game, the opening RESET, then the recorded routes of the earlier levels (`proofs/certificates/complete_key/raw/routes.jsonl`) replayed through `perform_action`.
- State key: the SHA-256 of a canonical serialization of the whole object graph of the game: every attribute of every object, the type, shape and bytes of every array, sets compared as sets. Four fields are keyed by a token, each justified in `READSITES.json`, which lists every access to them in the official game file and in the engine:
  - `_action` (the last action input) and `_full_reset` (a flag of the returned frame) are written by `perform_action` before any read (the engine program lines 204, 218 and 276);
  - `_action_count` is keyed as zero or nonzero, the only test the engine applies to it besides the increment (`handle_reset`, the engine program line 313);
  - `current_steps`, the step counter, is read only by the counter itself, by the frame bar and by the loss test `not current_steps` (the official s5i5 program line 2245, the official vc33 program line 2122). The search asserts at every expanded state that the counter is positive (column `min counter`), so the loss test never fires inside the searched depth, and states that differ only in the counter have the same transitions there.
- The levels other than the current one, and the clean copies of all levels, are shared by every state and keyed by a token; their content digest is checked unchanged after every layer.
- Alphabet: RESET, the six keyed actions and ACTION6.
  - Mode `full`: ACTION6 at each of the 4,096 display pixels, at every state.
  - Mode `classes`: one ACTION6 per click class. The class of each of the 4,096 pixels is computed at that state by the engine's own `display_to_grid` and `get_sprite_at`; `READSITES.json` (`click_flow`) lists every read of the click data in `step`, which uses the click only through those calls (and, in s5i5, the sign of the offset against a slider's midpoint). At every state each class with two or more pixels is also played at its last pixel and must give the representative's successor key.
  - On vc33 L1 and L2 both modes visit the same states (8 and 90).
- RESET: at a nonzero action count it is the engine's level reset, and its successor is searched like any other. At a zero count (on entering the level, or after a level reset) it is the full reset: on level 1 its successor equals the state by content; on a later level it returns the game to level 1, which ends the attempt at this level, so the edge is counted and not expanded.
- Verdict CONFIRMED: no state at depth floor - 1 or less wins the level, the recorded route wins at the floor, and its (floor - 1)-prefix does not win. No searched action ends the game.

## Results

| game | level | floor | mode | states | edges | layers closed | min counter | seconds | verdict | record sha256 |
|---|---:|---:|---|---:|---:|---|---:|---:|---|---|
| vc33 | 1 | 3 | full | 8 | 20,515 | 1..2 | 49 | 54.449 | CONFIRMED | `6601578eef0e8636c7c95d56afeee692ad10a267b46767cb6ab283c7980d8b8a` |
| vc33 | 1 | 3 | classes | 8 | 75 | 1..2 | 49 | 0.384 | CONFIRMED | `55d78df091e95f6edc03f94d9726734b0b5494422aa43b7ebc66ac2cff112085` |
| vc33 | 2 | 7 | full | 90 | 291,313 | 1..6 | 45 | 806.515 | CONFIRMED | `acce296b1848dafdb9493bcaa11dfccc0c7d32066d9c8f9488ddd4a01e80face` |
| vc33 | 2 | 7 | classes | 90 | 1,333 | 1..6 | 45 | 5.802 | CONFIRMED | `dc6ca21346b4bf5bc6451154cdd751ae2867d25e6b586dcfd60ee84da1d37357` |
| s5i5 | 1 | 13 | full | 118 | 447,227 | 1..12 | 39 | 863.015 | CONFIRMED | `0c71d140f8762ec0081926fc157bedd4087e6a22eacfc706e8acbe8df9026699` |
| s5i5 | 1 | 13 | classes | 118 | 1,526 | 1..12 | 39 | 7.663 | CONFIRMED | `a1373d84224210adae6b3205f952d2798091528ef5001d9d12b9d9f931a15a50` |
| s5i5 | 2 | 26 | classes | 4,892 | 94,040 | 1..25 | 126 | 789.266 | CONFIRMED | `3a3c8e163611ecd5e8a5fada91e1ccf634aa70706186d08c0efb17b9357699fe` |
| s5i5 | 5 | 28 | classes | 3,751 | 71,160 | 1..27 | 124 | 741.541 | CONFIRMED | `48e92d23636f30f8b3ddb7e7bdf4c81c78e8b3fb87a3e063b4a507d59fb31221` |

## Premises of the recorded fresh queries, discharged

| row | recorded fresh query rests on | this certificate |
|---|---|---|
| s5i5 L1 | a raw-byte state key and a partition of the click alphabet | whole-object key; all 4,096 pixels at every state |
| s5i5 L2, L5 | a counter and sort quotient of the state and a complete click partition | whole-object key with no quotient, the counter keyed by the read-site certificate and the positive-counter check; click classes recomputed at every state by the engine's own calls, each checked at its last pixel |
| vc33 L1, L2 | key normalization, the counter and the alphabet | whole-object key; all 4,096 pixels at every state |

## Depth-one check of level 1

`depth1.json` applies every legal single action at the level-1 start of each of the 25 public demo games (the six keyed actions and ACTION6 at each of the 4,096 display pixels) through `perform_action`: no action wins level 1 of any game. ACTION6 with null coordinates (`{"x": null, "y": null}`) raises `TypeError` inside the engine in the 18 games that read the click, changes nothing in the other 7, and completes no level in any game. The game-file digests are in `games_SHA256SUMS`. `games_SHA256SUMS` names `games/ar25.py`, `games/bp35.py`, `games/cd82.py`, `games/cn04.py`, `games/dc22.py`, `games/ft09.py`, `games/g50t.py`, `games/ka59.py`, `games/lf52.py`, `games/lp85.py`, `games/ls20.py`, `games/m0r0.py`, `games/r11l.py`, `games/re86.py`, `games/s5i5.py`, `games/sb26.py`, `games/sc25.py`, `games/sk48.py`, `games/sp80.py`, `games/su15.py`, `games/tn36.py`, `games/tr87.py`, `games/tu93.py`, `games/vc33.py` and `games/wa30.py`. Those game files are not in this pack. The list holds their digests.

## Check commands

    <interp> <withheld search program> --game-file <official game file> --class-name S5i5|Vc33 --routes proofs/certificates/complete_key/raw/routes.jsonl --game s5i5|vc33 --level N --mode full|classes --out <record>.json
    <interp> <withheld read-site program> --game-file <official s5i5 program> --game-file <official vc33 program> --engine-dir <arcengine package> --out READSITES.json
    <interp> <withheld depth-one program> --games-dir <official game files> --out depth1.json

The programs are withheld. Their SHA-256 values are 145ed2c455aefa061e5d1c8fcb4f0d2bf63dcd10d2647999b341c48ef8432262 (search), 3efd23541e25e0b79448ecff5c3f7462b8a4495b66d27a03967ba3e45bd7571b (read sites) and 5806a0b8b52bde31679d2cec2a59dd42b736b0336505ec339318f7e8852dbb4b (depth one). Each record names the program, the official game file and the routes by SHA-256. `<interp>` is the interpreter the programs are written for. Verify the included bytes: `cd proofs/certificates/complete_key && sha256sum -c SHA256SUMS`.

Claim codes are defined in proofs/certificates/floor_cert/MANIFEST.md.
