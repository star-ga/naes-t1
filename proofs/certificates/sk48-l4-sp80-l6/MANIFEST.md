# Fixed-instance bounds for sk48 level 4 and sp80 level 6

## Files in this pack

| file | bytes | sha256 | what it proves |
| --- | ---: | --- | --- |
| sk48_L4_fixed_instance.json | 1880 | 749d755f417afc2865365dbd79d673599e7624a959fc100f8eef6900943f5d9b | sk48 level 4 fixed-instance search: first win length 23, engine replay wins at 23, every state through depth 22 expanded. |
| sk48_model_walks.json | 125 | 1b2519387f1104ac80c70e89a5b3bacd7efa6754cf03c5cea439f59ada021633 | Forty random trajectories of that model, 600 steps, matched the official engine with no disagreement. |
| sp80_motion_summary.json | 3971 | efa7c10c9e6b413115443117e57bef19fe0646df22c43bdd23cddacc575a9728 | Per-piece motion graphs from the official step. The recorded winning placement has action cost 29. The action cost is the sum of the four distances plus one click for each extra piece that must be selected; it is counted apart from the route length. Placements of cost at most 28 number 133,321,580. |
| sp80_scan_check.json | 294 | 4343286ecfc2395b2ddfedd76cd4d81912b342f8f7a28803dcb654af98c37fa2 | Spill transcription matches the live winning spill tick for tick, and matches the engine on the start, the known win, and 80 further placements. |
| sp80_scan.json | 348 | 20c1bf448dc4d52b613d0c5badc0fec9b84472a8aa593536ef1b6d8821714499 | All 133,321,580 placements are tested. Three wins, minimum action cost 27 (route length 28), and the tested count equals the placement total. |

sk48 level 4, floor 23. The fixed graph is one snake: the only clickable base is not in a pair, so a click cannot change selection. Breadth-first search of the four move directions expanded every state through depth 22 (10,353,367 states; the depth-22 layer has 3,203,814) and the first win has length 23. The official engine replays that route as a win. The 23-action route matches the same model on every step, including the winning positions. Forty random walks (600 steps) also matched the engine. No shorter win is found.

sp80 level 6, floor 28. The 28-action route wins on the official engine and its 27-action prefix does not. Two further engine wins have length 29. The fixed-instance minimum is 28.

The claim that nothing shorter than 28 wins is the enumeration below, not a live spill of every non-winning placement. Motion graphs are built with the official step. Pieces do not block one another, so the action cost of a placement is the sum of the four distances plus one click for each extra piece that must be selected. All 133,321,580 placements of cost at most 28 are spill-tested by a transcription of the official water rule. The transcription reproduces the winning spill tick for tick, agrees with the engine on 82 placements, and every win it reports is replayed on the engine. It finds 3 wins, minimum cost 27 (length 28), and no win of cost 26 or less.


## Withheld (identified by digest)

The fixed-instance record of sp80 level 6 is a withheld working record. Digest `9736ca64f85128787fe0d61fc49004405ae336dc19a85fdcae1ee0231c564e39`, 6462 bytes. The final floor 28 is stated by `proofs/certificates/floor_cert/sp80`.

The official game files executed by the runs are not part of this pack. Each is identified by its size and SHA-256:

| game file | bytes | sha256 |
| --- | ---: | --- |
| sk48 | 44840 | b6ca05eed7ba81fe5a6a26c58410fa7daddf2039d3d1c42f600aaf9f6f0111e3 |
| sp80 | 39847 | 2306d62311a3cc027250217d86cb4e012898edc7647a7c291ade23f36c9b9715 |
