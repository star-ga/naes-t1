# s5i5 official-engine floor certificate

The search finds no shorter win.

Confirmed on the official completion loop, each with the recorded route winning and its prefix not winning:

| level | floor | states | edges | layers closed | seconds | artifact sha256 |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | 13 | 116 | 749 | 1..12 | 0.49 | be30d17cee3b6c73b5cbdd614d14f50ca55795b8f2601479a560956601e3171d |
| 2 | 26 | 4890 | 61100 | 1..25 | 32.362 | 70e90870dfa90c7c515753420dd6e63541fd3a30ab0525dd55547aec57332db7 |
| 3 | 37 | 116575 | 2121141 | 1..36 | 1067.153 | 8d55d353592a9f75fe4b92a2a59626b153d0871d01e0fb3c6b85771278d45eae |
| 4 | 30 | 13881 | 221952 | 1..29 | 151.796 | 7391b13b06385812f9775eefba81c4224d895a8ea6cf105868abc7af0cba470f |
| 5 | 28 | 3749 | 46228 | 1..27 | 38.662 | e74fb984ff0b6adde7b23e53b7ed3a55ff2fe936e932d4dc807a37f2c8d22aaa |
| 6 | 25 | 72922 | 840372 | 1..24 | 166.292 | 15ba9c834744f58103820d20bfe26f3f2392ac9f0bf2d8174c772667229488f1 |
| 7 | 45 | 582724 | 11628960 | 1..44 | 3189.549 | 89af4ef3051d89614f80428d9b809fa10d49388fcd053b20fde8d589ae834587 |
| 8 | 36 | 27637 | 521060 | 1..35 | 369.15 | d0d3ea19856bf80d1ed862bd89af591db188c110d75ceea1e2c0a435b9d1c3b9 |

All eight levels are confirmed. Every level of s5i5 has a proven floor.

Command, one process per level (the withheld search program; SHA-256 in the Withheld section of `proofs/certificates/floor_cert/MANIFEST.md`): `<interp> <withheld search program> --game <official game program> --certs <level records> --level N --seconds 7000`, under an outer time limit of 7200 seconds.

## Action model used here

`ARCBaseGame.perform_action` (the engine program, lines 204-231) handles RESET, then calls `step` until `is_action_complete`. A colliding click returns from `step` before `complete_action` (the official game program, lines 2214-2215 and 2239-2240); the next `step` restores the undo snapshot (the official game program, lines 2182-2187). The search uses that whole loop. Camera render only writes a frame. Before each search one click is compared with rendered `perform_action`.

`s5i5.step` (the official game program, lines 2188-2242) reads a click only as `camera.display_to_grid` then `get_sprite_at` for tag `0089rvqdprjwpz` or tag `0066ghlkyvdbgg`. Every display pixel is played at the level start and matches its class representative: 0 class mismatches. Actions 1, 2, 3, 4, 5 and 7 leave the pose unchanged at the start, so they are self-loops. RESET at action count 0 is `full_reset` (the engine program, lines 305-316) and leaves a later level. After any click, RESET is `level_reset` back to the clean start. Neither is expanded.

The seen key is the pixels and positions of tag `0001qwdmnlybkb` and the `uricqfoplr` closure. `step` writes only those sprites. The step counter is omitted from the key: every expanded click ticks it once, each budget is above its floor, and the win test `neurwiqfry` (the official game program, lines 2080-2086) does not read it.

Claim code: Cl. The search runs the official completion loop, and two premises remain: the click classes are checked at each level start, and the seen key omits the step counter by the argument above.

Path tokens: `<interp>` is the interpreter the `.py` programs are written for. The search logs are included as `proofs/certificates/floor_cert/s5i5/raw/L<N>.log.txt`; `proofs/certificates/floor_cert/s5i5/raw/L1.sha256` and `proofs/certificates/floor_cert/s5i5/raw/finished.sha256` list paths relative to this directory. `proofs/certificates/floor_cert/s5i5/raw/L1.sha256` names `s5i5_floor_bfs.py`. That entry is not in the tree. The search program is withheld. `proofs/certificates/floor_cert/MANIFEST.md` lists its digest in the withheld table. `proofs/certificates/floor_cert/MANIFEST.md` lists the digest of each original and of each included file.

Claim codes are defined in proofs/certificates/floor_cert/MANIFEST.md.
