"""fig7_human_over_minimum_hist.png: human baseline over the minimum, per level.
usage: python3 fig7_human_over_minimum_hist.py [levels.csv] [out.png]"""
import csv, sys, statistics
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
src = sys.argv[1] if len(sys.argv) > 1 else "../../data/demo25_levels_mmae.csv"
out = sys.argv[2] if len(sys.argv) > 2 else "../fig7_human_over_minimum_hist.png"
rows = list(csv.DictReader(open(src)))
r = np.array([int(x["human_baseline"]) / int(x["proven_floor"]) for x in rows])
n = len(r); above = int((r > 1).sum()); med = statistics.median(r)
bins = np.arange(1, r.max() + 0.5, 0.5)
fig, ax = plt.subplots(figsize=(10, 4.595), dpi=200)
bg = "#fcfcfb"; fig.patch.set_facecolor(bg); ax.set_facecolor(bg)
cnt, edges, bars = ax.hist(r, bins=bins, color="#2a78d6", edgecolor=bg, linewidth=0.6)
for c, e0, e1 in zip(cnt, edges[:-1], edges[1:]):
    if c: ax.text((e0+e1)/2, c+0.4, str(int(c)), ha="center", va="bottom", fontsize=9, color="#444")
ax.set_xlim(0.6, 32.0); ax.set_ylim(0, 42.5)
ax.set_xlabel("human baseline / proven minimum, per level", color="#444", fontsize=11)
ax.set_ylabel("levels", color="#444", fontsize=11)
for s in ("top", "right"): ax.spines[s].set_visible(False)
for s in ("left", "bottom"): ax.spines[s].set_color("#bbb")
ax.grid(axis="y", color="#e0e0e0", lw=0.8); ax.set_axisbelow(True)
ax.tick_params(axis="y", colors="#555", labelsize=11.5, length=3)
ax.tick_params(axis="x", colors="#888", labelsize=11.5, length=3)
fig.text(0.061, 0.952, "How far human baselines sit above the proven minimum", fontsize=14.1, fontweight="bold", ha="left", va="center")
fig.text(0.061, 0.887, f"{above} of {n} levels: a first-time human used more actions than needed. Median {med:.2f}x.", fontsize=10, color="#555", ha="left", va="center")
fig.subplots_adjust(left=0.061, right=0.982, top=0.859, bottom=0.152)
fig.savefig(out, facecolor=bg)
print(above, n, round(med, 3), list(cnt.astype(int)))
