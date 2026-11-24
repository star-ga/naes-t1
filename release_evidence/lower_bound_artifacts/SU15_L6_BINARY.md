# su15 L6 search binary

Pack record sha256: `b36275df27179798209cdde07637d34f4dc18cee6a49446b5b60ac60e2b751a6`.

The executable is 36784 bytes and matches that digest. It is a compiled search executable, so it is not included in `release_evidence/lower_bound_artifacts/files/` (see `MANIFEST.md`). This note pins the sources by digest and gives the command that rebuilds it.

## Source pin

The recorded command compiles `bfs.c` and the two headers it includes. All three must sit in the same directory, under these names, at these digests. All three are program source; each is held in the withheld set under its digest (`MANIFEST.md` handling value `program source`).

| name | sha256 | bytes |
|---|---|---:|
| bfs.c | b33f755377351085601d1dd15b00ac3391a41e48f46151d417bca56807c7242f | 10251 |
| su15m.c | e31dc6a2210f9c6daf3865b9be1573e107c8c1c6ac25d8d8836ad224bb53bf9c | 19154 (pack record bytes null; the withheld file matches this digest) |
| prune.h | 075c3d9f6fb4bb32b765bdef1253eb666fe0452d55bd72b31bb30dea47fb9f3a | 10826 (pack record bytes null; the withheld file matches this digest) |

The input beside the recorded run is `L6.cfg`, sha256 `cfe7fdcb2d8f17da3adf131abf225e08bf0867af873798eacba3a4dc2600d02c` (55 bytes, included in `release_evidence/lower_bound_artifacts/files/`). The recorded query is `bfs L6.cfg 4 4 0`.

## Build recipe

Documented command, from the recorded command note at its line 41 (pack record sha256 `e01aa292606127e22ec7380beaf4c34705b830665aa62899bd555dbc5520b762`; the note is held in the withheld set):

```
gcc -O2 -march=native -pthread bfs.c -lm -o bfs
```

The digest matches when that command is run by GCC 11.5.0. The matched binary's `.comment` section is `GCC: (GNU) 11.5.0`. `-march=native` also binds the digest to the instruction set that compiler detects.

The same sources and the same flags under GCC 15.2.0 produce `6422540eed8fe058b1b47283328408bfed598b32925a2c8fb65b7e9d858deadb`. That file is not the pack record row. The recorded row is the GCC 11.5.0 binary.

No search rerun is required. The rebuilt bytes are the recorded bytes.

Strings in the matched binary do not contain a home directory or a source-tree name.
