# g50t levels 3, 4 and 6 solver receipts

These files are stored solver receipts.
Each receipt has status UNSAT.

Level 3 has 94 receipts at bound 63.
Of those, 32 are mode final with P from 1 to 32.
Of those, 31 are mode cap with P from 1 to 31.
Of those, 31 are mode pause with P from 1 to 31.

Level 4 has 75 receipts at k 30.
The main grid has 45 receipts in proofs/certificates/g50t-l3-l6-v4/L4/main: 15 are mode final, 15 are mode cap and 15 are mode pause, each with P from 1 to 15.
The release-order grid has 15 receipts in proofs/certificates/g50t-l3-l6-v4/L4/release_order with P from 1 to 15.
The index-0 grid has 15 receipts in proofs/certificates/g50t-l3-l6-v4/L4/index0 with P from 1 to 15.
Each receipt in the two closing grids has field mode full and names solver cadical195.
The receipts in the main grid do not name a solver.

Level 6 has 58 receipts at bound 39.
Of those, 20 are mode final with P from 1 to 20.
Of those, 19 are mode cap with P from 1 to 19.
Of those, 19 are mode pause with P from 1 to 19.
Level 3 receipts and level 6 receipts do not name a solver.
These are solver verdicts. No proof-checker certificate is recorded for these queries.

The encoder is program source. It is not included.
Every receipt names encoder sha256 1b6ea6a032a32fc7d40b27b32b3a1b4ec81060f499539c1afa2e58f763548b41.

Mode final: the receipt names that mode, the bound and P. The receipt does not describe a satisfying assignment.
Mode cap: the receipt names that mode, the bound and P. The receipt does not describe a satisfying assignment.
Mode pause: the receipt names that mode, the bound and P. The receipt does not describe a satisfying assignment.
Mode index0: the receipt names that kind, k and P. The receipt does not describe a satisfying assignment.
Mode release_order: the receipt names that kind, k and P. The receipt does not describe a satisfying assignment.

Verdicts, k, bound, P and solver seconds stay in the receipt.
Check the checksums from the repository root with `cd proofs/certificates/g50t-l3-l6-v4 && sha256sum -c SHA256SUMS`.
