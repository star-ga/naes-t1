# Community records

The directory `community/` holds the public ARC Prize scorecard recounts that the paper compares with. One subdirectory per scorecard. The file list and the check command are in `community/README.md`.

`community_recordings.tsv` pins 322 public recording URLs and the SHA-256 of each download. The URLs are on https://arcprize.org/.

The counting-check receipt is `release_evidence/community/check_community_receipt.json`. The recounted scorecard means are in `release_evidence/community/recount_floor41_pass1.json`. That record keeps its bytes. In its `recording_inputs` list the 13 `per_level.json` digests and the 1 `per_level_resetfix.json` digest match the files in `community/`; the 14 `summary.json` digests are the digests of the summary files before the recounted means were written into them, so they differ from the `summary.json` files in `community/`, which hold the recounted means. The `floor_sha256` digest names a file of that run that is not included in this repository.

Measured by STARGA, not yet verified by ARC Prize.
