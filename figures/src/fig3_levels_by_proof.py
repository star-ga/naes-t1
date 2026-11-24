"""fig3_levels_by_proof.png: all 183 demo levels, proven minimum per level, by proof method.
usage: python3 fig3_levels_by_proof.py [levels.csv] [out.png]"""
import csv, sys
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import FancyBboxPatch

csv_path = sys.argv[1] if len(sys.argv) > 1 else "../../data/demo25_levels_mmae.csv"
out = sys.argv[2] if len(sys.argv) > 2 else "../fig3_levels_by_proof.png"
ORDER = ["s5i5", "ka59", "ft09", "tr87", "vc33", "m0r0", "tu93", "sp80", "su15", "ls20", "dc22",
         "bp35", "lf52", "re86", "r11l", "ar25", "tn36", "g50t", "cn04", "sb26", "wa30", "sk48",
         "sc25", "cd82", "lp85"]
CLASSES = [  # (proof_method, colour, text colour, legend slot (col,row))
    ("Exhaustive search (uniform cost / breadth-first)", "#2a78d6", "white", (0, 0)),
    ("Exact math (algebra, closed form, enumeration)", "#eb6834", "white", (0, 1)),
    ("Matching bounds (lower bound = verified win)", "#1baf7a", "white", (0, 2)),
    ("A* with an admissible heuristic", "#eda100", "black", (1, 0)),
    ("Bounded SAT (transition unrolled to one action below the floor; unsatisfiable)", "#6b4c9a", "white", (1, 1)),
    ("Empirical proof (winning route replayed on the engine; mathematical lower bound below it)", "#898781", "white", (1, 2)),
]
INK, GREY, LIGHT = "#0b0b0b", "#52514e", "#898781"
rows = list(csv.DictReader(open(csv_path)))
lv, fam, cnt = {}, {}, {c[0]: 0 for c in CLASSES}
for r in rows:
    lv[(r["game"], int(r["level"]))] = (r["proven_floor"], r["proof_method"])
    fam[r["game"]] = r["mechanic_family"]
    cnt[r["proof_method"]] += 1
assert len(rows) == 183 and len(ORDER) == 25
style = {c[0]: c for c in CLASSES}

fig = plt.figure(figsize=(25, 22), dpi=100, facecolor="#fcfcfb")
ax = fig.add_axes([0, 0, 1, 1]); ax.set_xlim(0, 2500); ax.set_ylim(2200, 0); ax.axis("off")
ax.text(572, 63, "All 183 demo levels: proven minimum actions per level, by proof method",
        fontsize=28, fontweight="bold", color=INK, ha="left", va="baseline")
ax.text(572, 112.5, "Number = floor (grey: wa30 L8 and wa30 L9, empirical proof).",
        fontsize=21, color=GREY, ha="left", va="baseline")
ax.text(572, 145, "NAES-T1 plays exactly that many actions on 183 of 183 levels.",
        fontsize=21, color=GREY, ha="left", va="baseline")
X0, DX, W, Y0, DY, H = 601, 185.6, 166, 216, 71.8, 56
for c in range(10):
    ax.text(X0 + DX * c + W / 2, 192, f"L{c + 1}", fontsize=22, color=LIGHT, ha="center", va="baseline")
for r, g in enumerate(ORDER):
    y = Y0 + DY * r
    ax.text(562, y + H / 2, f"{g}  ·  {fam[g]}", fontsize=21.8, color=GREY, ha="right", va="center_baseline")
    for l in range(1, 11):
        if (g, l) not in lv:
            continue
        val, pm = lv[(g, l)]
        _, col, tcol, _ = style[pm]
        x = X0 + DX * (l - 1)
        ax.add_patch(FancyBboxPatch((x, y), W, H, boxstyle="round,pad=0,rounding_size=8",
                                    fc=col, ec="none"))
        ax.text(x + W / 2, y + H / 2 - 4, val, fontsize=20, fontweight="bold", color=tcol,
                ha="center", va="center_baseline")
for pm, col, _, (c, r) in CLASSES:
    label = pm if pm.startswith(("Exhaustive", "Exact", "Matching", "Empirical")) else pm
    n = cnt[pm]
    text = f"{label}: {n} level{'s' if n != 1 else ''}"
    if pm.startswith("Exhaustive") or pm.startswith("Exact") or pm.startswith("Matching") or pm.startswith("A*") or pm.startswith("Empirical"):
        pass
    sx = 457 if c == 0 else 1411
    cy = 2056 + 41.5 * r
    ax.add_patch(FancyBboxPatch((sx, cy - 9.5), 55, 19, boxstyle="round,pad=0,rounding_size=4",
                                fc=col, ec="none"))
    ax.text(sx + 78, cy, text, fontsize=14, color=INK, ha="left", va="center_baseline")
fig.savefig(out, dpi=100, facecolor=fig.get_facecolor())
print(out, cnt)
