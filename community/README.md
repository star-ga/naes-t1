# Community scorecard records

These files are the public ARC Prize community scorecards that the paper compares with. Each directory is one scorecard. The summary file in that directory holds the page score and the recounted means. The per-level file holds the per-level action counts. The RGB-Agent directory also holds a reset-fix per-level file. The file `links.md` in each directory holds the public recording addresses of that scorecard, one in each line.

Each scorecard page is https://arcprize.org/scorecards/ plus the directory name.

The recording manifest is `community_recordings.tsv`. It pins 322 public recording URLs and the SHA-256 of each download.

The column `our_actions` is the per-level action count in `data/demo25_levels_mmae.csv`. The count check reads that table. From the repository root, run `./check_community --csv data/demo25_levels_mmae.csv --community community`.

A row in `levels_below_floor` has `game_version_is_current: false`. It was played on an earlier public version of that game, whose levels differ from the current version; `legacy_game_versions` in the same file names that version. The floors of this repository are the floors of the current versions. No recorded level of a current version is below its floor.

The counting-check receipt is `release_evidence/community/check_community_receipt.json`. What these records are is in `community_manifest.md`. The token <word withheld> inside a tag stands for a part of the tag that this record does not repeat.

Check the file bytes from this directory:

```
sha256sum -c SHA256SUMS
```

Measured by STARGA, not yet verified by ARC Prize.
