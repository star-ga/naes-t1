"""fig9_proof_pipeline.png: how a proven floor is certified.
usage: python3 fig9_proof_pipeline.py [out.png]"""
import sys
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import FancyBboxPatch, Polygon, Rectangle

out = sys.argv[1] if len(sys.argv) > 1 else "../fig9_proof_pipeline.png"

W, H = 2365, 687
BG, FILL = "#fcfcfb", "#f9f9f7"
BLUE, EDGE = "#2a78d6", "#e1e0d9"
INK, BODY, ARROW = "#0b0b0b", "#52514e", "#898781"
px = lambda p: p * 72 / 100.0

# Six classes, in the order used for this release.
METHODS = [
    "exhaustive search,",
    "exact math,",
    "matching bounds,",
    "A*, bounded SAT,",
    "empirical",
]

fig = plt.figure(figsize=(W / 100, H / 100), dpi=100, facecolor=BG)
ax = fig.add_axes([0, 0, 1, 1])
ax.set_xlim(0, W)
ax.set_ylim(H, 0)
ax.axis("off")

ax.text(42, 47,
        "How a proven floor is certified (anything less is labelled UPPER BOUND, never PROVEN)",
        fontsize=px(36), fontweight="bold", color=INK, ha="left", va="baseline",
        family="DejaVu Sans")

# Outer box edges measured on the previous figure. The stroke is centred
# on the path, so the path sits 2 px inside that outer edge.
BOXES = [
    (38, BLUE,
     ["Real game engine", "as the oracle"],
     ["restore and step the", "shipped dynamics;", "never re-implemented"]),
    (503, EDGE,
     ["Complete", "state key"],
     ["full game state; a field", "is dropped only if", "bisimulation proves", "it irrelevant"]),
    (968, EDGE,
     ["Sound action", "alphabet"],
     ["clicks grouped by", "object: equal effect,", "checked"]),
    (1433, EDGE,
     ["Exact search or", "exact math"],
     METHODS),
    (1898, BLUE,
     ["Certificate"],
     ["floor + optimal", "sequence; replay-", "verified; independent", "re-derivation"]),
]
BOX_W, BOX_H, BOX_Y = 393, 496, 96
for x, edge, title, body in BOXES:
    ax.add_patch(FancyBboxPatch(
        (x, BOX_Y), BOX_W, BOX_H,
        boxstyle="round,pad=0,rounding_size=18",
        fc=FILL, ec=edge, lw=px(4)))
    cx = x + BOX_W / 2
    ax.text(cx, 204, "\n".join(title), fontsize=px(29), fontweight="bold", color=INK,
            ha="center", va="center", family="DejaVu Sans", linespacing=1.15,
            multialignment="center")
    ax.text(cx, 442, "\n".join(body), fontsize=px(25), color=BODY,
            ha="center", va="center", family="DejaVu Sans", linespacing=1.16,
            multialignment="center")

for x0 in (439, 904, 1369, 1834):
    ax.add_patch(Rectangle((x0, 342), 19, 3, fc=ARROW, ec="none"))
    ax.add_patch(Polygon(
        [(x0 + 19, 336), (x0 + 33, 343), (x0 + 19, 351)],
        closed=True, fc=ARROW, ec="none"))

fig.savefig(out, dpi=100, facecolor=BG)
print(out)
