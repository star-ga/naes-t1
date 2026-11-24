# Development checks

The architecture policy is [.arch-mind/SPEC.md](.arch-mind/SPEC.md). Keep program
sources in `src/mind/` and gate logic inline in the workflow. Data, docs, model
files and result records are artifacts. Changes to architecture rules, their
baseline or workflow require architecture review before landing.

CI runs the architecture gate on every push to main and on every pull request from a branch of this repository. A pull request from a fork skips it, because the scanner image that the workflow pins is not public.

The step with id `gate` in `.github/workflows/architecture.yml` is the canonical CI gate; it needs
only Git, Bash and the tools that workflow calls. It performs a fresh census and scan every time.
With the program sources in `src/mind/` it announces `PRODUCT` and
enforces the acyclic import graph from those sources, together with the source
layout, scripting-source budget (none outside the named places in SPEC.md), tool pin, rules and absence of gateway
bypass findings. A tree without product MIND files announces `BOOTSTRAP` and
checks only the layout, budget, pin, rules and bypass findings; the dependency
score of an empty tree proves nothing about inference.

Building and checking the program itself needs the MIND compiler `mindc` 0.10.4
or later; the build, verify and selftest commands are in the README.

Before you change the sources, first run the gate above, then:

```bash
"$ARCH_MIND_BIN" session-start --repo . \
  --fixture .arch-mind/last_summary.json --agent developer
```

After the edits, rerun the gate to refresh the fixture, then close and verify:

```bash
"$ARCH_MIND_BIN" session-end --repo . --fixture .arch-mind/last_summary.json
"$ARCH_MIND_BIN" check-rules --repo . --fixture .arch-mind/last_summary.json \
  --mode enforce --verify-chain
```

Do not commit the local signing key (.arch-mind/key.local and keys.json), the evidence store (.arch-mind/store/) or the scan outputs (.arch-mind/last_summary.json and last_scan.json).
The checked-in baseline describes the initial zero-source state; it is never
the fixture used to judge later code. CI runs the proposed policy and, on a pull
request with an existing base policy, the base's own pinned gate and rules
against the proposed sources. When a pull request targets a base revision without an architecture policy, CI reports the
missing base policy as bootstrap. CI's local evidence chain is independent of
each developer's private chain.
