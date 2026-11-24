"""fig6: human baseline vs proven minimum per level (log-log scatter).
usage: python3 fig6_floor_vs_human_scatter.py [levels_csv] [out_png]"""
import csv, os, sys, statistics as st
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
csv_path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, "data", "demo25_levels_mmae.csv")
out = sys.argv[2] if len(sys.argv) > 2 else os.path.join(ROOT, "figures", "fig6_floor_vs_human_scatter.png")

BG, INK, MUTED, GRID, SPINE = "#fcfcfb", "#52514e", "#898781", "#e1e0d9", "#c3c2b7"
CLASSES = [  # (label, colour)
    ("Exhaustive search (uniform cost / breadth-first)", "#2a78d6"),
    ("Exact math (algebra, closed form, enumeration)", "#eb6834"),
    ("Matching bounds (lower bound = verified win)", "#1baf7a"),
    ("A* with an admissible heuristic", "#eda100"),
    ("Bounded SAT (transition unrolled to one action below the floor; unsatisfiable)", "#6b4c9a"),
    ("Empirical proof (winning route replayed on the engine; mathematical lower bound below it)", "#898781"),
]
PREFIX = [c[0].split(" (")[0].split(" =")[0] for c in CLASSES]

rows = list(csv.DictReader(open(csv_path)))
def cls(r):
    m = r["proof_method"]
    for i, (lab, _) in enumerate(CLASSES):
        if m.startswith(lab.split(" (")[0]):
            return i
    raise SystemExit("unknown proof class: " + m)
ratios = [int(r["human_baseline"]) / int(r["proven_floor"]) for r in rows]
assert all(x >= 1 for x in ratios)

plt.rcParams["font.family"] = "DejaVu Sans"
fig, ax = plt.subplots(figsize=(8.6, 8), dpi=200, facecolor=BG)
fig.subplots_adjust(left=135.5 / 1720, right=1687 / 1720, top=1 - 95 / 1600, bottom=1 - 1484.5 / 1600)
ax.set_facecolor(BG)
LIM = (2, 870)
ax.plot(LIM, LIM, ls="--", lw=1.0, color=SPINE, zorder=1)
ax.text(0.0, 0.0, "", transform=ax.transAxes)
for i, (lab, col) in enumerate(CLASSES):
    pts = [(int(r["proven_floor"]), int(r["human_baseline"])) for r in rows if cls(r) == i]
    ax.scatter([p[0] for p in pts], [p[1] for p in pts], s=44, color=col,
               edgecolor=BG, linewidth=1.0, zorder=3)
ax.set_xscale("log"); ax.set_yscale("log")
ax.set_xlim(*LIM); ax.set_ylim(*LIM)
ax.grid(True, color=GRID, lw=1.0, zorder=0)
ax.set_axisbelow(True)
for s in ("top", "right"):
    ax.spines[s].set_visible(False)
for s in ("left", "bottom"):
    ax.spines[s].set_color(SPINE); ax.spines[s].set_linewidth(0.9)
ax.tick_params(colors=MUTED, labelsize=10, length=0)
ax.tick_params(which="minor", length=0, labelleft=False, labelbottom=False)
ax.set_xlabel("proven minimum actions (floor), log scale", color=INK, fontsize=11)
ax.set_ylabel("human baseline actions, log scale", color=INK, fontsize=11)
fig.text(140 / 1720, 1 - 33 / 1600, "Every demo level: human baseline vs proven minimum",
         fontsize=14, fontweight="bold", color="#0b0b0b", va="top")
ax.text(0.015, 0.985, f"{len(rows)} levels; every point is on or above the line.\n"
        f"Median human / minimum = {st.median(ratios):.2f}x; max = {max(ratios):.1f}x.",
        transform=ax.transAxes, fontsize=10, color=INK, va="top", linespacing=1.15)
ax.text(0.870, 0.88, "human = minimum", rotation=45, color=MUTED, fontsize=9,
        transform=ax.transAxes, ha="center", va="center", rotation_mode="anchor")
h = [Line2D([], [], marker="o", ls="", color=c, markersize=5, label=l) for l, c in CLASSES]
ax.legend(handles=h, loc="lower right", frameon=False, fontsize=9, labelcolor="#0b0b0b",
          bbox_to_anchor=(1.0, -0.005), handletextpad=0.6)
fig.savefig(out, dpi=200, facecolor=BG)
