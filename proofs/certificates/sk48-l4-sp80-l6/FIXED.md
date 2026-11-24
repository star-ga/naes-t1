# Fixed instance for sk48 level 4 and sp80 level 6

sk48 level 4 floor is 23. The fixed graph is one snake: the only clickable base is not in a pair, so a click cannot change the selection. Fixed-instance breadth-first search of the four move directions expanded every state through depth 22 (10,353,367 states) and the first win has length 23. The official engine replays that route as a win at 23 actions.

sp80 level 6 floor is 28. Fixed-instance enumeration tested 133321580 placements and found 3 wins. The minimum cost is 27, which is length 28. The official engine replays that route as a win at 28 actions. The 27-action prefix state is NOT_FINISHED.

`SOURCE_SHA256.tsv` lists each source file, its digest, and whether the published bytes are identical. A whitespace-normalized file has a different digest from the source. The source digest stays in that table.

`sp80_scan.json` omits a parallelism field. The thread count reads `<threads>`. The source digest stays in `SOURCE_SHA256.tsv`. `SHA256SUMS` checks the published bytes.

sp80 level 6: the proven floor is 28. The fixed-instance record is withheld by digest `9736ca64f85128787fe0d61fc49004405ae336dc19a85fdcae1ee0231c564e39`. The floor is stated by `proofs/certificates/floor_cert/sp80`.
