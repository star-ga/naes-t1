# wa30 level 9: floor 57, status empirical

Game wa30-ee6fef47, level 9. Game file (the official wa30 game file, not part of this pack), sha256 0c8b63caee2d092d6c0750fd74e417de682d09973b549046394dadf0437b1477. The floor is 57 actions, counted from the level start, with no RESET in front. The proven lower bound is 50. All 183 level floors are proven; 2 of them (wa30 levels 8 and 9) by empirical proof. The class is Empirical.

## Upper bound: a winning route of 57 actions
Route (one action digit each): `4 4 4 4 4 5 1 1 5 3 3 1 5 1 1 5 2 2 2 4 5 1 4 5 3 2 3 2 3 3 1 3 3 3 3 3 3 5 4 4 5 2 3 5 3 3 5 4 4 4 4 4 4 1 1 1 5`.
`replay_route_57.log` replays it on the game file named above, after the certified routes of levels 1 to 8. The level completes at action 57 (`lc_before=8`, `completed_at_action=57`, `lc_after=9`, state WIN). The control, the same route without its last action, does not complete the level (`completed_at_action=None`, `lc_after=8`).
The replay is a check of the route. The floor is proven by empirical proof.

## Lower bound: no win in 49 actions or fewer
`lower_bound_run.log` is the recorded iterative-deepening A* run with an admissible heuristic (start value 30). Bounds 30 to 49 each end with `COMPLETE_NOWIN` or `CLOSED_EMPTY`; the last closed bound is `BOUND 49 COMPLETE_NOWIN lb=50`. The proven lower bound is 50.
The per-edge consistency check of the heuristic ran as a live counting observer: 0 violations on every bound.
The log in this folder has its machine fields replaced by placeholders. The unmodified log has sha256 b5f73e65e0e0cd4097e617a547c1c2e10436982f98c2be1e95a90b587a271caf (3,293 bytes); this copy has sha256 2da1be59b00322567ec8a4d82a6c73da1b28f4fc50fa915d16b1065ad81d4faa.

## Scope and limits
- The lower-bound run used an earlier build of the engine. The digest of that build is not recorded, and no theorem ties its transitions to the game file above. The run is a bound for that earlier build. The floor is proven by empirical proof.
