# Official-engine floor certificates

One directory per game. Each SUMMARY.md states the account, the commands and the verbatim results; levels.json gives the per-level verdict and claim code. Claim codes: C = a current-engine minimum with no lemma or model condition; Cl = the claim keeps lemma premises that the coverage ledger records for the producer; Cm = the claim rests on an explicit model of a game rule.

Included files are copies of the certificate records. In text files a host path or an interpreter name is replaced by a token: `<interp>` is the interpreter the `.py` programs are written for, `<threads>` is a worker thread count, `<out>` is the certificate directory, `<path>` is a local checkout path. A SUMMARY.md or levels.json rewritten for this bundle states the same results in final form. `original sha256` is the digest of the record as written by the run; `included sha256` is the digest of the file here. Digests quoted inside SUMMARY.md and levels.json are of the original records; where an included record differs from its original, both digests are listed below. Program sources and the full search log are withheld: each is identified by its SHA-256 in the section "Withheld (identified by digest)" below.

Verify the included bytes: `cd proofs/certificates/floor_cert && sha256sum -c SHA256SUMS`.

| file | bytes | included sha256 | original sha256 | what it is |
|---|---:|---|---|---|
| `proofs/certificates/floor_cert/sp80/SUMMARY.md` | 7421 | `89e3c58ea1000e09d31f81a5757dc4f148d93c8416ed750fc846684e80c99c57` | `cbef588a22b60061b71a9f6382ddad53edf5840d3cb0d5c0a6c22854deeee0a7` | certificate summary, rewritten |
| `proofs/certificates/floor_cert/sp80/levels.json` | 5046 | `c1efdb0c5c2357d49865df1f1f1b14b6cc1f12ea0bc0d6312aede3b08d89d7c3` | `9bd258e5f29b77c0e82641475b884db392014377cd2cc778c935f448ec5db271` | per-level verdicts, rewritten |
| `proofs/certificates/floor_cert/sp80/raw/L1.json` | 724 | `f2825c350bd9b6402e37e6dfdea9e5f02a438a09551930892b977b5eaa12dabc` | same | raw record |
| `proofs/certificates/floor_cert/sp80/raw/crosscheck.err` | 114 | `fdcd7bc0d6dfc4db5eadd8c01c8382bb360d50a0498742c385e40033b940e543` | `e1676b82cf39ba60cd2fe9ea19ee7a3be4dd4b741c53274083bff6c6f255f2d9` | raw record |
| `proofs/certificates/floor_cert/sp80/raw/crosscheck.txt` | 404 | `f13ff788ff93c8b15e0bf49d1b8c51ccb0f9862b182d16dbbf8de9a8684dd12f` | same | raw record |
| `proofs/certificates/floor_cert/sp80/raw/engine_replay.txt` | 998 | `a2f529ec68d3d54ead44421a426f5546292ac816f92ff41f5cf8bdd6795c69a8` | same | raw record |
| `proofs/certificates/floor_cert/sp80/raw/replays.json` | 1993 | `52458539aabdc6d0e29f5abe111785d75d3d6f9d5754010060314e42096c93a6` | same | raw record |
| `proofs/certificates/floor_cert/sp80/raw/scan4.err` | 149 | `6d0f589d600d4c2ab1d3e7571ac37f926e727f9972a14bc979f816928795e638` | `d1fec0e85d2b9f25c042f5f7afdee8db12cece75514f253952ae66c9b18f3527` | scanner progress log |
| `proofs/certificates/floor_cert/sp80/raw/scan4.txt` | 101 | `26feac1073dea8add29c9f0f2c7c0d5e1057c05504c083c855f074d9d124648e` | same | raw record |
| `proofs/certificates/floor_cert/sp80/raw/scan5.err` | 58 | `ac7e213210065a05ca5c11030bad27f317a54907639064f0075f77b516431bfe` | `4c415519fd7c7d87d7f9dca4b515e15daac5141a77b83758856436b48815d4f1` | scanner progress log |
| `proofs/certificates/floor_cert/sp80/raw/scan5.txt` | 97 | `a282d608792d5b3f125be6f33fd97b4e37f4fdd419ff9b8ac0e58d277d11ce5c` | same | raw record |
| `proofs/certificates/floor_cert/sp80/raw/scan6.err` | 130 | `69c9e9f1d7193e727227896e0d171e6648514d238c1d93d9c2742e2c5941716b` | `e40259d0b17c158bb379f580cac48e521bb7ba58dfc0aeb8d18471ffa1b4792c` | scanner progress log |
| `proofs/certificates/floor_cert/sp80/raw/scan6.txt` | 99 | `f07cc8cd213bd3cc6f5ac3cab1acd684ebc2d979013f672a7c44f4c9a805ca23` | same | raw record |
| `proofs/certificates/floor_cert/sp80/inputs/level2.bin` | 395628 | `511245dc60a19bab4d2f5ddffaf63a98c1a5296a7e536b1181bf4160036a797a` | same | scanner input |
| `proofs/certificates/floor_cert/sp80/inputs/level2_model.json` | 6563 | `cb8f69d3f4e88af5ba6e1b4b9fb565f9baf27cb679d181f9ea188fd0a5c24e91` | same | per-piece motion graphs from the official step |
| `proofs/certificates/floor_cert/sp80/inputs/level3.bin` | 396076 | `7169ec48a1aedf7a6595b460067a4ef7062c97f5476ee5f1994b889020ca8490` | same | scanner input |
| `proofs/certificates/floor_cert/sp80/inputs/level3_model.json` | 7906 | `810dcbcb56d39187ef18b08ea6700405829ba29dd07bd2882d10cb438acc4d27` | same | per-piece motion graphs from the official step |
| `proofs/certificates/floor_cert/sp80/inputs/level4.bin` | 399790 | `2fa1c91e160b66f7c9601561b1c25eb8506d290290a06247f1c0974ec46af53e` | same | scanner input |
| `proofs/certificates/floor_cert/sp80/inputs/level4_model.json` | 15846 | `623a3aeca1ee7d38feeff74458e971aee028559ae7cf0addbc6dae3463b8648c` | same | per-piece motion graphs from the official step |
| `proofs/certificates/floor_cert/sp80/inputs/level5.bin` | 398220 | `54e4c4e1c2e9fb499031b8b17e2da14c0f7b817d49d915835cf10347a9c3f0fb` | same | scanner input |
| `proofs/certificates/floor_cert/sp80/inputs/level5_model.json` | 13378 | `b2d7c923be8b8625aced6f4021cda68266db473a8b19dd53c97bb6988e6d3088` | same | per-piece motion graphs from the official step |
| `proofs/certificates/floor_cert/sp80/inputs/level6.bin` | 397598 | `6b773eb10774980230498f3beefb810ea01b0dfbbaea7d080a1730788dce4f27` | same | scanner input |
| `proofs/certificates/floor_cert/sp80/inputs/level6_model.json` | 12055 | `984b850d03ead1fd4767331596a949502f873f16923bbbb091de9773018cb645` | same | per-piece motion graphs from the official step |
| `proofs/certificates/floor_cert/ft09/SUMMARY.md` | 8330 | `1ce793e382640ea43169b018af8f9a01445c38c3cefb2c19fca5a3fafcc3186a` | `f39d2af255c68264960c6d96e74da7cd81ced39e43b3dbde518a062003f24e69` | certificate summary, rewritten |
| `proofs/certificates/floor_cert/ft09/artifacts/L1.json` | 2341 | `08970d1dace099f699b093a64dce7d49f17b31335e82459d73e30a3bc92b130c` | `7f7f09c2f5a97783a9b7b21793186679b0f66693f6e6c4efc7fc446f92fb0de7` | per-level certificate record |
| `proofs/certificates/floor_cert/ft09/artifacts/L2.json` | 2446 | `eb0e799077431d09cbfdea5b47142c7313261c44db07cc2f2be36a5dbc8fc57d` | `a2e0d9ba5246d3d2cfce1f8ff9f7f87a106478e078361f8a5dfd3951bb823ed2` | per-level certificate record |
| `proofs/certificates/floor_cert/ft09/artifacts/L3.json` | 2347 | `b2a00d4fa4ac2f02eb37c1ad4fe6c991f1e2a51f4ca772801fbab5638ea1660e` | `a3ce622bcd19ba4b67842f4fcaf6e2010d94f9422f904aa28c6d6bba52447f29` | per-level certificate record |
| `proofs/certificates/floor_cert/ft09/artifacts/L4.json` | 2256 | `561960a1276849d860b26fee4a30901b4ed8eed975bc6c347f7b0df38b088f2c` | `cce1f0f35be79f1da47b33b3626611f5e33695928d2266b90ab89cd40ae97467` | per-level certificate record |
| `proofs/certificates/floor_cert/ft09/artifacts/L5.json` | 2462 | `3c8d0187578a765fcbf50932605f37e4f10c7d049cc9a2cc246e5b4302538f19` | `aa653b92e3ad19325326ae7877b6e10b34fed2e0aba5597be1eb755010511fab` | per-level certificate record |
| `proofs/certificates/floor_cert/ft09/artifacts/L6.json` | 2302 | `8c318902f23f2c0c215696822d4926012e678c9ee5f1c828d1139cb2dc2c51a1` | `58655bdbe4904055d1975263d517bb54e54ec104bfbe31a9d66fb89bdca566b0` | per-level certificate record |
| `proofs/certificates/floor_cert/ft09/levels.json` | 5650 | `405bfb9c204e039cb6bcb275306727cade3d24098a7ca17d342a5e28a443e795` | `1091642876e83a496df975f13c8d5e17deb915511104de7d8d474e6aa48ab1bb` | per-level verdicts, rewritten |
| `proofs/certificates/floor_cert/ft09/logs/certify.txt` | 355 | `e8578dbcd7c6bfaa8fc705ef4b4fba43dc11dceae44c690c72b25a667e30fb6f` | same | certification log |
| `proofs/certificates/floor_cert/s5i5/SUMMARY.md` | 3912 | `746d962ff2a4af24f7f878781a6606cff0430e6e8507993383d8f62477798412` | `47ad3c1bab4c1ae4b7c74263d15b11b12332abfbeed0b22332f80fec949284f5` | certificate summary, rewritten |
| `proofs/certificates/floor_cert/s5i5/levels.json` | 4674 | `11f577b1df25b9294bdb18957d81a99f22a7e8b035a575304e5efddad3649ec2` | `0df1ac3c21bd88cdff138294054259090ea55cef0925e52da23fb5787ed5ebe9` | per-level verdicts, rewritten |
| `proofs/certificates/floor_cert/s5i5/raw/L1.json` | 4633 | `be30d17cee3b6c73b5cbdd614d14f50ca55795b8f2601479a560956601e3171d` | same | raw record |
| `proofs/certificates/floor_cert/s5i5/raw/L1.log.txt` | 1029 | `9f9c0d9beecbc64bde5dff28149ec277a499070b3df17b32de1747ca70c39b00` | same | search log (included as .log.txt) |
| `proofs/certificates/floor_cert/s5i5/raw/L1.sha256` | 235 | `8ac095b93f52939cf864bc65ff0d0c9dfb6fbaff1eb862881a552ee414318357` | `e657ce82347d74976988c94765270f407e5500ace6c5e26ff8328dc9d9236a27` | raw record |
| `proofs/certificates/floor_cert/s5i5/raw/L2.json` | 7410 | `70e90870dfa90c7c515753420dd6e63541fd3a30ab0525dd55547aec57332db7` | same | raw record |
| `proofs/certificates/floor_cert/s5i5/raw/L2.log.txt` | 2051 | `ba5ca8ab27ceee386f469166979721497a314a7082115a72863034e0312b336b` | same | search log (included as .log.txt) |
| `proofs/certificates/floor_cert/s5i5/raw/L3.json` | 10008 | `8d55d353592a9f75fe4b92a2a59626b153d0871d01e0fb3c6b85771278d45eae` | same | raw record |
| `proofs/certificates/floor_cert/s5i5/raw/L3.log.txt` | 3038 | `94deb890220d29e10a7e58332ec40f1a275a1ed8f312893d3edc61d3ec41353f` | same | search log (included as .log.txt) |
| `proofs/certificates/floor_cert/s5i5/raw/L4.json` | 8473 | `7391b13b06385812f9775eefba81c4224d895a8ea6cf105868abc7af0cba470f` | same | raw record |
| `proofs/certificates/floor_cert/s5i5/raw/L4.log.txt` | 2438 | `f51998d57442113aba94050d41a77955fc4dcb4d37b3edf16264c08f575cb039` | same | search log (included as .log.txt) |
| `proofs/certificates/floor_cert/s5i5/raw/L5.json` | 7691 | `e74fb984ff0b6adde7b23e53b7ed3a55ff2fe936e932d4dc807a37f2c8d22aaa` | same | raw record |
| `proofs/certificates/floor_cert/s5i5/raw/L5.log.txt` | 2209 | `543244aebd81138f424069f9fa088cac204df9da3e1e4b52a924bd2c804de0a4` | same | search log (included as .log.txt) |
| `proofs/certificates/floor_cert/s5i5/raw/L6.json` | 7313 | `15ba9c834744f58103820d20bfe26f3f2392ac9f0bf2d8174c772667229488f1` | same | raw record |
| `proofs/certificates/floor_cert/s5i5/raw/L6.log.txt` | 2048 | `c2750dccac369600af0be81afd880de1ff3924a5360b04f93c1c31a39e97a40e` | same | search log (included as .log.txt) |
| `proofs/certificates/floor_cert/s5i5/raw/L7.json` | 11346 | `89af4ef3051d89614f80428d9b809fa10d49388fcd053b20fde8d589ae834587` | same | raw record |
| `proofs/certificates/floor_cert/s5i5/raw/L7.log.txt` | 3819 | `190c51fc363fba3b564aee5c554ce3e18bff0fc0ec49d9cb8dccdde9eb590691` | same | search log (included as .log.txt) |
| `proofs/certificates/floor_cert/s5i5/raw/L8.json` | 9945 | `d0d3ea19856bf80d1ed862bd89af591db188c110d75ceea1e2c0a435b9d1c3b9` | same | raw record |
| `proofs/certificates/floor_cert/s5i5/raw/L8.log.txt` | 2914 | `8580bbfca0c5d6ad769fadfbeffe6ecbf7042d8cb2ab1417293c1b30704d4445` | same | search log (included as .log.txt) |
| `proofs/certificates/floor_cert/s5i5/raw/finished.sha256` | 906 | `11159ca03583704711b5e26c70ccbfba7492c2ce7dd5fcd10b04219634e1f590` | `c0b37889cfcaa002ce2aebb69484655f0d1c3021cc4f481ad230a719cdd155aa` | raw record |
| `proofs/certificates/floor_cert/tn36/SUMMARY.md` | 7334 | `99e3341581b33c673da941cfe51b60c36809b9b6388bd36b842c9b49008c8f4f` | `c6e9f4782231a365844da660e1bc8bf48d86f645781c4fa3884bc5dc8b5ca3bd` | certificate summary, rewritten |
| `proofs/certificates/floor_cert/tn36/levels.json` | 6292 | `29096da4fc2a45d5da20464c79a1ec32911f881a8f7c49c2cf2c09a481d43bb7` | `e281044ed3b72a22f75f017e24481d064a63a9103be1b6c2e31277e341339acc` | per-level verdicts, rewritten |
| `proofs/certificates/floor_cert/tn36/logs/L1.json` | 1088 | `ae7cf9c80b752303bf596db18af5a84f5012f63e1716678d6795decf633173ed` | same | search record |
| `proofs/certificates/floor_cert/tn36/logs/probe.json` | 14874 | `13310ec4da4a04416a44f0a0afae33e93b84e9e0a1eb34c64684cfeb4dfd7a8b` | same | search record |
| `proofs/certificates/floor_cert/tn36/logs/exact2.json` | 9057 | `9674f8aeee9cd431dd9e132e20c64698ac2ec740b8d16d0091e1999fff4213d3` | same | search record |
| `proofs/certificates/floor_cert/tn36/logs/replay.json` | 1678 | `288134e4059897984a9a94dded74d86824aa060935c6c03fc672eff2aa7aa64a` | same | search record |
| `proofs/certificates/floor_cert/tn36/logs/k3.json` | 1915 | `7840cf0e66638c2cac728b3b0e968833c45d3057a359446feb4f627dba5294df` | same | search record |
| `proofs/certificates/floor_cert/tn36/logs/k5.json` | 1400 | `d0ef390c18abea5843aae6858a3c550d458a2534505c58a8261f682e3e46c449` | same | search record |
| `proofs/certificates/floor_cert/tn36/logs/bound.json` | 231 | `9bb87068631129ed56775e88b69c5cad6ae6852c71317f36c1c9499d203674a9` | same | search record |
| `proofs/certificates/floor_cert/tn36/logs/k5_marks.txt` | 1235 | `51f0580a427b2d95cd8aea2ea94f04efec515dca7b470f189205d18428b5d84b` | same | marker lines of the search log |

## Withheld (identified by digest)

These files are not part of this pack. Each is identified by what it is, its size in bytes and the SHA-256 of the original.

| what it is | bytes | original sha256 |
|---|---:|---|
| sp80: scanner source | 18609 | 2989062ee065340b819fef26699c9d86b849090c7f05a677f2e8ca5399d2d1fa |
| sp80: packs a level model into a scanner input | 3438 | d603d42ca1b39e13b77f1d1104b59790060542211c8c0f10c6f0fe4594b148da |
| sp80: builds the motion graphs and runs the official-engine spills and replays | 30412 | aaa4544b1fd65ec34160e93dd9c0149ba6a09b743501be71e8dc6f5d8697ab0c |
| ft09: certifying program | 38698 | c6608247f8ad32c655a92cfaca785d3a775cd586f1b28d8e3be30762156f31cb |
| s5i5: search program | 26889 | 899ee3f0fe85c0d258ecf75defc9e0c4703abd6fab4db0ecf399b66a9050a9e5 |
| tn36: full search log | 8611923 | cf39d277e667e28758ee37aae5379cf08c39219d346e67c6b6eb33e96d54fcbd |
| tn36: engine search program | 37661 | 69c766c77cb052a6f8c33b2b1da7c3d1328f5da6a1668cf8fa1e80b7fc87a3f8 |
| tn36: exact opcode program | 43065 | 3eabee389e3fa2f42a6909252163e91c0391227496322f64cf9c19b2956ac906 |
| tn36: official-engine replay of the seven routes | 2781 | ee5de5835ac9ee8eb046641fbd6dc190b59bd1247296eeb25ce2e6159db46b29 |
| tn36: two- and three-run chain search | 29334 | eaad4ce5d5ed904270ba62c393dc688362203d2ade29dc66effcf65c87e59300 |
| tn36: four- to seven-run chain search | 18624 | 9e186d2716ec282940966571768356f760d1c30063d7505387bb56ba670d1207 |
| tn36: run-count bound | 3117 | 8b6fd5f7071872ccfc7fff900ff41a3f63d443732caf20ce3eb15c98a6648b0e |
