"""fig10_action_efficiency_per_game.png: per-game level-weighted human/NAES-T1 action ratio.
usage: python3 fig10_action_efficiency_per_game.py [levels.csv] [out.png]"""
import sys
import pandas as pd
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

csv = sys.argv[1] if len(sys.argv) > 1 else "../../data/demo25_levels_mmae.csv"
out = sys.argv[2] if len(sys.argv) > 2 else "../fig10_action_efficiency_per_game.png"
BG, GRID, TXT, TITLE = "#fcfcfb", "#e1e0d9", "#52514e", "#0b0b0b"
BLUE, ORANGE = "#2a78d6", "#eb6834"

df = pd.read_csv(csv)
# level index is the weight, so later levels count more
r = df.groupby("game").apply(
    lambda g: (g.level * g.human_baseline / g.our_actions).sum() / g.level.sum(), include_groups=False
).sort_values(ascending=False)
mean = r.mean()
n = len(r)

W, H = 2200, 1720
fig = plt.figure(figsize=(11, 8.6), dpi=200, facecolor=BG)
ax = fig.add_axes([126 / W, 1 - 1606 / H, (2165 - 126) / W, (1606 - 94) / H])
ax.set_facecolor(BG)
ax.barh(range(n), r.values, height=0.633, color=BLUE, zorder=3)
for i, v in enumerate(r.values):
    ax.text(v + 0.08, i, f"{v:.2f}", va="center", fontsize=9.5, color=TXT)
ax.axvline(1.0, color=ORANGE, lw=1.6, zorder=4)
ax.axvline(mean, color="#7a7974", lw=1.3, ls=(0, (4, 3)), zorder=4)
ax.text(1.0 + 0.05, -1.05, "1.0 = human baseline", fontsize=9.5, color=TXT, va="center")
ax.text(mean + 0.05, -1.05, f"mean over 25 games = {mean:.2f}", fontsize=9.5, color=TXT, va="center")
ax.set_yticks(range(n))
ax.set_yticklabels(r.index, fontsize=11.5, color=TXT)
ax.set_ylim(n - 1 + 0.573, -1.30)
ax.set_xlim(0, 11.09)
ax.set_xticks([0, 2, 4, 6, 8, 10])
ax.xaxis.grid(True, color=GRID, lw=0.8)
ax.set_axisbelow(True)
ax.tick_params(axis="x", labelsize=11.5, colors="#89877f", length=0, pad=3)
ax.tick_params(axis="y", length=0, pad=3)
for k, s in ax.spines.items():
    s.set_visible(k == "bottom")
ax.spines["bottom"].set_color("#c3c2b7")
ax.set_xlabel("action efficiency vs human = level-weighted mean of (human baseline ÷ NAES-T1 actions)", fontsize=11, color=TXT, labelpad=6)
fig.text(125 / W, 1 - 49 / H, "Action efficiency vs human per game: human baseline ÷ NAES-T1 actions", fontsize=13.5, fontweight="bold", color=TITLE, va="center")
fig.savefig(out, dpi=200, facecolor=BG)
