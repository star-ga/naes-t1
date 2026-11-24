# Proof methods

This note is the method account for the 183 floors. The floor of a level is the minimum action count on the official game file, counted from the level start. The count excludes a leading RESET. Every proof's action model covers every engine-effective action, including every display-pixel click. A click token in a certificate is one display pixel. Where several pixels share one effect, the search still accounts for every pixel, either by playing it or by a partition check that places it with a representative of the same effect.

Every certificate sets `opening_reset_counted` to false. The route token count is the action count from the level start, and that count does not add a RESET in front of the route. The details `complete_state_ucs_full_dispatch_reset_inclusive` (tu93 L2, tu93 L7) and `complete_state_ucs_licensed_alphabet_reset_inclusive` (re86 L1) put RESET in the search alphabet. RESET is an engine action, and the search expands it. A RESET token inside the route counts as one action. Certificates whose statement calls the search RESET-inclusive use the same rule: re86 L1, vc33 L1, vc33 L2, vc33 L3, vc33 L4, vc33 L5, vc33 L6, vc33 L7.

The proof class of each level is the `proof_class` column of [`levels.csv`](levels.csv). On the 181 levels whose class is not Empirical proof, `lower_bound.value` equals the floor. On wa30 levels 8 and 9 (Empirical proof) the floor is the length of the replayed winning route, and `lower_bound.value` is the proven lower bound below it. The class counts are: exhaustive search 92, bounded SAT 7, exact math 37, matching bounds 24, A* 21, Empirical proof (winning route replayed on the engine; mathematical lower bound below it) 2.

Each level has exactly one class, from what its certificate did. The tests run in this order and the first YES decides: (1) a winning route replayed on the game engine, with a proven lower bound published beside it: Empirical proof; (2) the game transition is unrolled step by step to one action below the floor, and every such formula is unsatisfiable on the current instance: Bounded SAT; (3) a complete best-first search of the A* family (A*, iterative-deepening A* or proof-number search) with an admissible heuristic; closed per algorithm: for A*, every state whose estimated total cost is below the floor was expanded; for iterative-deepening A*, every cost bound below the floor was searched to exhaustion; for proof-number search, the search proved that no win exists below the floor; no search finds a win below the floor: A*; (4) a complete search with no heuristic estimate (breadth-first, uniform-cost or layered) over the game states, on the engine or on a reduced model whose merged states a stated equivalence or dominance lemma justifies, closes every cost below the floor: Exhaustive search; (5) a lower bound at the start state from a relaxation (a model with a game rule dropped), a coverage count or an admissible heuristic already equals the length of a replayed winning route: Matching bounds; (6) a closed form, a dynamic program, a complete enumeration over the solution descriptions of the full game (end configurations, placements, programs, merge trees, rotation assignments) with every case priced exactly, or a constraint system over end configurations shown to have no solution gives the lower bound; no game rule is dropped: Exact math. When a certificate combines arguments, the class is the argument that closes the last gap.

A level has one class: the class of the argument that closes the last gap below the floor. When two arguments fit, the first of Empirical, Bounded SAT, A*, Exhaustive search, Matching bounds, Exact math decides. A step that applies one action, or one lossless macro charged its exact action count, to a game state makes a search. A rule is dropped when the bounding model is a relaxation, or when parts that interact are priced apart and joined by an inequality. A coverage count is one value at the start state that adds, over the items the win still needs, the distance each needs when moved alone; an enumeration of every end configuration that prices each one exactly drops no rule. Counting the actions that the rules charge every route drops no rule, also when the count is written as a start value.

## Exhaustive search

Exhaustive search: a complete search with no heuristic estimate (breadth-first, uniform-cost or layered) over the game states, on the engine or on a reduced model whose merged states a stated equivalence or dominance lemma justifies, closes every cost below the floor.

The certificate checks three numbers: the route length, `lower_bound.value`, and `floor` all match, and the statement records the closed layers or the state count. Re-check by counting the route tokens, confirming those three numbers, running `cd proofs/certificates/levels && sha256sum -c SHA256SUMS` from the repository root, and replaying the route on the official game file.

Levels (92), from [`levels.csv`](levels.csv):

bp35 L1, cd82 L1, cd82 L2, cd82 L3, cd82 L4, cd82 L5, cd82 L6, dc22 L1, dc22 L2, dc22 L3, dc22 L4
dc22 L5, dc22 L6, ft09 L1, ft09 L2, ka59 L1, ka59 L2, ka59 L3, ka59 L4, ka59 L5, ka59 L6, ka59 L7
lp85 L1, lp85 L2, lp85 L3, lp85 L4, lp85 L5, lp85 L6, lp85 L7, lp85 L8, ls20 L1, ls20 L2, ls20 L3
ls20 L4, ls20 L5, ls20 L6, ls20 L7, m0r0 L1, m0r0 L2, m0r0 L4, m0r0 L5, m0r0 L6, re86 L1, s5i5 L1
s5i5 L2, s5i5 L3, s5i5 L4, s5i5 L5, s5i5 L6, s5i5 L7, s5i5 L8, sc25 L1, sc25 L2, sc25 L3, sc25 L4
sc25 L5, sc25 L6, sk48 L1, sk48 L2, sk48 L3, sk48 L4, sk48 L5, sk48 L6, sk48 L7, sk48 L8, su15 L1
su15 L4, su15 L5, su15 L6, su15 L7, su15 L8, su15 L9, tn36 L1, tn36 L6, tn36 L7, tr87 L1, tu93 L1
tu93 L2, tu93 L3, tu93 L4, tu93 L5, tu93 L6, tu93 L7, tu93 L8, tu93 L9, vc33 L1, vc33 L2, vc33 L3
vc33 L4, vc33 L5, vc33 L6, vc33 L7

Proof details in this class:

- `bidirectional_complete_state_search_on_lockstep_checked_model`: ka59 L2
- `click_bfs_plus_undo_dominance`: su15 L1
- `complete_alphabet_complete_state_ucs`: m0r0 L6
- `complete_alphabet_uniform_cost_exhaustion`: vc33 L3, vc33 L4, vc33 L6, vc33 L7
- `complete_finite_state_layers_native_coupling_L1_action_dominance`: sk48 L1
- `complete_layout_bfs_full_click_alphabet`: su15 L6, su15 L7, su15 L8
- `complete_prefix_tree_licensed_alphabet`: cd82 L1, cd82 L2
- `complete_state_bfs_on_sampled_equivalent_simulator`: dc22 L6
- `complete_state_macro_uniform_cost_search_all_facings`: sc25 L5, sc25 L6
- `complete_state_macro_uniform_cost_search_reachable_start`: sc25 L3
- `complete_state_ucs_exhausted_injective_key`: ka59 L3, ka59 L4, ka59 L6
- `complete_state_ucs_fuel_dominance`: ls20 L2, ls20 L4, ls20 L5, ls20 L6, ls20 L7
- `complete_state_ucs_full_dispatch_reset_inclusive`: tu93 L2, tu93 L7
- `complete_state_ucs_licensed_alphabet`: su15 L9
- `complete_state_ucs_licensed_alphabet_reset_inclusive`: re86 L1
- `complete_state_ucs_macro_reduction`: sc25 L2
- `complete_state_ucs_native_action_quotient`: ka59 L1, ka59 L5
- `complete_state_ucs_reverified`: sc25 L4, sk48 L5, sk48 L8, su15 L4, su15 L5
- `complete_state_ucs_simulator_bisimulation_gated`: dc22 L5
- `complete_state_ucs_structural_alphabet`: dc22 L1, dc22 L2, dc22 L3, dc22 L4
- `complete_state_ucs_recovered`: m0r0 L1, m0r0 L2, m0r0 L4, m0r0 L5
- `complete_state_uniform_cost_bfs`: sk48 L2, sk48 L3
- `complete_state_uniform_cost_layer_exhaustion (C reimplementation of the search model of sk48, colour-class symmetry quotient, 64-bit packed key)`: sk48 L6, sk48 L7
- `complete_supergraph_BFS_through_64_plus_fresh_exact_engine_replay_at_65`: ka59 L7
- `compound_rotation_state_graph_exhaustion_button_alphabet`: lp85 L6
- `engine_bfs_center_tuple`: ft09 L1, ft09 L2
- `engine_bfs_completion_loop`: s5i5 L3, s5i5 L4, s5i5 L6, s5i5 L7, s5i5 L8
- `engine_bfs_no_shorter`: tn36 L1
- `engine_bfs_whole_object_key`: s5i5 L1, s5i5 L2, s5i5 L5, vc33 L1, vc33 L2
- `fixed_instance_bfs_depth_22_win_at_23`: sk48 L4
- `graph_exhausted+lemma`: tr87 L1, tu93 L6
- `macro_dijkstra_complete_state_key`: sc25 L1
- `move_ordering_uniform_cost+lemma`: bp35 L1, lp85 L1, lp85 L2, lp85 L4, lp85 L7, lp85 L8, ls20 L3, tu93 L3, tu93 L5, tu93 L8, tu93 L9
- `octant_symmetry_uniform_cost_exhaustion`: cd82 L3, cd82 L4, cd82 L5, cd82 L6
- `rotation_group_state_graph_exhaustion_button_alphabet`: lp85 L3, lp85 L5
- `run_chain_search_max_pop_bound`: tn36 L6, tn36 L7
- `uniform_cost_bfs_complete_state_full_alphabet`: vc33 L5
- `zero_heuristic_astar_click_equiv+lemma`: ls20 L1, tu93 L1, tu93 L4

`engine_bfs_no_shorter` (tn36 L1) is breadth-first search on the official game file. The only action is the engine click, and the click is a display pixel. The search expands 6449 nodes, runs to completion, and finds no win shorter than 7. The opcode dynamic program in the exact-math section meets the same floor.

`run_chain_search_max_pop_bound` (tn36 L6, tn36 L7) enumerates program-run chains. No single program wins. A click count is the toggles plus the number of runs. The maximum edge popcount plus the run count is a lower bound, and deleting a cycle cannot reduce it, so simple paths bound every path. Only run counts 2 through 6 on level 6, and 2 through 7 on level 7, can cost under 16. Each of those counts finishes with capped 0 and no shorter chain. Every larger count costs at least 16. The certificate route has length 16 and wins on the official game file. The account of the homes, the color writers, and the node counts is in [`proofs/certificates/floor_cert/tn36/SUMMARY.md`](certificates/floor_cert/tn36/SUMMARY.md).

## Exact math

Exact math: a closed form, a dynamic program, a complete enumeration over the solution descriptions of the full game (end configurations, placements, programs, merge trees, rotation assignments) with every case priced exactly, or a constraint system over end configurations shown to have no solution gives the lower bound; no game rule is dropped.

The certificate checks that `lower_bound.value` equals the route length and the floor, and that the statement names the enumeration size, the algebraic cost, or the proof that establishes the value. Re-check by counting the route tokens and reading that statement. Replay the route on the official game file.

`qf_lia_smt_exact_enumeration` (ar25 L4, ar25 L5) is exact math. The formula is quantifier-free linear integer arithmetic. It encodes the exact integer cost of the legal endpoints. Unsatisfiability below the floor is that exact cost. It is not a propositional unrolling of engine steps, so the detail stays in this section rather than under bounded SAT.

`opcode_dp_one_play_plus_min_hamming` (tn36 L2, tn36 L3, tn36 L4, tn36 L5) is an exact-math argument. Those levels have no collectible and no portal, and a failed run restores the home, so the program words are the only memory. Any chain of programs costs at least the hamming distance from the initial words to the last program, plus one play. The dynamic program finds one program of that length equal to the floor, with 0 mismatches on the opcode sample. tn36 L1 meets the same program cost. Its row class stays exhaustive search because the engine breadth-first search also closes that floor. The row class of tn36 L2, tn36 L3, tn36 L4 and tn36 L5 is exact math.

Levels (37), from [`levels.csv`](levels.csv):

ar25 L1, ar25 L2, ar25 L3, ar25 L4, ar25 L5, ar25 L6, ar25 L7, ar25 L8, ft09 L3, ft09 L4, ft09 L5
ft09 L6, lf52 L1, sb26 L1, sb26 L2, sb26 L3, sb26 L4, sb26 L5, sb26 L6, sb26 L7, sb26 L8, sp80 L1
sp80 L2, sp80 L3, sp80 L4, sp80 L5, sp80 L6, su15 L2, tn36 L2, tn36 L3, tn36 L4, tn36 L5, tr87 L2
tr87 L3, tr87 L4, tr87 L5, tr87 L6

Proof details in this class:

- `alter_offset_complete_enumeration_disjoint_cost_partition`: tr87 L6
- `alter_offset_exhaust_enum_min_rot_plus_walk`: tr87 L5
- `closed_form_exhaustive_endpoint`: ar25 L1, ar25 L2
- `engine_measured_columns_meet_in_middle_min_weight`: ft09 L3, ft09 L4, ft09 L5, ft09 L6
- `engine_spill_of_every_placement`: sp80 L1
- `exact_cost_algebra_exhaustive_endpoint_enumeration`: ar25 L3, ar25 L6, ar25 L7, ar25 L8
- `one_merge_group_lemma_value_count_merge_tree_enumeration`: su15 L2
- `opcode_dp_one_play_plus_min_hamming`: tn36 L2, tn36 L3, tn36 L4, tn36 L5
- `placement_enumeration_transcribed_spill_rule`: sp80 L2, sp80 L3, sp80 L4, sp80 L5, sp80 L6
- `qf_lia_smt_exact_enumeration`: ar25 L4, ar25 L5
- `required_merge_pair_count_closed_form`: lf52 L1
- `rotation_group_closed_form_nonalter`: tr87 L2, tr87 L3, tr87 L4
- `walk-topology exhaustion + exact placement counting`: sb26 L1, sb26 L2, sb26 L3, sb26 L4, sb26 L5, sb26 L6, sb26 L7, sb26 L8

## Matching bounds

Matching bounds: a lower bound at the start state from a relaxation (a model with a game rule dropped), a coverage count or an admissible heuristic already equals the length of a replayed winning route.

The certificate checks that `lower_bound.value` equals `upper_bound.length` and `floor`. Re-check those three fields and replay the route on the official game file.

Levels (24), from [`levels.csv`](levels.csv):

bp35 L2, bp35 L7, cn04 L1, cn04 L2, cn04 L3, cn04 L4, cn04 L5, cn04 L6, g50t L2, r11l L1, r11l L2
r11l L3, r11l L4, r11l L5, r11l L6, re86 L2, re86 L3, re86 L4, re86 L5, re86 L6, re86 L7, re86 L8
su15 L3, wa30 L3

Proof details in this class:

- `assembly_word_schedule_dp_exhaustive_lb`: r11l L5, r11l L6
- `empty_contour_lb_meets_verified_win`: bp35 L7
- `lineage_relaxation_lower_bound_plus_bound_10_exhaustion_machine_checked`: su15 L3
- `phase_decomposition_coverage_lb`: re86 L2, re86 L3, re86 L4, re86 L5, re86 L6, re86 L7, re86 L8
- `relaxed_object_distance_matching_configuration_enumeration`: cn04 L1, cn04 L2, cn04 L3, cn04 L4, cn04 L5, cn04 L6
- `relaxed_placement_enumeration_select_place_count`: r11l L1, r11l L2, r11l L3, r11l L4
- `relay_algebra_ablation_stable_lb_meets_verified_win+lemma`: g50t L2
- `relay_algebra_admissible_lb_meets_verified_win`: bp35 L2, wa30 L3

## A*

A*: a complete best-first search of the A* family (A*, iterative-deepening A* or proof-number search) with an admissible heuristic; closed per algorithm: for A*, every state whose estimated total cost is below the floor was expanded; for iterative-deepening A*, every cost bound below the floor was searched to exhaustion; for proof-number search, the search proved that no win exists below the floor; no search finds a win below the floor.

The certificate checks that the first win has length equal to the floor and that the route replays to a win. Re-check the route length against `floor` and replay the route on the official game file.

Levels (21), from [`levels.csv`](levels.csv):

bp35 L3, bp35 L4, bp35 L5, bp35 L6, g50t L1, lf52 L2, lf52 L3, lf52 L4, lf52 L5, lf52 L6, lf52 L7
lf52 L8, lf52 L9, lf52 L10, m0r0 L3, wa30 L1, wa30 L2, wa30 L4, wa30 L5, wa30 L6, wa30 L7

Proof details in this class:

- `A* over complete Markov state (token cells+types, pillars, push-blocks, held, arrows, board offset); reduced alphabet (A`: lf52 L2
- `Abstract A* over peg-solitaire Markov state (goal cells+types, pillars, push-blocks) read from the real game engine state; actions:`: lf52 L3, lf52 L4, lf52 L5, lf52 L6, lf52 L8
- `astar_complete_alphabet_bnb_gated_model`: bp35 L3, bp35 L5
- `astar_complete_compact_state_pair_pattern_merge_count`: lf52 L9
- `astar_consistent_h_pdb_plus_structural_lb`: lf52 L10
- `astar_consistent_heuristic`: m0r0 L3
- `astar_consistent_heuristic_first_goal_pop`: wa30 L2, wa30 L6, wa30 L7
- `dfpn_proven`: g50t L1
- `exact A* over the complete abstract state (typed goal cells + block cells + mobile pillar cells) with an over-approximated 3-token+block pattern-database heuristic; engine->abstract cost-model lemma`: lf52 L7
- `idastar_bound_exhaustion_pdb_h0_82`: wa30 L5
- `idastar_bounded_tt_complete_bound_exhaustion`: wa30 L4
- `native_model_astar_complete_api_alphabet+lemma`: bp35 L4, bp35 L6
- `sokoban_astar_hungarian`: wa30 L1

## Bounded SAT with DRAT

Bounded SAT: the game transition is unrolled step by step to one action below the floor, and every such formula is unsatisfiable on the current instance.

Checker certificates in this repository: all 100 g50t level 7 queries at bound 40 (85 checked by the verified LRAT checker cake_lpr, 15 by drat-trim). [`proofs/certificates/g50t-l7-drat/CERT_100.md`](certificates/g50t-l7-drat/CERT_100.md) lists the 100 queries with the digest of each proof, and [queries_100.tsv](certificates/g50t-l7-drat/queries_100.tsv) holds the same list as a table. The 15 drat-trim queries have no LRAT proof, and drat-trim is not a formally verified checker. DRAT proofs of twenty of the formulas are also recorded one by one: each row in [`proofs/certificates/g50t-l7-drat/CHECKED.md`](certificates/g50t-l7-drat/CHECKED.md) has formula sha256, uncompressed-proof sha256, gzip sha256, and verdict `s VERIFIED`. The gzip files are not stored here. The other SAT rows record solver UNSAT answers and do not include a DRAT proof in this section.

`qf_lia_smt_exact_enumeration` is not in this section. It is exact math, for the reason given above. The tn36 floors are not in this section. Their certificates record the opcode dynamic program (levels 2-5) and the run-chain search (levels 6 and 7), both named above.

Re-check a DRAT row by comparing the three digests in the table and, with the gzip bytes of that digest, by running drat-trim on the formula of the recorded formula digest. Re-check the level certificate the same way as an exhaustive-search row: route length, floor, and a win on the official game file.

Every level whose closing argument is such an UNSAT answer has this class: bp35 L8, L9 and g50t L3 to L7.

Levels (7), from [`levels.csv`](levels.csv):

bp35 L8, bp35 L9, g50t L3, g50t L4, g50t L5, g50t L6, g50t L7

Proof details:

- `sat_bmc_unsat_at_F_minus_1`: bp35 L8, bp35 L9
- `sat_bmc_unsat_at_F_minus_1_all_pass_counts+lemma`: g50t L5
- `sat_bmc_v4_unsat_at_F_minus_1_main_grid_94_of_94`: g50t L3
- `sat_bmc_v4_unsat_at_F_minus_1_main_grid_45_of_45+release_order_grid_15_of_15+index0_closing_grid_15_of_15`: g50t L4
- `sat_bmc_v4_unsat_at_F_minus_1_main_grid_58_of_58`: g50t L6
- `sat_bmc_v4_unsat_at_F_minus_1_main_grid_60_of_60+release_order_grid_20_of_20+index0_closing_grid_20_of_20`: g50t L7

## Empirical routes

Empirical proof: a winning route replayed on the game engine, with a proven lower bound published beside it.

wa30 L8 route 91, proven lower bound 57. wa30 L9 route 57, proven lower bound 50. The route is a verified win on the official game file, counted from the level start. The proven lower bound is a run of exhaustive iterative deepening that closes every bound up to that number and does not close the remaining gap. For level 8 the closed bounds are 23 through 56, so the sound lower bound is 57. For level 9 the closed bounds run through 49, so the sound lower bound is 50.

The certificate checks that `floor` and `upper_bound.length` are the route length, and that `lower_bound.value` is 57 on level 8 and 50 on level 9. Re-check by counting the route tokens, reading that statement, and replaying the route on the official game file. A replay checks the route. The floor is proven by empirical proof.

Levels (2), from [`levels.csv`](levels.csv):

wa30 L8, wa30 L9

Proof details in this class:

- `replay_verified_route_upper_bound; sound lower bound 50`: wa30 L9
- `replay_verified_route_upper_bound; sound lower bound 57`: wa30 L8
