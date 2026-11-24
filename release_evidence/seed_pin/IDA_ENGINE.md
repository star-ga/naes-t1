# wa30 level 8 and level 9 IDA* engine pin

Package name: not recoverable.
Package version: not recoverable.

The bundled level 8 logs and the level 9 prover log contain no `arcengine` string and no `arc-agi` string. The bound-log directory does not contain a package manifest. The certificate field `recorded_game_source_sha256_in_lower_correspondence_receipt` is null for both levels, and `recorded_source_game_sha256` is null for both levels.

## Level 9, recorded identities

The prover log start line contains these tokens. Machine identity and concurrency fields on that record are omitted here.

- `bin_md5=9b99292902a37261564e000d40415086`
- `src_md5=25bc5e4bc6fb8a6832bca42656948ab4`
- `hnew_md5=53d113b1d45155bb1735733a828d77d4`
- `lev_md5=409515c94b6ba0695b62187cbb958f9b`

The level 9 certificate `recorded_runtime_identities` records:

- `lower_binary_recorded_md5` `9b99292902a37261564e000d40415086` (same as `bin_md5`)
- `lower_binary_sha256` `451f6e819b0e8ac8ba33e61ae742b21b620511ea960805c2e88e497392e3f323`
- `levels_recorded_md5` `409515c94b6ba0695b62187cbb958f9b` (same as `lev_md5`)
- `levels_sha256` `74fde749266c79580cfd7147f6f4b82f84028a698aa0924e3a3176c1d44f651f`
- `recorded_game_id` `wa30-ee6fef47`
- `recorded_game_source_sha256_in_lower_correspondence_receipt` null

The recorded replay command names the binary `wa30l9h`. That file is not included in this release. No binary is re-hashed here.

## Level 8, recorded identities

The level 8 certificate `recorded_runtime_identities` records:

- `lower_binary_recorded_md5` `62a6ce09177877fe7930e7607e280a7d`
- `lower_binary_sha256` `a91f0b49c398d42c713459e0e66b427ee24fafbac8ceb1f8a04ff55744da6ffb`
- `levels_recorded_md5` `409515c94b6ba0695b62187cbb958f9b`
- `levels_sha256` `74fde749266c79580cfd7147f6f4b82f84028a698aa0924e3a3176c1d44f651f`
- `associated_positive_control_game_source_sha256` `0c8b63caee2d092d6c0750fd74e417de682d09973b549046394dadf0437b1477`
- `recorded_game_source_sha256_in_lower_correspondence_receipt` null

`engine_replay.out` line 1:

```
ENGINE wa30.py sha256 0c8b63caee2d092d6c0750fd74e417de682d09973b549046394dadf0437b1477
```

That sha256 matches the pinned engine file named in the positive-control record. The engine file and the file `GAME_INSTANCES.json` of the pack are not included in this release. It is the positive-control source hash. It is not a hash stored in the lower-run receipt.

## Recorded prover source hashes

`release_evidence/remote_lower_bounds/bound_logs/wa30_L9_20261101/SHA256SUMS_src.txt` in the level 9 bound log lists sha256 values for the prover sources. This check re-hashed the three files that are present in that log's `src/` directory:

- `release_evidence/remote_lower_bounds/bound_logs/wa30_L9_20261101/src/levels_full.txt` matches `74fde749266c79580cfd7147f6f4b82f84028a698aa0924e3a3176c1d44f651f`
- `release_evidence/remote_lower_bounds/bound_logs/wa30_L9_20261101/src/levels_L.txt` matches `516e37b816594f94688db9d0cb48241d14ae7a0a38ee17641e1ed61c38fbf43a`
- `release_evidence/remote_lower_bounds/bound_logs/wa30_L9_20261101/src/levels_R.txt` matches `7f4d2b05ca3afa08b9e063804b94b9af679f604391750b5d450b56d92d19b904`

The other names in that sums file (`dls.c`, `e1b_aR.c`, `e1c_aR_cross.c`, `e1_ronly.c`, `e2_lsearch.c`, `h0sched.c`, `r_need.c`, `stepper.c`, `sym2.c`, `sym_l.c`, `eng.h`, `eng_track.h`, `hmap.h`, `hset.h`, `engine_diff.py`, `routes61.json`) are not included in this release. Their recorded hashes are not recomputed here.
