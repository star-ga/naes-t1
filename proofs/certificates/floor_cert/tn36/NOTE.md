# tn36 logs

`proofs/certificates/floor_cert/tn36/logs/` holds the records for the seven tn36 floors.

- `L1.json` is the engine breadth-first search for level 1. Stopped is false. The node count is 6449. No shorter win exists.
- `exact2.json` is the opcode dynamic program for levels 1-7. Levels 1-5 match the floor. Levels 6 and 7 have no winning single program.
- `replay.json` is the replay of the seven certificate routes on the official game file. Each route wins at the floor. The prefix one action shorter does not win.
- `bound.json` is the max-pop lower bound by run count for levels 6 and 7.
- `k3.json` enumerates run counts 2 and 3 on levels 6 and 7. Every enumerated count has capped 0 and an empty best chain. The level tag OPEN in that file names run counts absent from that file.
- `k5.json` enumerates the remaining run counts under the bound. The level tag is CLOSED. Shorter is null. The open list is empty.
- `k5_marks.txt` holds the marker lines of the full enumeration trace, including the chain-table check and the CLOSED lines.

`SOURCE_SHA256.tsv` names each file in this directory and each program that produces the records. The programs are not in this repository. The full enumeration trace is not in this repository. It exceeds 400 lines. Its sha256 is in that table. The bytes in `proofs/certificates/floor_cert/tn36/logs/` are the source records, copied without a rewrite. `SHA256SUMS` in the parent directory checks every file here.
