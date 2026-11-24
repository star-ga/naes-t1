PROVEN_11
# su15 level 3 (game su15-1944f8ab): no win in 10 clicks or fewer. Prover p1 (lineage-tree DP, made explicit and machine-checked)

Folder: `proofs/certificates/su15_L3/p1/` (this folder).
Engine source su15.py sha256 a5f91f7c963d6ca6447dae0ab21342b48a3f511601c40dfa8e972bdc59b4651e (checked by simlib.check_sha in every engine run).
Simulator used by EVERY check and by the search: simc.h sha256 fa1b9089f9d7065a86f30a95c0956e38e29db2cd80199d37e50ed38e53d51a83.
Bound: p1dp.h sha256 ed765a614c72544f7c28ef3d5b993af9913f07995b7f789af0a3609b60c3d84a ; search ida.c sha256 47412a4f5b811754bc51ae565f1fc525ec16bd99005a47cdfad255730ec9125f.
All hashes: proofs/certificates/su15_L3/p1/logs/hashes_final.txt and proofs/certificates/su15_L3/p1/logs/nodeA/hashes_nodeA_chain4.txt, proofs/certificates/su15_L3/p1/logs/nodeA/hashes_exh5.txt (machine A copies, identical).

## The argument in 12 lines
1. State = multiset of pieces (value, x, y). Level 3 start: six v0 (1x1), three v1 (2x2); mass = sum 2^v = 12. Win = one v3 (4x4) and one v2 (3x3) centred in a zone (5,46,9,9)/(19,46,9,9), tested after every movement tick.
2. Undo only ever restores a state already reached earlier by an undo-free route that is not longer (R_UNDO), so undo cannot shorten a win; noop and fail clicks leave the state unchanged (R_CLASS).
3. Every hit piece lands with its centre on the click point P (clamped); hence (R_CLASS) a click is: noop, single move (one piece -> landc(v,P)), pair merge (two equal pieces -> one v+1), fail (different values meet: state unchanged), lossy (3+ equal pieces: mass drops), or a win.
4. Mass never rises; a win needs mass 8+4 = 12 exactly, so any lossy click kills the route (R_MASS), and at the winning moment exactly two pieces exist, {v3, v2}.
5. So a shortest win is a sequence of single moves and pair merges, each touching exactly ONE node of the lineage forest (which start pieces a piece descends from), plus a final click.
6. The final click is one of F0 (after a move/merge the v3 and v2 sit in zones), F1 (pre-state {v3,v2}, both hit, both centred in zones at the same tick 1..3: the joint click), F2 (pre-state {v3,v2}, one hit, it passes through a zone at tick 1..3 while the other already sits in a zone) (R_FINAL, exhaustive check).
7. Relaxation: drop all interactions between lineages (a click may move a piece even if other pieces are near). Each lineage subtree T then costs D[T][pos] = exact minimum in the relaxed problem (dynamic program over subsets with the real in-range sets and real landing/merge positions).
8. Every real useful click is a legal relaxed move charged to exactly one subtree, so h(s) = min over final rules F0/F1/F2 of the summed subtree costs is <= the true number of clicks still needed (R_H).
9. h(start) = 11 (minimum over F0, F1, F2; F1 gives 11).
10. Exhaustion with bound 10 (root prune disabled): the root is clicked at all 4096 points, giving 992 distinct children; every child has h >= 10 > 9 clicks left, so all are pruned; no win.
11. Controls: bound 11 finds an 11-click win (replayed on the real engine: completes at action 11, not before). Control: with the mid-flight win test switched off, the search does not find the 11-action win.
12. Self-attack: true distances by exhaustive BFS (<=3 clicks) on random near-win states never fall below h; targeted engine lockstep (borders, 3+ merges, distance exactly 8) 0 mismatches; undo invariant 0 violations.

## P1 SIMULATOR = ENGINE (simc.h via libsimc.so built from wrap.c + simc.h; same simc.h compiled into every checker and the search)
| check | command | log | verbatim result line |
|---|---|---|---|
| random walks incl. undo, state AND undo stack compared each step | `python lock_engine.py walks 21 6000 <MAXLEN>` / `walks 22 10000 <MAXLEN>` (run by the earlier p1 pass; MAXLEN not recorded in the log) | proofs/certificates/su15_L3/p1/logs/lock_walks_s21.log, proofs/certificates/su15_L3/p1/logs/lock_walks_s22.log | `LOCKSTEP_WALKS seed 21 walks 6000 steps 45333 mismatch 0 ...` ; `LOCKSTEP_WALKS seed 22 walks 10000 steps 153740 mismatch 0 wins 0 gameover 289 {... 'undo': 9365}` |
| states x all 4096 clicks (near-win) | `python lock_engine.py allpos 11 160 near` | proofs/certificates/su15_L3/p1/logs/lock_allpos_near_s11.log | `LOCKSTEP_ALLPOS seed 11 kind near states 160 clicks 655360 mismatch 0 wins_matched 3632` |
| states x all 4096 clicks (near-win), rerun this pass | `python lock_engine.py allpos 13 160 near` | proofs/certificates/su15_L3/p1/logs/selfcheck/lock_allpos_near_s13.log | `LOCKSTEP_ALLPOS seed 13 kind near states 160 clicks 655360 mismatch 0 wins_matched 3860 {'ok': 62685, 'noop': 575080, 'fail': 13735, 'win': 3860} lossy_clicks 996` |
| states x all 4096 clicks (mixed reach/near/rand) | `python lock_engine.py allpos 12 160 mix` | proofs/certificates/su15_L3/p1/logs/lock_allpos_mix_s12.log | `LOCKSTEP_ALLPOS seed 12 kind mix states 160 clicks 655360 mismatch 0 wins_matched 1127` |
| walks rerun this pass with recorded commands | `python lock_engine.py walks 23 6000 15` / `walks 24 10000 30` | proofs/certificates/su15_L3/p1/logs/selfcheck/lock_walks_s23.log, _s24.log | `LOCKSTEP_WALKS seed 23 walks 6000 steps 48416 mismatch 0 ...` ; `LOCKSTEP_WALKS seed 24 walks 10000 steps 153312 mismatch 0 wins 0 gameover 264 {... 'undo': 9211}` |
| mid-flight wins (win test on every tick) | `python lock_midflight.py 60 <seed>` (earlier p1 pass) | proofs/certificates/su15_L3/p1/logs/lock_midflight.log | `LOCKSTEP_MIDFLIGHT states 60 clicks 203520 engine_wins 1980 engine_wins_only_mid_flight(mutant says no) 1504 engine_wins_also_end_state 476 mismatch 0` |
| targeted (self-attack, engine lens) | `python lock_targeted.py 41 15 border|cluster|d8` (+ seed 42, 40 states) | proofs/certificates/su15_L3/p1/logs/selfcheck/lock_targeted_*.log | `LOCK_TARGETED kind cluster seed 41 states 15 clicks 61440 mismatch 0 ... lossy_clicks 1384` (all kinds mismatch 0) |
Totals: 32,000 walks (400,801 steps, 24,221 undos) + 7,500 undo-check walks (263,634 actions); 480 (allpos) + 60 (midflight) + 165 (targeted) + 60 (ticks) = 765 states with all click positions; 0 mismatches anywhere. Start state is asserted equal to the engine's level-3 start (lock_engine.py `assert eng_state(g0)==L3`).
Known, harmless difference: on an ALREADY-WON state a click that hits nothing runs the engine's win test (engine wins) while simc reports noop. Every state on a route is non-won (each was produced by a click whose result was win-tested, or restored by undo from such a state), so this never matters; the lockstep scripts skip injected pre-won states for this reason.

## P2 COMPLETE ACTIONS
- Every expanded state is clicked at all 64x64 = 4096 positions (ida.c `children()`: `for cy<64 for cx<64`); log line per depth shows `clicks=4096`.
- R_DEDUPE: children are merged only if their canonical sorted (v,x,y) lists are identical (`cmpst==0`); a win is detected on the click itself before any merging. Exact by construction.
- R_UNDO (one sentence): an undo always lands on a state already reached earlier in the same route by an undo-free route of length <= the current action count, so a shortest win never needs undo. Source: snapshots are pushed by `hlsimbcfgc()` before every click and popped by `vczehveskr()` (ACTION7 and the collision undo); the restore copies only piece lists. Check: `python undo_check.py 31 1500 24 0.35` and `32 6000 40 0.3` -> proofs/certificates/su15_L3/p1/logs/selfcheck/undo_check_s31.log, _s32.log: `UNDO_CHECK seed 32 walks 6000 actions 227736 undos 68603 actions_at_depth>=10 173736 max_stack 35 undo_to_nonvisited 0 undo_model_mismatch 0 invariant_dmin<=t_violations 0 sim_mismatch 0`. The lockstep walks above compare the full engine undo stack with the model stack every step (0 mismatches).
- Step budget / penalty: they can only end the game (lose); they cannot create a win, so they cannot shorten anything.

## P3 RULES (each one sentence, its check, its log)
| rule | check (exhaustive unless marked) | log, verbatim |
|---|---|---|
| R_CLASS-LA: one piece of any value 0..3 at any valid position, any click: in range and row 10..62 -> it lands exactly at landc(v,P), never lossy; otherwise noop | `chk_lemmas LA` all 4 values x all positions x 4096 clicks | proofs/certificates/su15_L3/p1/logs/nodeA/lemma_LA.log `LA clicks=53780480 in_range=2907572 noop=50872908 violations=0` |
| R_CLASS-LB: two hit pieces: equal values always merge into one v+1 at the predicted place; different values always fail, except {v3,v2} mid-flight wins | `chk_lemmas LB v1 v2` all 16 value pairs, all position pairs in range, all clicks | proofs/certificates/su15_L3/p1/logs/nodeA/lemma_LB_*.log e.g. `LB v1=2 v2=3 pairs=205511450 merge_ok=0 fail=192544850 midflight_win=12966600 violations=0`; `LB v1=1 v2=1 pairs=72343402 merge_ok=72343402 fail=0` |
| R_CLASS-LC: landed rectangles of any three values at any click pairwise overlap, so 3+ hit pieces = fail (mixed) or lossy (all equal) | `chk_lemmas LC` | proofs/certificates/su15_L3/p1/logs/nodeA/lemma_LC.log `LC pair-overlap checks=651264 violations=0` |
| R_CLASS (whole states): the classification holds on whole states (non-hit pieces untouched) | `chk_states bfs2` (ALL 455,385 states within 2 clicks) + `rand 300000` + `mass12 300000`, x 4096 clicks | proofs/certificates/su15_L3/p1/logs/nodeA/states_bfs2.log `CHK_STATES set=bfs2 states=455385 clicks=1865256960 ... class_violations=0 mass_violations=0`; states_rand.log, states_mass12.log `class_violations=0 mass_violations=0` (sampled sets) |
| R_MASS: clean clicks keep mass, lossy clicks lower it, so a state with mass != 12 or a piece > v3 cannot win | chk_states mass counters (above) + won() needs v3+v2 (=12) | same logs, `mass_violations=0`; search log `pruned_R_MASS=0` (never fired) |
| R_FINAL-FP (joint click): state {v3 at a, v2 at b}, both hit: win <=> some tick 1..3 has both centred in zones; otherwise fail | `chk_final FP` every click, every a, every b in range | proofs/certificates/su15_L3/p1/logs/nodeA/final_FP.log `FP checks=205511450 wins=12966600 violations=0` |
| R_FINAL-FS (one mover): {mover, static}, only mover hit: win <=> static in zone and mover in zone at a tick 1..4 | `chk_final2 FS 3` and `FS 2`: every click, every mover position, EVERY static position (no sampling) | proofs/certificates/su15_L3/p1/logs/nodeA/final2_FS3.log `FS_EXHAUSTIVE mover=3 static=2 checks=2548506726 wins=4748863 violations=0`; final2_FS2.log `FS_EXHAUSTIVE mover=2 static=3 checks=2190493642 wins=3815190 violations=0` |
| R_H: h(s) = relaxed lineage DP bound <= true clicks to win (relaxation: lineages independent, real geometry; every real useful click = one relaxed move, final click per F0/F1/F2) | proof by relaxation (lines 5-8 above) using the exhaustive lemmas; DP window check in dp_init (exit 3 on violation; in-range offsets lie in [-11,8]); independent numpy re-implementation | proofs/certificates/su15_L3/p1/logs/dp_indep.log `DP_INDEP tables 150 c_tables 150 entries_compared 614400 mismatches 0` / `H_INDEP F0(end-state) 12 F1(pair mid-flight) 11 F2(single mid-flight) 12 h=min 11` |
| R_H cross-checks vs true distance | DP vs exact BFS on sub-instances; h along real 11-routes; h(s) <= 1+h(s') on walks; h vs exhaustive-BFS true distance on random near-win states | proofs/certificates/su15_L3/p1/logs/subinst_cap2_sz4.log, proofs/certificates/su15_L3/p1/logs/subinst_cap3_sz3.log `dp>bfs(UNSOUND)=0` on every line; proofs/certificates/su15_L3/p1/logs/route_admissibility.log `ROUTE_ADMISSIBILITY flags=3 violations=0` (both routes); proofs/certificates/su15_L3/p1/logs/nodeA/cons3_seed*.log `violations=0`; proofs/certificates/su15_L3/p1/logs/nodeA/tdist_seed*.log (see self-attack) |
| R_TT: a state already refuted with >= the same remaining budget is not searched again (sound because the state is Markov by R_UNDO/R_CLASS) | by construction | exhaustion log `pruned_R_TT=0` (never fired) |
| dpcap: table entries above the remaining budget are dropped, so h returns min(true DP value, budget+1): never above the DP value | by construction (all costs are sums of non-negative terms) | n/a |

## P4 EXHAUSTION (bound 10, from the level-3 start, root prune disabled so the root is really expanded)
Command (machine A): `./ida 10 3 exh 0 <shard> <mod> noroot` ; first run 8 shards: proofs/certificates/su15_L3/p1/logs/nodeA/exh10_noroot_shard0..7.log ; rerun with a fresh compile and recorded hashes, 5 shards: proofs/certificates/su15_L3/p1/logs/nodeA/exh10_rerun_shard0..4.log (hashes_exh5.txt).
| depth | expanded | clicks | noop | fail | lossy | wins | distinct children | h-evaluations | pruned R_H | pruned R_MASS | pruned R_TT |
|---|---|---|---|---|---|---|---|---|---|---|---|
| 0 | 1 | 4096 | 2845 | 245 | 0 | 0 | 992 | 992 (sum over shards) | 992 | 0 | 0 |
| 1..10 | 0 | - | - | - | - | - | - | - | - | - | - |
Rerun (fresh compile ida_exh5 sha256 1b3605fb..., sources hashed in proofs/certificates/su15_L3/p1/logs/nodeA/hashes_exh5.txt), 5 shards: `DEPTH 0 expanded=1 clicks=4096 noop=2845 fail=245 lossy=0 distinct_children=992 hcalls=199 pruned_R_MASS=0 pruned_R_H=199 pruned_R_TT=0 win_clicks=0` / `NO_WIN within B=10 (shard 0/5) exhausted=1` (hcalls 199+199+198+198+198 = 992, all pruned by R_H, all 5 shards rc=0).
First run, verbatim (shard 0): `DEPTH 0 expanded=1 clicks=4096 noop=2845 fail=245 lossy=0 distinct_children=992 hcalls=124 pruned_R_MASS=0 pruned_R_H=124 pruned_R_TT=0 win_clicks=0` and `NO_WIN within B=10 (shard 0/8) exhausted=1` (all 8 shards; hcalls sum 992, pruned_R_H sum 992).
Consistency with the measured counts: 992 distinct children at depth 1 (chk_states bfs2: `DEPTH1 distinct_states=993` = 992 + the start itself, `DEPTH2 distinct_states=454391` all-time = 993 + 453,398).
Note: the exhaustion is shallow because h is strong; all the weight of the proof is on R_H, which is why R_H is backed by the relaxation argument plus exhaustive lemma checks and four independent cross-checks. A deeper exhaustion (h only from depth 2) was not run: h costs about 2.5 s per state (logs: htime2 `cap=8 h=9 2.51s`), i.e. ~13 CPU-days for the 453,398 depth-2 states.

## Controls
- Positive: `./ida 11 3 find <seed> 0 1` (8 seeds) -> proofs/certificates/su15_L3/p1/logs/nodeA/find_real_B11_seed6.log `WIN_FOUND len=11 route: 57,29 48,29 5,23 40,33 28,21 26,27 33,39 13,23 19,30 23,38 27,46` (a new 11-route, different from the two known ones).
- Control: with the mid-flight win test switched off, the search does not find the 11-action win.

## P5 REPLAY on the real engine
Replay on the real engine (logs: proofs/certificates/su15_L3/replay_cert_route.log and proofs/certificates/su15_L3/p2/logs/replay_11.log):
`su15 L 3 route len 11 result(actions_to_complete,state) (11, 'NOT_FINISHED')` and `negative control (drop last action): (0, 'NOT_FINISHED')` (positive-control route wins at click 11, not before);
No route of 10 or fewer exists to replay.

## Self-attack (the three adversarial lenses, run by p1 on its own proof)
### Lens r-engine (assume the simulator is wrong where it matters)
| target | command | log | verbatim |
|---|---|---|---|
| wins at tick 1, 2, 3 and 4 on the real engine, {v3,v2}/{v3,v1,v1}/{v2,v2,v2} near zones, all clicks rows 10..62 | `python lock_ticks.py 60 51` | proofs/certificates/su15_L3/p1/logs/selfcheck/lock_ticks_s51.log | `LOCK_TICKS states 60 clicks 203520 mismatch 0 engine_wins_by_first_winning_tick {1: 455, 2: 295, 3: 9, 4: 6}` |
| pieces clamped on all four borders, mixed values, all 4096 clicks | `python lock_targeted.py 41 15 border` / `42 40 border` | proofs/certificates/su15_L3/p1/logs/selfcheck/lock_targeted_border*.log | `LOCK_TARGETED kind border seed 42 states 40 clicks 163840 mismatch 0 ... piece_click_pairs_at_distance_exactly_8 1039` |
| merge groups of 3..6 equal pieces (+ a stray piece) | `python lock_targeted.py 41 15 cluster` / `42 40 cluster` | proofs/certificates/su15_L3/p1/logs/selfcheck/lock_targeted_cluster*.log | `LOCK_TARGETED kind cluster seed 42 states 40 clicks 163840 mismatch 0 {... 'fail': 5507, 'win': 1} lossy_clicks 2928` |
| pieces at distance exactly 8 near zones | `python lock_targeted.py 41 15 d8` / `42 40 d8` | proofs/certificates/su15_L3/p1/logs/selfcheck/lock_targeted_d8*.log | `LOCK_TARGETED kind d8 seed 42 states 40 clicks 163840 mismatch 0 {... 'win': 448} ... piece_click_pairs_at_distance_exactly_8 1065` |
| undo at every depth (stack up to 35), penalties/collisions mixed in | `python undo_check.py 32 6000 40 0.3` | proofs/certificates/su15_L3/p1/logs/selfcheck/undo_check_s32.log | `undo_to_nonvisited 0 undo_model_mismatch 0 invariant_dmin<=t_violations 0 sim_mismatch 0` |
| level facts read from the engine | `python engine_facts.py` | proofs/certificates/su15_L3/p1/logs/engine_facts.log | `zones [(5, 46, 9, 9), (19, 46, 9, 9)]`, `goal [[3, 1], [2, 1]]`, `enemies 0`, `steps_budget 48 range 8 ticks 4 speed 4` |
Step budget 48 and the collision penalty can only end the game (lose); a win in <= 10 actions is far inside the budget. Result: nothing broke.

### Lens r-lemma (try to make h larger than the true distance)
- Relaxation membership (every real move is a legal relaxed move) is exactly LA + LB + LC + FP + FS, all exhaustive over every position/click (P3 table); FS is exhaustive (chk_final2, 4.74e9 clicks, 0 violations).
- True distance by exhaustive breadth-first search (all 4096 clicks per state, exact simulator, depth <= 3) on random near-win mass-12 states ({3,2},{3,1,1},{2,2,2},{3,1,0,0},{2,2,1,1},{3,0,0,0,0},{2,2,1,0,0}) vs h: `./tdist <seed> 400 150000` -> proofs/certificates/su15_L3/p1/logs/nodeA/tdist_seed1..4.log. Seeds 1-4 (20 min each on machine A): 1585 states; true distance 1: 447 states (h tight on 447), 2: 419 (tight 378), 3: 250 (tight 194); no win within 3: 396; depth-3 layer too large to finish: 73; **h > true distance: 0**. Verbatim: tdist_seed1.log: `TDIST_RUNNING after_state=385 seed=1 states=400 true_d1=107(tight 107) true_d2=103(tight 95) true_d3=54(tight 39) no_win_le3=106 unknown_d3=15 violations=0` ; tdist_seed2.log: `TDIST seed=2 states=400 true_d1=103(tight 103) true_d2=103(tight 94) true_d3=68(tight 55) no_win_le3=99 unknown_d3=27 violations=0` ; tdist_seed3.log: `TDIST seed=3 states=400 true_d1=111(tight 111) true_d2=111(tight 100) true_d3=63(tight 47) no_win_le3=99 unknown_d3=16 violations=0` ; tdist_seed4.log: `TDIST seed=4 states=400 true_d1=126(tight 126) true_d2=102(tight 89) true_d3=65(tight 53) no_win_le3=92 unknown_d3=15 violations=0`
- h along real winning routes: route_admissibility.log `ROUTE_ADMISSIBILITY flags=3 violations=0` (h = 11,10,...,1 exactly along both known 11-routes: h is tight there).
- One-step consistency h(s) <= 1 + h(s') on 1,800 random transitions: proofs/certificates/su15_L3/p1/logs/nodeA/cons3_seed1..8.log `violations=0`.
- DP vs exact BFS on sub-instances (subsets of the start pieces, cost caps 2 and 3): `dp>bfs(UNSOUND)=0` on every line.
Result: no state with h > true distance was found.

### Lens r-shorter (try to find a 10)
- Positive-control search at bound 11 (8 random seeds) found an 11, never a 10 (a win found at depth <= 10 would have been reported with its length).
- Weakened-pruning attack: the same search at bound 10 with h relaxed by one click (prune only if h-1 > clicks left: i.e. it explores every state an off-by-one h would have wrongly cut), randomised DFS: first run 4 seeds x 3.3 min, second run 5 seeds x 25 min (`./ida_slack 10 3 find <11..15> 0 1 noroot 1`, proofs/certificates/su15_L3/p1/logs/nodeA/slack1b_find_B10_seed11..15.log): every run ended by the 1500 s timeout with no `WIN_FOUND` line (the program prints `WIN_FOUND len=.. route: ..` and stops the moment it finds a win). This is a randomised attack, not an exhaustive one: the census below shows 255 depth-1 children with h = 10, so an exhaustive slack-1 search is far beyond the budget.
- Depth-1 census of h (full DP, no cap) over all 992 children of the start: `./census <k> 4` -> proofs/certificates/su15_L3/p1/logs/nodeA/census_shard0..3.log: `CENSUS shard 0/4 distinct_children=992 hist: h10=67 h11=176 h12=5` (+ shards 1-3: h10=64/62/62, h11=180/182/183, h12=4/4/3) = 255 children with h=10, 721 with h=11, 16 with h=12, none below 10: every child needs >= 10 more clicks, so >= 11 in total.
- Counts: 992 distinct states at depth 1 (search log) and 453,398 at depth 2 (chk_states bfs2: 454,391 all-time - 993) match the numbers recorded earlier.
- Controls present: bound-11 positive control (WIN len 11, engine-replayed) and the mid-flight-off control - see Controls.
Result: no 10-click win found.


## Independent check
An independent refuter checked the final value on 2026-11-19 and found no win in 10 clicks or fewer. The check note is withheld (digest 5c5eb614be4d5479b10c50d3616522d3e38f9068f7b21660fed744bea510b104; row in proofs/certificates/levels/WITHHELD.md).

## Scope and limits
- P1 is a sampled lockstep, not a formal equivalence; the rule checks are exhaustive against simc.h, which inherits P1.
- R_CLASS on whole states with 3+ pieces uses: LC (exhaustive) + the engine source (union-find; first overlapping pair of different values = collision) + whole-state checks on all 455,385 states within 2 clicks and 600,000 sampled states; not every 9-piece state is enumerated (impossible), but the per-click behaviour depends only on the hit pieces, which LA/LB/LC cover exhaustively.
- The exhaustion is depth 1 deep; see note in P4.
