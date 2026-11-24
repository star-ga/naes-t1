"""fig8_proof_method_by_family.png: proof method by mechanic family.
usage: fig8_proof_method_by_family.py [levels_csv] [out_png]"""
import csv, sys, collections
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import Patch, FancyBboxPatch

csv_path = sys.argv[1] if len(sys.argv) > 1 else "../../data/demo25_levels_mmae.csv"
out = sys.argv[2] if len(sys.argv) > 2 else "../fig8_proof_method_by_family.png"

CLASSES = [
    ("Exhaustive search (uniform cost / breadth-first)", "#2a78d6", "white"),
    ("Exact math (algebra, closed form, enumeration)", "#eb6834", "white"),
    ("Matching bounds (lower bound = verified win)", "#1baf7a", "white"),
    ("A* with an admissible heuristic", "#eba000", "black"),
    ("Bounded SAT (transition unrolled to one action below the floor; unsatisfiable)", "#6b4c9a", "white"),
    ("Empirical proof (winning route replayed on the engine; mathematical lower bound below it)", "#8a8984", "white"),
]
# row order as in the published figure (top to bottom)
ORDER = ["collect/route", "peg merge", "directional nav", "fruit merge/vacuum",
         "gravity+push", "sokoban + enemies", "arm kinematics", "phase coverage",
         "placement/assignment", "sequence/order", "sokoban-like push", "track/button",
         "block push + fuse", "constraint/bridge", "fuel-limited nav", "program/opcode",
         "record-replay ghosts", "coupled-control maze", "edit-and-commit plumbing",
         "gated navigation", "pick-place", "rigid-body assembly", "spell/macro",
         "symmetric paint"]

cnt = collections.defaultdict(collections.Counter)
n = 0
for r in csv.DictReader(open(csv_path)):
    cnt[r["mechanic_family"]][r["proof_method"]] += 1
    n += 1
assert set(cnt) == set(ORDER), set(cnt) ^ set(ORDER)

BG = "#fcfcfb"
# Taller canvas so the six class names sit under the bars, not on them.
fig = plt.figure(figsize=(11, 10.8), dpi=200, facecolor=BG)
ax = fig.add_axes([450 / 2200, 472 / 2160, (2166 - 450) / 2200, 1592 / 2160], facecolor=BG)
for i, fam in enumerate(ORDER):
    left = 0
    for name, col, tcol in CLASSES:
        v = cnt[fam][name]
        if not v:
            continue
        pad = 0.03
        ax.add_patch(FancyBboxPatch((left + pad, i - 0.34), v - 2 * pad, 0.68,
                     boxstyle="round,pad=0,rounding_size=0.05", mutation_aspect=0.5 / 0.1 * 0.1 * 8.3,
                     fc=col, ec="none", zorder=3))
        ax.text(left + v / 2, i, str(v), ha="center", va="center", color=tcol,
                fontsize=8.3, fontweight="bold", zorder=4)
        left += v
ax.set_ylim(len(ORDER) - 0.5, -0.5)
ax.set_xlim(0, 12.5)
ax.set_yticks(range(len(ORDER)))
ax.set_yticklabels(ORDER, fontsize=11, color="#444")
ax.tick_params(axis="y", length=0, pad=6)
ax.tick_params(axis="x", labelsize=11, colors="#777")
ax.set_xticks(range(0, 13, 2))
ax.set_xlabel("levels", fontsize=11, color="#333")
ax.grid(axis="x", color="#e4e3df", linewidth=0.8, zorder=0)
ax.set_axisbelow(True)
for s in ("top", "right", "left"):
    ax.spines[s].set_visible(False)
ax.spines["bottom"].set_color("#c3c2b7")
fig.text(453 / 2200, 1 - 33 / 2160, f"Proof method by mechanic family ({len(ORDER)} families, {n} levels)",
         fontsize=14, fontweight="bold", va="top", ha="left")
fig.legend([Patch(color=c) for _, c, _ in CLASSES], [n_ for n_, _, _ in CLASSES],
           loc="upper left", bbox_to_anchor=(450 / 2200, 390 / 2160),
           fontsize=9, frameon=False, handlelength=1.6, handleheight=0.6,
           borderaxespad=0, ncol=1, labelcolor="#333")
fig.savefig(out, dpi=200, facecolor=BG)
