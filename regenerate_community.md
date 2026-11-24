# Community check

This note says how to rebuild the community counting check from the programs in this repository.

The inputs are `community/` and `data/demo25_levels_mmae.csv`. The manifest `community_recordings.tsv` pins the 322 public recording URLs. The saved receipt is `release_evidence/community/check_community_receipt.json`. Measured by STARGA, not yet verified by ARC Prize.

## Build the check program

From the repository root, concatenate these units in this order. The units are `std_json_read.mind`, `std_json_free.mind`, `std_json_scan.mind`, `std_json_values.mind`, `std_json_lexer.mind`, `std_json_parse.mind`, `std_json_dump.mind`, `std_fs.mind`, `std_fs_io.mind`, `std_fs_path.mind`, `check_community.mind`, `check_community_csv.mind`, `check_community_rec.mind`, `check_community_score.mind` and `check_community_report.mind`.

```
mkdir -p build
cat std_json_read.mind std_json_free.mind std_json_scan.mind \
    std_json_values.mind std_json_lexer.mind std_json_parse.mind \
    std_json_dump.mind std_fs.mind std_fs_io.mind std_fs_path.mind \
    check_community.mind check_community_csv.mind check_community_rec.mind \
    check_community_score.mind check_community_report.mind > \
    build/check_community.mind
mindc build --release --backend mlir --out ./check_community build/check_community.mind
```

The `mindc` command writes the program in the current directory. The command reads local files only. With `mindc` 0.10.4 this program builds with `--backend mlir`, which needs `mlir-opt`, `mlir-translate` and `clang` from LLVM 20 on `PATH`.

## Run the count check

```
./check_community --csv data/demo25_levels_mmae.csv --community community
```

Pass `--community` as `community`. A match prints one line per scorecard and ends with the line `# ALL MATCH`. The exit code is 0.

The count check reads `data/demo25_levels_mmae.csv` in this repository. That file is the reference table. The receipt `release_evidence/community/check_community_receipt.json` records the result of that check.

## Recording bytes

This repository does not include the recording bytes. `community_recordings.tsv` pins each public URL and its SHA-256. The fetch units are `fetch_recordings.mind`, `fetch_recordings_plan.mind`, `fetch_recordings_manifest.mind`, `fetch_recordings_hash.mind` and `fetch_recordings_main.mind`, with `std_fs.mind`, `std_fs_io.mind`, `std_fs_path.mind`, `std_sha256.mind` and `std_sha256_compress.mind`.

```
cat std_fs.mind std_fs_io.mind std_fs_path.mind std_sha256.mind \
    std_sha256_compress.mind fetch_recordings.mind fetch_recordings_plan.mind \
    fetch_recordings_manifest.mind fetch_recordings_hash.mind \
    fetch_recordings_main.mind > build/fetch_recordings.mind
mindc build --release --backend mlir --out ./fetch_recordings build/fetch_recordings.mind
```

The program plans from the file `links.md` in each scorecard folder of `community/`. Each line of that file is one recording address.

```
./fetch_recordings --dry-run --community community
```

The dry run reads no network. It lists the 322 recordings of the 14 scorecards from the link file of each scorecard folder and ends with the line `# 322 recordings in 14 scorecards (dry run: no network)`.

```
./fetch_recordings --community community --out <folder> --manifest <new_manifest.tsv>
./fetch_recordings --verify --community community --manifest community_recordings.tsv
```

The first line takes the recording files in `<folder>/<scorecard id>/` that the plan lists, hashes each one and writes a new manifest with the bytes, lines and SHA-256 of each file. For a recording whose file is absent it prints `FAIL`, the address and `download unsupported`. The second line hashes the recording files of the plan and compares each SHA-256 with its row in `community_recordings.tsv`. It prints `MISSING`, `MISMATCH` or `NOT-IN-MANIFEST` for each file that does not match and ends with `# verified <matching>/<planned>`. The option `--only <scorecard id>` limits a run to one scorecard.

The fetch program has no network code: it lists the recording files that are present, hashes them and compares the hashes with the manifest, and it reports a file that is absent as `FAIL` with `download unsupported`. The count check above reads local files only.

## File hashes

From `community/`:

```
sha256sum -c SHA256SUMS
```
