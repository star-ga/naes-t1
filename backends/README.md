# Optional GPU and TPU builds

The CPU build is open source. The GPU and TPU builds are optional. They are STARGA's commercial MIND backends, so their binaries are protected.

The open CPU path is the complete system and reproduces every result; GPU inference is OPTIONAL through STARGA's commercial, protected MIND backend and NOT REQUIRED for any result.

| Path | What it is | Licence |
|---|---|---|
| `backends/gpu/gpu_run_side2h_5499.sealed` | NAES-T1 protected GPU module (optional), for cards with compute capability 8.0 and above | Protected binary; part of STARGA's commercial MIND backends; optional, not required for any result. |
| `backends/gpu/gpu_run_t4_5499.sealed` | NAES-T1 protected GPU module (optional), for cards with compute capability 6.0 up to 7.5 | Protected binary; part of STARGA's commercial MIND backends; optional, not required for any result. |
| `backends/tpu/pjrt_fb2` | NAES-T1 protected TPU loader (optional) | Protected binary; part of STARGA's commercial MIND backends; optional, not required for any result. |
| `backends/tpu/pjrt_fb2r` | NAES-T1 protected TPU loader (optional) | Protected binary; part of STARGA's commercial MIND backends; optional, not required for any result. |
| `backends/tpu/pjrt_kat` | NAES-T1 protected TPU loader (optional) | Protected binary; part of STARGA's commercial MIND backends; optional, not required for any result. |
| `backends/tpu/e3_check` | NAES-T1 protected TPU checker (optional) | Protected binary; part of STARGA's commercial MIND backends; optional, not required for any result. |

Check the six files: `cd backends && sha256sum -c SHA256SUMS`.

The exactness receipts of the two GPU builds are `release_evidence/gpu/rtxpro6000_exact_5499.json` and `release_evidence/gpu/gpu_exact_5499.json`. The build digests in these receipts are the digests of the two GPU files of this folder. A passing GPU check matches the final-run decision stream. `release_evidence/gpu/per_variant_release.json` lists each variant of the two builds with its result, and `release_evidence/gpu/loader_variants.json` lists the variants.

The receipts also give the digests of the kernel inputs and of the control fixture of each run; these files are not part of this repository.

The exactness receipt of the TPU builds is `release_evidence/tpu/device_exact_5499.json`. The build digests in this receipt are the digests of the four TPU files of this folder. A passing TPU check matches the final-run decision stream.
