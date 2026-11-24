# Release record: build with mindc 0.10.4

Compiler: mindc 0.10.4 (tag v0.10.4 of github.com/star-ga/mind, commit 90acf89bfacae3d5664d5d6b5d183e1d81dc2399). Archive sha256: x86-64 b887ddedaa03a5a5d7124456d4b433bce5782b66d9dacac58c9dcf9083dc8003, aarch64 4856acf7360e196e09d7576fd3d2d4b3c2c98d344350c313a2c20fba5d7f2990.
Build (x86-64, native backend): mindc build build/naes_t1_unit.mind --no-cache --out naes_t1 (the unit of src/unit_order.txt: 31 standard-library modules and 46 sources). Build (aarch64, and x86-64 through the MLIR backend): mindc build build/naes_t1_unit.mind --backend mlir --no-cache --out naes_t1 (the unit of src/unit_order_mlir.txt: 31 standard-library modules and 21 sources).
Program unit sha256 (x86-64): 7536fd8a7334f1a84dd476de21849765fb66f06d16087e3d2298e204c3b6443c. Executable sha256 of this unit: 0e0398866f8252a62557b32816b44eac5e036094b42b08e9ccd98bc377f2f61b (x86-64, native backend; two builds are byte-equal).
verify: MATCH 5499/5499, decision-stream digest 4d2d98f711a1209eace8f3841952e39d4d75870d48079c3175b361bba9746396. selftest (two runs equal): e0c8ae9167d99b37b00194fb3bdf5d545952ae4fc223e0c2d5be3382708d7444.
bench with 1, 2, 4 and 8 worker processes: MATCH 5499/5499 and the same decision-stream digest for each count.
Evidence of the network kernel evidence/naes_t1_kernel.mic3: sha256 3b31a74d78148283c7cf87e4a1a4c18a83edad0997f6c0ffcefc30d650504985, attested deterministic, scope whole_program.
Community count check built with mindc 0.10.4 (MLIR backend): ALL MATCH.
The records under release_evidence/compiler that name no compiler version belong to the earlier compiler release.
