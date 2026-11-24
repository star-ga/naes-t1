PROVEN_11
# su15 level 3 (game su15-1944f8ab): no win in 10 clicks or fewer; minimum = 11 (prover p2)

Date 2026-11-19. Folder `proofs/certificates/su15_L3/p2/` (this folder). The logs of the second machine are in `proofs/certificates/su15_L3/p2/logs_nodeA/`.

Claim: every action sequence that wins su15 level 3 from our level 3 start state
`(0,8,28) (0,12,23) (0,31,15) (0,31,22) (0,55,23) (0,61,23) (1,18,16) (1,30,32) (1,46,22)` (value,x,y) has at least 11 actions.
Four different 11-click wins replay on the real engine (P5). Verdict PROVEN_11 rests on P1 to P5 below.

## 0. Method in five lines

1. Engine source read (lines cited below): a click moves ONLY the pieces it hits; merge/collision is tested ONLY among
   hit pieces; the win test runs after every movement tick and after the tick-4 merge; a merged group gets value v+1
   whatever its size. With R2 (every hit piece lands with its rectangle containing P, exhaustive) this gives: a
   successful click that loses no mass is a single-piece move or a same-value pair merge, or (pre-state exactly {v3,v2})
   a final click that may win mid-flight.
2. Hence a winning route is a forest of "lineages" (sets of original pieces that end in the v3 and in the v2); projecting
   the route on one lineage gives a legal route of the **interference-free relaxation**; the relaxation is solved
   exactly by exhaustive layered search over (piece subset, position) = 150 subsets x 4096 positions.
3. Its value at the start state is h(start) = 11. Two independent codes give it:
   `dpb.c` (simulator `simp2.h`, W table) and `proofs/certificates/su15_L3/p2/indep/indep_dp.py` (written from the engine source, landing tables taken
   from 23,744 real-engine calls, joint click computed per (click, tick) instead of a W table).
4. P4: iterative deepening with bound 10 and this h: the root is pruned (h=11>10), no win. Same code: bound 11 finds a
   win (positive control); control: with the mid-flight win test switched off, the search does not find the 11-action win.
5. Self-attack with the three adversarial lenses (sections R-ENGINE, R-LEMMA, R-SHORTER): no break, no 10-click win.

Design: p2 does not reuse the dynamic program; it builds from counting bounds and pattern
databases. Pass 1 measured the simple box/counting relaxation (`proto_box.py`, `proofs/certificates/su15_L3/p2/logs/proto_box.log`: "10 9" = 10
without, 9 with the joint click): it leaves h(start)=9 < 10, too weak to close a depth-10 search. The bound used here
is a pattern database in the exact sense (exact distance tables of the relaxed sub-problem "one lineage alone, its
sub-lineages never block each other", built by exhaustive search from the start positions), written from scratch, but it is the same
*family* of argument as the lineage DP of p1. A flaw common to every lineage argument would affect both proofs;
that is why P3 grounds every step in the engine source and in exhaustive machine checks.

## P1. Simulator = engine

Engine `su15.py` sha256 `a5f91f7c963d6ca6447dae0ab21342b48a3f511601c40dfa8e972bdc59b4651e` (the game file is not included; checked by `eng.py` on
every start, `proofs/certificates/su15_L3/p2/logs/engine_source_hash.log`). Proof simulator `simp2.h` sha256
`42fd13b8f1920a790c51069d9cb627a30f2e2c079fff99ac536d78dda542e21b` (same file on machine A, `proofs/certificates/su15_L3/p2/logs_nodeA/hashes.log`,
`proofs/certificates/su15_L3/p2/logs_nodeA/lemma_lens/hashes_bfs_nearwin.log`); `dpb.c` used on machine A sha256 `26d39c28...`, local `a0d76a06...` differs only by
the added `sub` mode, `W.bin` sha256 `93201b1b...`. All hashes: `proofs/certificates/su15_L3/p2/logs/hashes_all.log`,
`proofs/certificates/su15_L3/p2/logs/hashes_pass2.log`.

| test | log | verbatim result |
|---|---|---|
| 6000 engine-driven random walks (up to 14 steps) | `proofs/certificates/su15_L3/p2/logs/lock_walk_101.log` | `LOCKCHECK records=42192 mismatches=0 ... merges=8242 noop=2659 fail=658` |
| 150 deep states x all 4096 clicks (+walks) | `proofs/certificates/su15_L3/p2/logs/lock_walk_allpos.log` | `LOCKCHECK records=657343 mismatches=0 ... merges=34945 ... fail=26420` |
| 315 injected near-win states x 4096 ({v3,v2},{v3,v1,v1},{v2,v2,v2},{v3,v2,v0}) | `proofs/certificates/su15_L3/p2/logs_nodeA/lock_nearwin.log` | `LOCKCHECK records=1101824 mismatches=0 engine_wins=4580 ... excluded_prestate_already_won=188416` |
| sensitivity: same check with mid-flight win test removed | `proofs/certificates/su15_L3/p2/logs_nodeA/lock_nearwin_NEGCONTROL_nomid.log` | `mismatches=2179` (goes red) |
| sensitivity: click range 7 instead of 8 | `proofs/certificates/su15_L3/p2/logs/lock_NEGCONTROL_range7.log` | `mismatches=57854` (goes red) |
| (pass 2) engine landing tables vs simulator, every (value 0..3, click P, hit start position) | `proofs/certificates/su15_L3/p2/indep/tabcmp.log` | `TABCMP engine_table_entries=23744 sim_instances=4960920 mismatches=0` |
| (pass 2) mid-flight win TICK, 600 {v3,v2} pre-states near zones x every click hitting a piece, engine stepped tick by tick, compared with the own model used by `proofs/certificates/su15_L3/p2/indep/indep_dp.py` | `proofs/certificates/su15_L3/p2/engine_lens/tickwin_600.log` | `TICKWIN states 600 clicks 268872 mismatches 0`; wins at tick 1: 18929, tick 2: 5750, tick 3: 104 (model = engine for every click) |
| (pass 2) own trajectory model vs engine, tick by tick | `proofs/certificates/su15_L3/p2/indep/indep_dp.log` | `TRAJCHECK samples 1500 mismatches 0` |

Total states with all 4096 clicks: 150 + 269 (46 already-won injected pre-states excluded: they cannot occur on a route,
the click that made them would have won) = 419 >= 300; walks 6000 >= 5000. Engine constants read live at level 3
(`proofs/certificates/su15_L3/p2/indep/indep_dp.log` line `CONSTS`): `R 8 STEP 4 TICKS 4 YMIN 10 YMAX 63 W 64 H 64 SZ [1, 2, 3, 4] ZONES [(5, 46, 9, 9),
(19, 46, 9, 9)] GOAL {3: 1, 2: 1}`, no enemies (`fezhhzhih=[]`), step budget 48.

## P2. Complete actions, exact merging, undo

* Every table and every search loops over all 4096 click positions (`dpb.c`: `build_tables`, `buildW`, `ida`, `cons`,
  `bfs`; `proofs/certificates/su15_L3/p2/indep/indep_dp.py`: `CLICKS` = all P in rows 10..62, rows outside do nothing in the engine: `axaxyjxqoe` returns at
  once if `grid_y <= 9 or grid_y >= 63`, source line 1136; also covered by the 64x64 lockstep). Clicks outside 0..63 are
  no-ops (`proofs/certificates/su15_L3/p2/logs/click_range.log`).
* Merging of actions is exact only: successor states de-duplicated by full state equality (`cmpst`); landing tables
  merge start positions only where the landing is proven identical: R2 and R3 below (exhaustive), plus `TABCMP` above.
* Undo (ACTION7, source `step` line 1129 -> `vczehveskr` line 2094) pops the snapshot pushed by `hlsimbcfgc` (line 2083)
  at the start of every in-grid click; a collision also pops its own snapshot (state `ivqtcpakv` -> `vczehveskr`). So
  the stack always holds the pre-click states of the clicks not yet undone, and the current state equals the state
  reached by applying those clicks in order. Deleting every undone click and its undo gives a shorter action list that
  reaches the same states in the same order; undo is never followed by a win test (ACTION7 branch has no
  `cbdhpcilgb`), and a restored state is never already winning (every state on a route is produced by a click whose
  win test did not fire). Machine check: `proofs/certificates/su15_L3/p2/logs/undo_check.log`
  `UNDOCHECK walks=6000 clicks=46308 undos=19974 undo_state_mismatch=0 undo_to_unvisited_state=0`.
* Level entry: `on_set_level` (source lines 966-1041) sets the history to `[]` and pushes the level-3 start snapshot
  (line 1040-1041), so an undo at the start of level 3 restores the start state itself; no level-2 state can come back.
* No-op and failed clicks leave the fruit state unchanged (P1); the step budget (48) and penalties can only cause a
  loss, never a win, so deleting them never hurts. Hence: a win in L actions implies a win with <= L successful clicks.

## P3. Rules: one sentence, one check, one log

Sigma = sum of 2^v over pieces; start Sigma = 6x1 + 3x2 = 12 = 2^3 + 2^2 (goal).

| # | rule | check | log (verbatim) |
|---|---|---|---|
| R1 | No click raises Sigma (merge of k>=2 pieces of value v gives one piece of value v+1, 2^(v+1) <= k 2^v; no other piece source at level 3), the goal needs Sigma >= 12, so every click of a winning route is lossless and a mid-flight win starts from exactly {v3, v2}; the search may drop lossy children. | source `nhtlfoopnx` lines 1331-1395 (value `pkjjooafkc + 1` for any group size); `compcheck` on all engine-checked records | `proofs/certificates/su15_L3/p2/logs_nodeA/compcheck_nearwin.log` `COMPCHECK records=1290240 composition_mismatches=0 sum_increase=0`; `proofs/certificates/su15_L3/p2/logs/compcheck_1.log` `records=657343 ... sum_increase=0`; `proofs/certificates/su15_L3/p2/logs_nodeA/lemma_triple.log` `LEMMA_TRIPLE sampled=7453049 lossy_single_piece=7453049 mismatches=0` |
| R2 | Every piece hit by a click P lands at a position that depends only on (value, P) and its rectangle contains P. | exhaustive: simulator all values 0..8; own engine-checked model all values 0..3 vs ENGINE landing | `proofs/certificates/su15_L3/p2/logs_nodeA/lemma_reach.log` `LEMMA_REACH instances=8651902 mismatches=0`; `proofs/certificates/su15_L3/p2/indep/r2_ownmodel.log` `R2_OWNMODEL instances=2907572 landing_mismatch_vs_engine_FP=0 landed_rect_not_containing_P=0` |
| R3 | Two pieces of equal value v<=2 hit by one click always merge, losslessly, into one piece of value v+1 at a position that depends only on P. | exhaustive over all ordered position pairs (and follows from R2 + merge at mean of centres) | `proofs/certificates/su15_L3/p2/logs_nodeA/lemma_pair_{0,1,2}.log` `instances=108848034 / 144002054 / 184264116 mismatches=0` |
| R4 | Two pieces of different values hit by one click always collide (click undone) unless the pair is {v3,v2} and the win test fires at tick 1, 2 or 3. | exhaustive over all position pairs, 6 value pairs; source `mdetahtgad` lines 1224-1279 (collision on first overlapping mixed pair, before the tick-4 win test) | `proofs/certificates/su15_L3/p2/logs_nodeA/lemma_mixed_*.log` 973,353,480 instances, `mismatches=0`, `win_events` only for (2,3): 12966600 |
| R5 | So a successful lossless click is (a) a single-piece move to FP[v][P], (b) a pair merge to GP[v][P], or (c) the final click from {v3,v2}; all other pieces stay where they are. | R1-R4 + source (`axaxyjxqoe` lines 1135-1160: only `xxkedcuzq` = hit pieces move, `pkrdtzfrth` lines 1505-1578; merge set = hit pieces only) + composition test (hit subset alone gives the same result) | `proofs/certificates/su15_L3/p2/logs_nodeA/compcheck_nearwin.log` `composition_mismatches=0` (hit-set sizes 1: 115141, 2: 32780, >=3: 7110) |
| R6 | Projection: in a lossless winning route every click acts on pieces of one lineage only (a merge across T3 and T2 would put an original piece in both final pieces), so the clicks charged to lineage T, in order, form a route of the relaxed one-lineage problem, and L >= D[T3][a] + D[T2][b] (+1 for a joint final click). | argument (this file, below) whose every step is R1-R5 | -- |
| R7 | Relaxation solved exactly: D[T][p] = least relaxed clicks to turn original pieces T into one piece at p, by layered search over 150 subsets x 4096 positions with move and merge edges taken from the simulator/engine. | two independent codes agree; small-instance D equals exact BFS of the real simulator | `proofs/certificates/su15_L3/p2/logs_nodeA/root.log` `ROOT_BOUND 11 normal=12 joint=11`; `proofs/certificates/su15_L3/p2/indep/indep_dp.log` `INDEP_ROOT_BOUND 11 normal=12 joint=11`; `proofs/certificates/su15_L3/p2/logs/sub_pairs.log`, `sub_triples.log`, `sub_quads.log` (9 sub-instances of 2, 3, 4 pieces, BFS depth 3-4): `dp_greater(VIOLATION)=0` everywhere; 9579 of 9583 BFS-reached positions have D = exact, 4 have D < exact |
| R8 | Final click: the win set W of {v3 at a, v2 at b} pre-states that one click wins (both hit and win at tick 1-3, or one hit with the other standing in a zone) is complete. | built by calling the simulator for every P, every hit pair and every hit piece with the other in a zone; every engine win from a 2-piece pre-state is in W; independent per-(P,tick) computation gives the same root value | `proofs/certificates/su15_L3/p2/logs_nodeA/buildW.log` `BUILDW events=21530653 distinct_pairs=555265`; `proofs/certificates/su15_L3/p2/logs_nodeA/wcheck_nearwin.log` `WCHECK engine_wins=149327 on_2piece_prestate=60775 not_in_W=0`; `proofs/certificates/su15_L3/p2/engine_lens/tickwin_600.log` |
| R9 | h(s) = min over partitions (T3 mass 8, T2 mass 4) of min( min_zone D[T3] + min_zone D[T2], min over (a,b) in W of D[T3][a] + D[T2][b] + 1 ). | `bound()` in `dpb.c`; consistency and BFS checks below | see R-LEMMA |
| R10 | Cost cap 14: relaxed values above 14 are stored as "unreachable"; every relaxed value <= 14 is exact because all parts of a relaxed route of cost <= 14 cost <= 14, so h <= 10 would be found if it existed. | definition `CAPC` | -- |

**R6 in full.** Take a winning action list of length L and reduce it (P2) to <= L successful clicks; by R1
all are lossless. Give every piece its *content* (the original pieces merged into it). Merges unite contents; nothing is
lost (R1), so the final v3 and v2 have contents T3, T2 that partition the 9 originals (mass 8 and 4), and every piece at
every time has content inside T3 or inside T2. By R5 a click is a move of one piece or a merge of two equal-value pieces
(both hit; their contents are disjoint and lie in the same final piece) or the final {v3,v2} click. Charge each move or
merge to the content of the piece it produces. By induction over the route: if a piece with content S stands at p, the
clicks charged to subsets of S number at least D[S][p] (move: p in Hit_v(P) and lands at FP[v][P] by R2, so
D[S][FP] <= D[S][p]+1; merge of S1 at p1 and S2 at p2 both in Hit_v(P), lands at GP[v][P] by R3, so
D[S][GP] <= A[S1][P]+A[S2][P]+1 <= D[S1][p1]+D[S2][p2]+1). The relaxation ignores the interaction between lineages
(collisions with other pieces, the need to avoid hitting them), which only removes constraints. Final click, two
cases. (i) Its pre-state is exactly {v3 at a, v2 at b}: whatever the click does (both hit and win at tick 1-3, one hit
passing through or landing in a zone while the other stands in a zone), (a,b) is in W by construction (R8), and the
final click is charged to neither lineage: L >= D[T3][a]+D[T2][b]+1 ("joint"). (ii) Otherwise the pre-state has more
than two pieces, so by R1 the win cannot fire before the tick-4 merge; the final click is then a merge charged to one
lineage and the post-state rests with the v3 and the v2 centred in zones: L >= D[T3][a]+D[T2][b] with a, b zone
positions ("normal"). So L >= h(start).

## P4. Exhaustion (bound 10) and controls

`dpb ida W.bin 10` (iterative deepening, h = R9 at every node, children = every distinct lossless successor over all
4096 clicks), `proofs/certificates/su15_L3/p2/logs_nodeA/ida_bound10.log`:

```
IDA bound=10 result=NO_WIN_WITHIN_BOUND
IDA depth 0: nodes=1 pruned_by_h=1 children_generated=0 tt_hits=0      (h(start)=11 > 10)
IDA depth 1..10: nodes=0
```
Nodes visited 1 (the root), expanded 0, distinct states 1, prunes: rule R9 1 at depth 0, other rules 0 (lossy
children dropped by R1 and failed/no-op clicks dropped by P2 only matter below the root). The real work is the exhaustive
relaxed search (`proofs/certificates/su15_L3/p2/logs_nodeA/root.log`): (subset, position) cells by cost
`0:9 1:2061 2:6367 3:13717 4:25383 5:36560 6:50130 7:67540 8:74179 9:73007 10:62145 11:40120 12:19472 13:6606 14:1126`,
`valid_subsets=150`. The independent code (`proofs/certificates/su15_L3/p2/indep/indep_dp.log`) explores the same 150 subsets
(`DP subsets 150`), 63 partitions, best `(11, 12, 11, ...)`.

* Positive control, same code, bound 11 (`proofs/certificates/su15_L3/p2/logs_nodeA/ida_bound11_POSCONTROL.log`): `IDA_WIN depth 11`,
  `IDA_ROUTE 4,23 12,22 13,31 17,39 28,21 33,27 55,27 47,30 39,34 31,40 23,46` (replays, P5); per-depth nodes
  1,1,1,1,1,5,3833,4760,1,1,1 with children 993,1000,970,968,930,... (depth-1 children 993 incl. the no-op state, cf. the
  measured 992 distinct states at depth 1).
* Control: with the joint/mid-flight win test switched off (`-DNO_MIDFLIGHT`, W rebuilt), the bound-11 search does not find the 11-action win (`proofs/certificates/su15_L3/p2/logs_nodeA/ida_bound11_MUTATION_nomid.log`: `IDA bound=11 result=NO_WIN_WITHIN_BOUND`, root pruned). The independent code gives the bound 11 (`proofs/certificates/su15_L3/p2/indep/indep_dp.log`).

## P5. Replay on the real engine

Replays: `proofs/certificates/su15_L3/p2/logs/replay_11.log`, `proofs/certificates/su15_L3/p2/logs/replay_beam11.log`; each route prints
`result(actions_to_complete,state) (11, ...)` and the negative control (drop last action) `(0, 'NOT_FINISHED')`:
* `55,28 47,31 14,24 34,22 33,30 22,24 40,36 27,31 23,39 32,42 23,46` (given)
* `55,28 46,28 12,31 31,23 27,30 18,25 19,34 17,43 39,34 32,40 25,46` (given)
* `4,23 12,22 13,31 17,39 28,21 33,27 55,27 47,30 39,34 31,40 23,46` (p2 IDA bound 11)
* `34,22 14,24 55,28 37,29 47,31 40,37 32,43 19,25 20,34 21,43 23,46` (p2 beam, depth 11, `proofs/certificates/su15_L3/p2/logs_nodeA/shorter/beam_20000_1_8_1.0_11.log`)
No route of 10 or fewer clicks was found by any search.

## Self-attack (adversarial lenses), pass 2

**R-ENGINE.** Tried: (1) read the engine click path line by line (`step` 1043-1133, `axaxyjxqoe`, `pkrdtzfrth`,
`sgmsqapcxe` (enemy code only; level 3 has no enemies), `mdetahtgad`, `nhtlfoopnx`, `cbdhpcilgb`, `jnieciwfsv`,
`kcqeohsztd`, `hlsimbcfgc`, `vczehveskr`): order per tick = move hit pieces -> (tick 4: merge/collision, return on
collision) -> win test; matches `simp2.h`. (2) 23,744 real-engine calls for every (value, P) landing and pair merge, compared
with the simulator for all 4,960,920 start positions: 0 mismatches. (3) 268,872 engine clicks on {v3,v2} states near the
zones, win tick checked: 0 mismatches, wins at ticks 1, 2, 3 all present. (4) borders: all landing tables include P in
rows 10 and 62 and columns 0 and 63 and every clamped start position (R2 exhaustive, `R2_OWNMODEL` 0 mismatches); pieces exactly at
distance 8 are inside the exhaustive hit sets (`inrange` = engine `kcqeohsztd`, `<= R*R`). (5) budget 48 > 11, penalties
only lose. Not done: engine-level lockstep of groups of three or more beyond the pass 1 records (7,110 hit sets
of size >= 3 in the near-win lockstep, all matching).

**R-LEMMA.** Tried: (1) BFS with the real simulator (all 4096 clicks, exact de-dup) to depth 3 from 185 states at small
true distance (states of three 11-click wins at distance 1..6, random children and grandchildren),
`proofs/certificates/su15_L3/p2/logs_nodeA/lemma_lens/bfs_nw_*.log`, re-tallied in `proofs/certificates/su15_L3/p2/lemma_lens/nearwin_bfs_retally.log`:
`NEARWIN_BFS_RETALLY rows=185 exact_found_h_equal=133 exact_found_h_less=0 h_greater_than_exact(REAL VIOLATIONS)=0 no_win_within_reached_depth(no information)=52`.
h equals the true distance on all 133 states where the BFS reached a win. (2a) Consistency at the root over ALL its successors: `proofs/certificates/su15_L3/p2/logs_nodeA/lemma_lens/cons_root_and_hashes.log`
`CONS states=1 children=993 violations=0`, i.e. every one of the 992 distinct depth-1 states (+ the unchanged start) has
h >= 10, so with bound 10 the whole depth-1 layer would be pruned as well; no depth-1 win. (2c) Consistency on 60 random
depth-1 states (in parallel on a second machine, same dpb, W.bin and simp2.h hashes `proofs/certificates/su15_L3/p2/logs_nodeB/lemma_lens/hashes_nodeB.log`):
`proofs/certificates/su15_L3/p2/logs_nodeB/lemma_lens/cons_nodeB_total.log` `CONS_NODEB_TOTAL files 20 states 60 children 61105 violations 0`, i.e. h(s) <= 1 + h(s')
for all 61,105 depth-2 successors of those states. Same on 72 other random depth-1 states (in parallel on a second machine, hashes `proofs/certificates/su15_L3/p2/logs_nodeA/lemma_lens/hashes_cons_depth1.log`):
`proofs/certificates/su15_L3/p2/logs_nodeA/lemma_lens/cons_nodeA_d1_total.log` `CONS_NODEA_D1_TOTAL files 8 states 72 children 74533 violations 0`. And on 34 further depth-1 states (machine B): `proofs/certificates/su15_L3/p2/logs_nodeB/lemma_lens/cons_nodeBb_total.log` `CONS_NODEBB_TOTAL completed_files 18 states 34
children 34023 violations 0` (6 more states stopped unfinished). In total 166 of the 992 depth-1 states,
169,661 depth-2 successors, 0 violations. (2b) Consistency h(s) <= 1 + h(s') over every
real successor: `proofs/certificates/su15_L3/p2/logs_nodeA/old_checks/cons_*.log` 7 runs, 216 states, 180,294 children, `violations=0`. (3) Sub-problem
equality D = exact BFS (`proofs/certificates/su15_L3/p2/logs/sub_*.log`). (4) Note on the checker (not the bound): `dpb bfs` prints
`VIOLATION h=X but no win within depth d` whenever no win is found within d and h > d+1; that is not a violation (the
true distance is then > d); the 40 such lines in `proofs/certificates/su15_L3/p2/logs_nodeA/old_checks/bfs_shard_*.log` (pass 1, states far from a win)
and 51 in the new logs are of this kind; the number of real violation lines (`VIOLATION h=X exact=Y`) is 0 in all logs.
(5) The relaxation allows every real move: R5 + composition test; the relaxed edges are a superset because the relaxation
drops the "do not hit other pieces" constraint.

**R-SHORTER.** Tried: 7 beam searches to depth 10 on the exact simulator with heuristics that do NOT use the lineage bound
(zone distance, partner distance, pure random; width 20,000; `proofs/certificates/su15_L3/p2/shorter/beam10.c`, logs `proofs/certificates/su15_L3/p2/logs_nodeA/shorter/beam_*_10.log`):
all `BEAM_NO_WIN within 10`, 659,431,424 simulated clicks each. Positive control: same program with depth 11 finds an
11-click win that replays on the engine.  Plans that use a collision, penalty or
undo on purpose cannot help (P2). Plans with two joint clicks are impossible (a joint click only exists from {v3,v2},
i.e. it is the last click). A merge of three loses mass (R1).

## Independent check (`proofs/certificates/su15_L3/check/`)

The independent check found no break and no win in at most 10 clicks. It reproduced with its own
code: the p2 cost histogram 0..14 and `ROOT_BOUND 11`, R2 (8,651,902 instances), R4 win
events (12,966,600), W size (d0+d1 = 555,265 = `BUILDW distinct_pairs`), the depth-1/2 counts 992 / 453,398, h >= 10 on all
993 depth-1 states, and an exact distance table of all 10,029,864 {v3,v2} states with h <= exact on 3,015 sampled states.

## Scope notes

* The pruning is ONE relaxation. Its soundness = R6 (an argument in words) on top of machine-checked R1-R5 and the
  engine source.
* Same method family as p1; the proofs are independent in code and checks, not method.
* The IDA transposition table keys on a 64-bit hash; it is never used in the bound-10 run (root pruned).
* `dpb.c` local copy (sha `a0d76a06...`) differs from the machine A copy that produced the IDA/root logs (sha `26d39c28...`) only by
  the `sub` mode and a `g_sub` flag that is 0 in all proof modes.
