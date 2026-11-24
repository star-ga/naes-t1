# Source extracts

This folder holds 28 source extracts. The certificate records of 65 levels cite them by digest. The file name of each extract is the SHA-256 of the record as it was first written. The certificates quote that digest.

3 of the 28 files had machine names, local paths or labels replaced by placeholders and tokens (`host A`, `host B`, `<path>`, `<label withheld>`). 24 files are edited extracts: the same replacements apply, and in addition any line or clause that only told of an earlier value or earlier state of a level, or of how the work was organised, was removed or shortened. Every proof field, lemma, count, digest and witness of the final value stays. For these 27 files the published bytes differ from the recorded bytes, so the SHA-256 of the file does not equal its name. 1 file is unchanged, and its SHA-256 equals its name.

`MANIFEST.tsv` lists every file in this folder with both digests: `sha256_recorded` (equal to the file name) and `sha256_published` (the bytes in this folder). The column `scrub` says `scrubbed`, `edited` or `unchanged`. The column `status` says `published` or `withheld`. The columns `levels` and `what_it_proves` say which levels cite the file.

Check the published bytes from the repository root with `cd proofs/certificates/lower_bounds/extracts && sha256sum -c SHA256SUMS`.

Rule names such as R1, R1.1, R1.2, R2.5 and R3.4 are labels of an internal rule list. Loop variables and other names in code were not changed.

A record may name working files of its run that are not part of this pack.

12 records are not in this folder. Each record is withheld. `MANIFEST.tsv` lists each one in the section "Withheld (identified by digest)" with its digest and its size. A level record that cites one of them names it as withheld by digest.
