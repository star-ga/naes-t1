# Checksums

Every `SHA256SUMS` file lists the sha256 of the files beside it. Check one directory from the repository root, for example `cd proofs/certificates/levels && sha256sum -c SHA256SUMS`.

`proofs/certificates/levels/` holds 183 certificates. The `floor` values add to 5,499. Each floor matches `data/demo25_levels_mmae.csv`. `SHA256SUMS` in `proofs/certificates/levels/` checks the published bytes. Each certificate records `source_certificate_sha256`. On tn36 levels 1-7 that digest equals a log under `proofs/certificates/floor_cert/tn36/logs/`. On wa30 level 9 it equals `proofs/certificates/wa30_L9/RECORD.md`. On every other level the source file is not in this repository. The published certificate omits command lines, host names, and parallelism settings.

`proofs/certificates/g50t-l7-drat/MANIFEST.md` and `proofs/certificates/g50t-l7-drat/CHECKED.md` carry one table. Each row names a formula digest, an uncompressed-proof digest, a gzip digest, and the verdict `s VERIFIED`. The formula files and the gzip files are not in this repository.

`proofs/certificates/g50t-l3-l6-v4/` holds the stored solver receipts of g50t levels 3, 4 and 6, with `README.md` (the counts), `MANIFEST.md` and `SHA256SUMS`.

`proofs/certificates/lower_bounds/NOTE.md` describes the one record per level in `proofs/certificates/lower_bounds/files/`.

`proofs/certificates/sk48-l4-sp80-l6/SOURCE_SHA256.tsv` states the source digest of each fixed-instance file. `SHA256SUMS` in that directory checks the published bytes.

`proofs/certificates/seed_pin/` holds the g50t level 7 seed record. `proofs/certificates/negctl/` holds three numeric rejection results. `proofs/certificates/wt_rerun/` holds the s5i5 level 4 record for floor 30. `proofs/certificates/floor_cert/` holds confirmed rows that match `proofs/levels.csv`. `proofs/certificates/floor_cert/tn36/` holds the seven tn36 closures, the logs those closures cite, and `proofs/certificates/floor_cert/tn36/SOURCE_SHA256.tsv`. The programs and the log that the table marks as withheld are not in this repository.

`proofs/certificates/su15_L3/` holds the record of su15 level 3 at floor 11: two proofs with their logs and sources, a third independent code, and the replay of the certificate route. `proofs/certificates/su15_L3/RECORD.md` there is the summary. `SHA256SUMS` in that directory checks the published bytes.

`proofs/certificates/wa30_L9/` holds the record of wa30 level 9 at floor 57: the replay of the 57-action route with its control, the recorded lower-bound run, and the summary. The floor is proven by empirical proof.

`proofs/certificates/su15_L1/` holds the engine breadth-first search log of su15 level 1. `proofs/certificates/lower_replay/` holds the replay runs of recorded lower bounds for lp85 levels 3, 5 and 6, tr87 levels 2 to 5 and wa30 level 1. `proofs/certificates/complete_key/` holds the whole-object-key searches for s5i5 levels 1, 2, 5 and vc33 levels 1, 2. `proofs/certificates/floor_cert/`, `proofs/certificates/sk48-l4-sp80-l6/` and `proofs/certificates/wt_rerun/` carry the records for ft09, sp80, s5i5, tn36, sk48 level 4 and sp80 level 6. For sp80 level 6 the proven floor is 28.

The public record of a level is its statement file in `proofs/certificates/lower_bounds/files/` (lower bound, method, proof name and Checked line) and its level file in `proofs/certificates/levels/`, which holds the digest of every artifact. Program sources and working records are withheld by the rule of this pack and are identified by digest. Counted from the level files against this tree (the list of digests of withheld records is `proofs/certificates/levels/WITHHELD.md`): 118 levels cite one or more included record files in `release_evidence/lower_bound_artifacts/files/`; 17 levels cite included record files only under `proofs/certificates/`; 48 levels have their statement, their level file with the digests, and one or more source extracts in `proofs/certificates/lower_bounds/extracts/`, and no other included record file. Every level has its statement. This is a rule about the record, not a judgement on any argument.

