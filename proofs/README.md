# Floor proofs

The floor of a level is the minimum action count on the official game file, counted from the level start. The count excludes a leading RESET. NAES-T1 plays each of the 183 levels in this many actions. The floors add to 5,499. The table is [`levels.csv`](levels.csv), taken from [`data/demo25_levels_mmae.csv`](../data/demo25_levels_mmae.csv).

Every path in these documents and in the level files is written from the repository root, except a link target inside a markdown link, which is relative to its file. A bare file name with no slash names a file in the folder that the document describes.

## Public record of a level

The public record of a level is its statement file in `proofs/certificates/lower_bounds/files/` (lower bound, method, proof name and Checked line) and its level file in `proofs/certificates/levels/`, which holds the digest of every artifact. Program sources and working records are withheld by the rule of this pack and are identified by digest. Counted from the level files against this tree: 118 levels cite one or more included record files in `release_evidence/lower_bound_artifacts/files/`; 17 levels cite included record files only under `proofs/certificates/`; 48 levels have their statement, their level file with the digests, and one or more source extracts in `proofs/certificates/lower_bounds/extracts/`, and no other included record file. Every level has its statement. This is a rule about the record, not a judgement on any argument.

## Proof classes

- Exhaustive search: a complete search with no heuristic estimate (breadth-first, uniform-cost or layered) over the game states, on the engine or on a reduced model whose merged states a stated equivalence or dominance lemma justifies, closes every cost below the floor.
- Exact math: a closed form, a dynamic program, a complete enumeration over the solution descriptions of the full game (end configurations, placements, programs, merge trees, rotation assignments) with every case priced exactly, or a constraint system over end configurations shown to have no solution gives the lower bound; no game rule is dropped.
- Matching bounds: a lower bound at the start state from a relaxation (a model with a game rule dropped), a coverage count or an admissible heuristic already equals the length of a replayed winning route.
- A*: a complete best-first search of the A* family (A*, iterative-deepening A* or proof-number search) with an admissible heuristic; closed per algorithm: for A*, every state whose estimated total cost is below the floor was expanded; for iterative-deepening A*, every cost bound below the floor was searched to exhaustion; for proof-number search, the search proved that no win exists below the floor; no search finds a win below the floor.
- Bounded SAT: the game transition is unrolled step by step to one action below the floor, and every such formula is unsatisfiable on the current instance. Checker certificates in this repository: all 100 g50t level 7 queries at bound 40 (85 checked by the verified LRAT checker cake_lpr, 15 by drat-trim).
- Empirical proof (winning route replayed on the engine; mathematical lower bound below it) — Empirical proof: a winning route replayed on the game engine, with a proven lower bound published beside it. All 183 level floors are proven; 2 of them (wa30 levels 8 and 9) by empirical proof.

Every proof's action model covers every engine-effective action, including every display-pixel click.

The class counts are: exhaustive search 92, bounded SAT 7, exact math 37, matching bounds 24, A* 21, Empirical proof (winning route replayed on the engine; mathematical lower bound below it) 2.

## Empirical floors

Two floors are empirical routes with a proven lower bound that stops short of the route: wa30 L8 route 91, proven lower bound 57; wa30 L9 route 57, proven lower bound 50. Both routes win on the official game file. The lower bounds are closed exhaustive bounds. All 183 level floors are proven; 2 of them (wa30 levels 8 and 9) by empirical proof.

## What a certificate file contains

`proofs/certificates/levels/<game>_L<level>.json` is one level.

- `floor` matches the floor column. On the 181 levels of the other five classes it equals `lower_bound.value`. On wa30 levels 8 and 9 (empirical proof) it is the length of the replayed winning route, and `lower_bound.value` is the proven lower bound below it.
- `naes_t1_actions` is the NAES-T1 action count.
- `proof_class` and `proof_detail` name the class and the specific argument.
- `lower_bound.statement` is the bound in words. `lower_bound.kind` is the argument name.
- `upper_bound.route` is the winning action list. A token is an action id. A token `6@x,y` is a click on display pixel `(x, y)`. `upper_bound.length` is the number of tokens. `opening_reset_counted` is false.
- `artifact_sha256` lists supporting records by kind and sha256. Large proof files stay outside this repository and are named only by those hashes.
- `source_certificate_sha256` is the digest of the record the certificate is drawn from. On tn36 levels 1-7 that digest is a log under [`proofs/certificates/floor_cert/tn36/logs/`](certificates/floor_cert/tn36/logs/). On wa30 level 9 it is the record [`proofs/certificates/wa30_L9/RECORD.md`](certificates/wa30_L9/RECORD.md). On every other level the source file is not in this repository. Command lines, host names, and parallelism settings are not in the published file. See [`proofs/certificates/NOTE.md`](certificates/NOTE.md).

## Re-check

From the repository root, run the check in each directory that contains a `SHA256SUMS` file, for example `cd proofs/certificates/levels && sha256sum -c SHA256SUMS`.

Route replay: send the tokens of `upper_bound.route` in order to the official game file of that level (the versioned game id is `game_id` of the certificate; toolkit `arc-agi` 0.9.9 with `arcengine` 0.9.3, seed 0), from a fresh public RESET of the game, after the routes of the earlier levels. A pass completes the level after exactly `upper_bound.length` actions and not before. STARGA replayed all 183 routes in this way with the replay program of its certificate pack (SHA-256 `cfa99edfeb23b4f5081f53efc7ff8698b591961800d2af0dae9616faf2880377`, game manifest SHA-256 `4f871cda56d52598c354a12bd7595831fb3dcd0b20f070cd108ce80f2f487e31`): 183 of 183 `"verdict": "PASS"`. That program is a Python program and is not part of this repository; its command for one level is `python3 <replay program> --games-dir <directory of the official game files> --game <game> --level <level> --output <file>`. The method-by-method account is [`METHODS.md`](METHODS.md).
