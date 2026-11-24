"""fig1_headline.png: five headline tiles. Usage: fig1_headline.py [levels.csv] [out.png]"""
import sys
import pandas as pd
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import FancyBboxPatch

csv = sys.argv[1] if len(sys.argv) > 1 else "../../data/demo25_levels_mmae.csv"
out = sys.argv[2] if len(sys.argv) > 2 else "../fig1_headline.png"
d = pd.read_csv(csv)
W, H = 3000, 520
BG, TILE, EDGE, INK, MUTED = "#fcfcfb", "#f9f9f7", "#e1e0d9", "#0b0b0b", "#52514e"
BIG, SMALL = 83.5, 32  # px
tiles = [
    ("25 / 25", ["games won"]),
    ("183 / 183", ["levels won"]),
    ("1.149", ["RHAE index", "(1.00 = 100%, max 1.15)"]),
    ("1.000", ["MMAE, demo 25", "(1.00 = 100%, max 1.00)"]),
    (f"{int(d.our_actions.sum()):,}",
     ["actions in total", f"(human baselines: {int(d.human_baseline.sum()):,})"]),
]
fig = plt.figure(figsize=(W / 100, H / 100), dpi=100, facecolor=BG)
ax = fig.add_axes([0, 0, 1, 1]); ax.set_xlim(0, W); ax.set_ylim(H, 0); ax.axis("off")
px = lambda p: p * 72 / 100  # pixel -> points
xs = [24, 620, 1215, 1811, 2406]
for x0, (big, lines) in zip(xs, tiles):
    ax.add_patch(FancyBboxPatch((x0 + 1, 32), 563 - 2, 457 - 2,
        boxstyle="round,pad=0,rounding_size=22", fc=TILE, ec=EDGE, lw=px(2)))
    ax.text(x0 + 40, 229, big, fontsize=px(BIG), fontweight="bold", color=INK, va="baseline", ha="left", family="DejaVu Sans")
    base = [369, 405] if len(lines) == 2 else [387]
    for t, b in zip(lines, base):
        ax.text(x0 + 40, b, t, fontsize=px(SMALL), color=MUTED, va="baseline", ha="left", family="DejaVu Sans")
fig.savefig(out, dpi=100, facecolor=BG)
