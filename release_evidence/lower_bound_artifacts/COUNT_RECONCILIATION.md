# Lower-bound count reconciliation

The certificate folder in this repository is `proofs/`. The lower-bound list has 627 distinct SHA-256 rows: 610 with a recorded size and 17 without one. The 17 are inside the 627. `MANIFEST.md` maps each of the 627 digests to its outcome: an included file in `release_evidence/lower_bound_artifacts/files/`, a source extract published in `proofs/certificates/lower_bounds/extracts`, the withheld set, or a compiled executable that is not included.

An earlier record counts 609 external lower-bound artifacts. That count is an earlier pack record: 609 rows, 592 with a size and 17 without. A later pack record adds 18 rows and removes none. All 18 have a size: 10 are cited by the level file of g50t L7 and 8 are cited by no level file. 592 + 18 = 610 sized rows. 610 + 17 = 627. 609 + 18 = 627.

## Recorded counts

The earlier pack record has 609 rows: 17 rows without a size and 592 rows with a size. The later pack record has 627 rows: 17 rows without a size and 610 rows with a size. The verification summary count is 609.

## The 18 rows between 609 and 627

Labels are the bundle manifest labels for those digests. A label that may not be public reads `<label withheld>` (rule in `MANIFEST.md`, section Withheld tokens). All 18 are on `SHA256SUMS.originals`. The outcome column is the row's handling value in `MANIFEST.md`: 9 are included in `release_evidence/lower_bound_artifacts/files/` (`unchanged`, `paths replaced`, `tokens replaced` or `paths and tokens replaced`) and 9 are in the withheld set (`over 800 lines`, `internal record` or `internal note`).

| sha256 | bytes | levels | label | outcome |
|---|---:|---|---|---|
| 0a00fe7fe9e54646be187272a8655e984d2a96550bc604790ae10987b1039510 | 2113 | g50t_L7 | REPRODUCE.txt | internal record |
| 2b013d3fc69451fc92831d3530f75a06bc428dc9fb67440dffd8a08d6b3bdc02 | 1411 | g50t_L7 | index0_pctrl_P01.json | paths replaced |
| 41811f60f372bb2c39376a62a2c3b49c327156879214f00e3a27b1e6d8a2cb13 | 20014 | g50t_L7 | index0_precise_dispatch.json | paths and tokens replaced |
| 61a31d049cc1a860340142c9ef0d10cf9d696b31ee8e07fc5d8202a184fc5f0f | 81 | - | build-host_final_1a352e8.txt | unchanged |
| 686b270d2ed7bac38c3c350148c0e6300fede373dc64e2cec6bac3955c91d109 | 52767 | g50t_L7 | <label withheld> | over 800 lines |
| 6a510266c694fe7eaf6e2041e876ef89a7263013c72f7d53cc2d9c4b2724bc3a | 3072 | g50t_L7 | README.md | internal note |
| 821289597d23bde6961243d385f796557ab1abbdbea55189fdc685ad85dd66df | 11314 | - | SHA256SUMS | internal record |
| 939111f8ace342babb9b9a2b6d7a619b5c22fd68c6b88a94b306f9a78357d2f0 | 51167 | g50t_L7 | SHA256SUMS | internal record |
| 9ebcdfa87c1d3d94284884ec82207bbc6cc03328b29493c8d4e8cafef912e281 | 8931 | - | <label withheld> | tokens replaced |
| a296007d63151f7c4b3edc9a163089d4bc2339dc3f835a067d50a18527d250bf | 8933 | - | <label withheld> | tokens replaced |
| aa98af8cb6b4b6e607a32cec25065267e1a75e3784d063ddfda2e58509c8e413 | 592 | g50t_L7 | pctrl_P01.cadical195.json | paths replaced |
| ab51227dd24243f2c1988e7d3acd6dd6bf8b874777e192b4178b90201b901682 | 167 | - | summary.txt | tokens replaced |
| b80dab232f9b61a46633c316f09b6be2c961db4d98ad831edb28d577c3fb9322 | 1693 | - | <label withheld> | internal note |
| c7af589b006e10b46503eea54a457a59fcad0351354c9d2e98b574660518cdf7 | 34969 | g50t_L7 | index0_precise_portfolio.json | paths and tokens replaced |
| cd2c34c5217788aa780018630f2821b48154a14a87047b6fa636633ba53065f4 | 2387 | g50t_L7 | G50T_L7_41_PROOF_20261105.md | internal note |
| e291257ecbfe1e3e135c8ae2ace05754aaa66a692b674b81f69b8c082ab947bb | 1965 | - | README.md | internal record |
| ed02bc263f1e2e9529c7e225cc369817685dd7feaeb4562901be0158d0c51ad6 | 1312 | g50t_L7 | route_41.json | tokens replaced |
| f9db2697233c625d61ae4858a4e2dcb448b8377f52675dba8ec049e8c3e3cb3d | 917 | - | <label withheld> | internal record |

## The 17 rows with no recorded size

These 17 are inside the 609 and inside the 627. Three of them are the compiled search executables that are not included (`MANIFEST.md`); the su15 L6 executable is 36784 bytes.

| sha256 | levels |
|---|---|
| 075c3d9f6fb4bb32b765bdef1253eb666fe0452d55bd72b31bb30dea47fb9f3a | su15_L6, su15_L7, su15_L8 |
| 1b6ea6a032a32fc7d40b27b32b3a1b4ec81060f499539c1afa2e58f763548b41 | g50t_L3, g50t_L4, g50t_L6, g50t_L7 |
| 2a1e65e601bf3bc9d8d8deae4dd592c37ee6b2e13b9a7bb7975cc110a73f511f | - |
| 3e1eb93df7cc6df06134afa5d91016d071a65ef34a4d53225f2d65cf0cc8ee5d | - |
| 451f6e819b0e8ac8ba33e61ae742b21b620511ea960805c2e88e497392e3f323 | - |
| 5ce2b5061c5e1825b2bc9daf254c60869bd6c36441df674d44e582efe9d0f6c7 | wa30_L8 |
| 61eb96db9957be922da58097d47becb3c6674a23f3bd785cdcb96a69a0bc851c | - |
| 6498d2a974ea4ef5166fc5a0f7e547cf99e881b2c78ca284af05dcb0ee121f57 | - |
| 6f21f2065744e484ac38534f940158034e69f39add3b10a3d9fd258de7293aa9 | g50t_L7 |
| 72664e665aaf0f1ad0cee06edc060ec1dc69b24a97eb92e19e3b3a39c934881b | s5i5_L1 |
| 9044d5866aa64b654e38ae04d24e610bf1986fa9e1637754e190d6bf0526b6cb | - |
| a91f0b49c398d42c713459e0e66b427ee24fafbac8ceb1f8a04ff55744da6ffb | wa30_L8 |
| b022f5e7c960499917c609655504462e6e8444ac62807ffc9d12e72732f4be08 | - |
| b36275df27179798209cdde07637d34f4dc18cee6a49446b5b60ac60e2b751a6 | su15_L6 |
| bb20355d6d8f48fafacfdc41366ac6c406d1e3adc84a24a7c16669c056cc0f44 | - |
| d9b7d8978417716006986d6ecb87a959ac026f074cd249a208049d4a63a4ebee | vc33_L3, vc33_L4, vc33_L6 |
| e31dc6a2210f9c6daf3865b9be1573e107c8c1c6ac25d8d8836ad224bb53bf9c | su15_L6, su15_L7, su15_L8 |
