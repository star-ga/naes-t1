# NAES-T1 architecture contract

The program belongs in `src/mind/`. It loads the model and consumes rendered
frames to produce actions. Model files, data, documentation, figures, proof
certificates and results are artifacts, not executable product modules.
Outside `src/mind/`, MIND or scripting source is allowed only in the named
places below. None of them is part of the product.

The intended dependency direction is client/serve → inference → tensor kernels
and weight loading. Artifact files supply data and must not become code
dependencies. The scanner enforces acyclicity; it does not infer these conceptual
layers or prove that gameplay uses pixels only. Layer-specific import constraints
need explicit module names. No protection attributes or evidence calls are
required merely to increase a metric: this is an inference program, not a
governance kernel.

## Named non-product source

| Place | Files at this release | Reason | `rules.mind` scope |
| --- | ---: | --- | --- |
| `.arch-mind/rules.mind` | 1 MIND | the policy of this gate | tooling |
| `check_community*.mind`, `fetch_recordings*.mind`, `std_*.mind` at the repository root | 22 MIND | the community check and fetch programs and their std units; README.md, `regenerate_community.md` and the paper name these paths, and the build concatenates them from the root | tooling |
| `.github/scripts/selftest-build.sh` | 1 shell | the build script of the selftest workflow | tooling |
| `figures/src/*.py` | 9 Python | the figure generators | exclude (artifacts) |
| `proofs/certificates/**/*.py` | 29 Python | the independent check scripts of the proof certificates | exclude (artifacts) |

The census admits the root MIND files by exact name, the build script by exact
path, and Python only by suffix in the two named folders. Any other `.py`, `.sh`,
`.ts` or `.mind` file stops the gate. A missing named community file also stops
it. A new file in a named place needs a reviewed change of the census and this
table. Each run prints the count of every class and of the product sources.

## Scope and pin

The canonical gate is the `gate` step in `.github/workflows/architecture.yml`.
It scans MIND under `src/mind/`, with the scopes in `rules.mind`. A census runs
first. It rejects symbolic links to program sources, MIND files outside
`src/mind/` and any tracked or visible untracked `.py`, `.sh` or `.ts` file,
except the named places above. Gate logic stays inline in the workflow: the
pull-request step runs the base revision's own gate text, and a separate script
file would come from the proposed tree instead. Tooling and artifacts are
excluded from the architecture metrics.

The scanner is the image
`ghcr.io/star-ga/arch-mind@sha256:c63d54d8f2aa9914402f8c8b37381438ebfb2189f383052de722b44377e7772d`.
The gate pulls that digest and verifies the SHA-256 of four tool files inside it
before any scan. The MIND reflection producer of this image reports schema 1
and strips one leading `src` or `lib` component from module and import names.
Its scan output does not have an independent scanner-version tag; the image
digest and the file digests identify the implementation. No compiler or sidecar
build is needed for MIND. The image is a private package. When the pull fails,
the gate fails; it never passes without a scan.

## Rules and baseline ruling, 2026-11-04

| Rule | Required raw score | Derivation |
| --- | ---: | --- |
| `acyclicity_q16` | 10000 | Policy: no cyclic import edges. Empty-source measurement is 10000 by convention and is not evidence about a program. |
| `gateway_bypass_q16` | 10000 | Policy: no source-derived gateway bypass findings. The tree has zero findings. |

`.arch-mind/baseline.json` records the zero-source state measured on
2026-11-04, before the program sources arrived. The gate never reads it.
There are no existing floors to lower. Ratios for purity, depth, modularity,
redundancy, protection or evidence density are not floors. In particular,
floating-point weights do not imply a fixed-point purity requirement. Add any
future floor only with a real scoped measurement and a recorded policy ruling;
never lower one to accommodate a regression.

The tree holds product MIND sources under `src/mind/`, so the gate takes the
`PRODUCT` path: it scans the real repository with the declared product scope,
checks that the scan counted exactly the product files of the census, and runs
the rules in enforce mode on that fresh fixture. No checked-in baseline can
substitute for that scan. The `BOOTSTRAP` path is only for a tree with zero
product sources: the scanner rejects a declared product scope with zero matching
files, so the gate then reads an empty temporary directory and checks that the
summary has zero modules and edges. A green gate does not verify inference,
result reproduction or model correctness. No-regression uses the base revision's
own pinned policy against the proposed source tree in pull-request CI, in
addition to the proposed policy.

## Evidence and review

Use session-start and session-end as described in CONTRIBUTING.md. Their private
signing key, store, fixtures and scan outputs are ignored. CI verifies a fresh
local evidence chain; it does not claim to authenticate a developer's private
chain. Changes to this directory or the architecture workflow require an
architecture review before landing. Branch protection and external review
assignment remain repository-owner administration.

Negative controls are performed in isolated temporary copies: an acyclic pair of
MIND modules activates the scoped scan and passes; adding the reverse import
makes enforce mode fail. A misplaced MIND source and a forbidden script also
make the census fail, also when the file is in a named folder with a suffix that
folder does not admit, or at the root with a name outside the named set. These
controls use the same workflow step as the real gate.
