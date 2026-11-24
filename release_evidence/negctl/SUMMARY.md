# Planted negative controls for three checkers

The paper already records one planted-false lemma, for a single wa30 enemy-carry claim. These three runs add one control for each of three checkers: the endpoint enumerator, the undo-lemma checker, and the admissible A* checker. Each run takes one certified level, plants one false lemma or one inadmissible heuristic, and gives it to the checker of the recorded argument that the plant targets. The final class of each level and the checker that gets the control are stated in its section. The unplanted check accepts. The planted check rejects. The first counterexample is the field named in the result line.

Command lines, exit codes, and output are in the sibling logs. Checkout paths, certificate paths, and the work directory are written as tokens. A game-load line that names the proof checkout is omitted from each stdout copy; the same line on stderr has that path replaced by `<proof-checkout>`.

## ar25 level 3 (endpoint-enumerator checker)

Final class: Exact math (algebra, closed form, enumeration). Recorded floor 40. The checker is the pinned endpoint-enumerator cost model (`enumerate_level`). This run uses a re-hosted copy of that cost model. It reached the level-3 start by replaying the certified level-1 and level-2 routes (`levels_completed_before_L3` = 2). The enumeration matches the certified row: 2,449,440 configs, 1 winner, minimum L1 cost 40, axis y = 9, shapes (11, 14) and (3, 14). In the result line, s0 and s1 are those two shape positions.

Planted lemma: one action may change both coordinates, so Chebyshev distance lower-bounds the move cost. Selection and axis terms stay on the certified L1 terms. The unique winner is the first counterexample: certified L1 cost 40, planted Chebyshev cost 28, with 1,166,400 configs seen through that axis.

The same result records a second plant on this row. Planted lemma: a one-cell reflection-axis move shifts every reflected image by at most one cell. ACTION 1 moves the horizontal axis from y 16 to y 15 (shift 1). The image of cell (4, 7) on shape `0027crpzfdutil` moves from y 25 to y 23 (shift 2). The image formula is the enumerator's `2*ay - py`.

Log: `negctl_ar25_L3.txt`. Exit code 0. `rejected` is true.

## su15 level 1 (undo-lemma checker)

Final class: Exhaustive search (uniform cost / breadth-first). Recorded floor 7, lemma-conditional. The control is given to the undo-lemma checker of the recorded undo argument, `action7_live_checks`, in the proof tooling (not included in this release). The witness is the certified path on the level-1 receipt. The true lemma passes in the same run: the witness wins at click 7, no prefix wins, and 6 of 6 nonterminal route states restore. This control runs that undo check. The recorded edge search stays as certified.

Planted lemma: ACTION7 leaves the semantic fruit/enemy board unchanged after a click. The first counterexample is route index 0, click (6, 3, 50). The fruit moves from (2, 3, 58) to (2, 2, 49). ACTION7 restores (2, 3, 58). The stack depth is 1 before the click and 1 after the undo.

The current game source spells attributes differently from the historical proof text. This run remaps attribute names through the receipt attribute map and runs the live checker. It leaves string constants unchanged. The source-text guard of the recorded proof looks for the old spellings, so this run does not call it.

Log: `negctl_su15_L1.txt`. Exit code 0. `rejected` is true.

## lf52 level 1 (admissible A* checker)

Final class: Exact math (algebra, closed form, enumeration). Recorded floor 8. The control is given to the admissible-A* checker, `prove()`, in the proof tooling (not included in this release). It records a consistency violation when `h(parent) - h(child)` exceeds the edge cost, and it then refuses status `PROVEN`.

The worked admissible example in the paper is wa30 level 2. That search states `h(s) <= 1 + h(s')` and trusts unit edge weight; it keeps no per-edge rejection record. lf52 level 1 has an admissible-A* checker that does refuse. This control uses lf52 level 1. The pinned checker file is not edited. The planted factor replaces `h_of` after import.

Unplanted heuristic `h = 2 * max(0, N_active - T)` returns status `PROVEN`, floor 8, 6 nodes, `consistency_violation` null, replay verified.

Planted heuristic `h = 4 * max(0, N_active - T)`. A jump costs 2 and deletes at most one peg, so the heuristic can drop by 4. The first counterexample is edge index 5, edge `[(6, 19, 20), (6, 31, 20)]`, parent h 16, child h 12, cost 2, drop 4, active pegs 5 to 4. The planted search returns status `SANDWICH`. The violation object stored by the search is the last overwrite (parent h 8, child h 4, cost 2). The first counterexample is the edge field above.

Log: `negctl_lf52_L1.txt`. Exit code 0. `rejected` is true.

## Coverage

Each of the three checkers has an executable check on the level that is run. No checker in this set is reported as unchecked. The existing wa30 carry control stays as published and is not rerun.
