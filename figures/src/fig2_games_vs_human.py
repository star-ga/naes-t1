"""fig2_games_vs_human.png: per-game actions, our play vs human baseline.
usage: python3 fig2_games_vs_human.py [levels.csv] [out.png]"""
import sys
import pandas as pd
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

csv = sys.argv[1] if len(sys.argv) > 1 else "../../data/demo25_levels_mmae.csv"
out = sys.argv[2] if len(sys.argv) > 2 else "../fig2_games_vs_human.png"
BG, GRID, LINK, TXT, TITLE = "#fcfcfb", "#e1e0d9", "#e6e6e0", "#52514e", "#0b0b0b"
BLUE, ORANGE = "#2a78d6", "#eb6834"

df = pd.read_csv(csv)
g = df.groupby("game")[["our_actions", "human_baseline"]].sum()
g["ratio"] = g.human_baseline / g.our_actions
g = g.sort_values("ratio", ascending=False)
n = len(g)

fig, ax = plt.subplots(figsize=(11, 9), dpi=200, facecolor=BG)
fig.subplots_adjust(left=125 / 2200, right=2165 / 2200, top=1 - 103 / 1800, bottom=1 - 1687 / 1800)
ax.set_facecolor(BG)
for i, (name, r) in enumerate(g.iterrows()):
    ax.plot([r.our_actions, r.human_baseline], [i, i], color=LINK, lw=2.4, zorder=1, solid_capstyle="round")
    ax.text(r.human_baseline + 28, i, f"{r.ratio:.1f}x", va="center", fontsize=10, color=TXT)
ax.scatter(g.our_actions, range(n), s=36, color=BLUE, zorder=3, label="NAES-T1 (at the floor on 183 of 183 levels)")
ax.scatter(g.human_baseline, range(n), s=36, color=ORANGE, zorder=3, label="human baseline")
ax.set_yticks(range(n))
ax.set_yticklabels(g.index, fontsize=11.5, color=TXT)
ax.set_ylim(n - 1 + 1.15, -1.15)
ax.set_xlim(0, 2063)
ax.xaxis.grid(True, color=GRID, lw=0.8)
ax.set_axisbelow(True)
ax.tick_params(axis="x", labelsize=11.5, colors="#89877f", length=0, pad=3)
ax.tick_params(axis="y", length=0, pad=3)
for s in ax.spines.values():
    s.set_visible(False)
ax.set_xlabel("actions to win all levels of the game", fontsize=11, color=TXT, labelpad=4)
ax.set_title("Per game: our actions vs the human baseline", loc="left", fontsize=14, fontweight="bold", color=TITLE, pad=14)
ax.legend(loc="lower right", fontsize=11, frameon=False, borderaxespad=0.4, markerscale=1.0, handletextpad=0.6)
fig.savefig(out, dpi=200, facecolor=BG)
