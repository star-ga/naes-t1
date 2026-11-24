# Release evidence checksums

`release_evidence/SHA256SUMS` lists the SHA-256 digest of each file in this folder. Verify with `cd release_evidence && sha256sum -c SHA256SUMS`.

GPU receipts, one line each:

- `release_evidence/gpu/rtxpro6000_exact_5499.json`: exactness of the protected GPU build on the release fixture, measured on an NVIDIA RTX PRO 6000 Blackwell Server Edition.
- `release_evidence/gpu/rtxpro6000_trt_exact_5499.json`: TensorRT exact counts on the final model, measured on an NVIDIA RTX PRO 6000 Blackwell Server Edition.
- `release_evidence/gpu/gpu_exact_5499.json`: exactness of the protected GPU builds on the release fixture.
- `release_evidence/gpu/per_variant_release.json`: each variant of the protected GPU builds with its result on the release fixture, from the two exactness receipts.
- `release_evidence/gpu/loader_variants.json`: the list of the variants of the protected GPU builds.

TPU receipt, one line:

- `release_evidence/tpu/device_exact_5499.json`: exactness of the protected TPU builds on the release fixture, measured on a TPU v5e (twelve runs in two passes).

Timing, cost and scorecard records, one line each:

- `release_evidence/perf/published_timings.json`: the published timing records (CPU, NVIDIA RTX PRO 6000 paired run, TPU, earlier GPU engine set), as the article and the paper print them.
- `release_evidence/cost/retail_cost.json`: the published retail CPU cost, as the article and the paper print it.
- `release_evidence/cost/retail_tariff.json`: the public price-list entry of the cost: version 20261103235716.
- `release_evidence/play/scorecard_tags.json`: the tag line of the scorecard, from play/scorecard.json.
- `release_evidence/play/mode_a.json`: the closed-loop replay of the 25 public demo games by the release executable through the gateway of the official toolkit (25 of 25 games, 183 of 183 levels, 5,499 actions, every level at its floor).
- `release_evidence/program/unit_files.json`: the source files of the two program units (the x86-64 native unit and the aarch64 MLIR unit): the unit lists, the counts and the digest of the x86-64 unit.
- `release_evidence/compiler/native_build_0104.json`: the build and the checks of the release executable with the compiler release mindc 0.10.4 (tag commit, archive digests, digest of the standard library, unit and executable digests, selftest, verify and bench results).
