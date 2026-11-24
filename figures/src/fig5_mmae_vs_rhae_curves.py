"""fig5: MMAE and RHAE on one level (m0r0 level 5).
usage: python3 fig5_mmae_vs_rhae_curves.py [levels_csv] [out_png]"""
import csv, os, sys
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
csv_path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, "data", "demo25_levels_mmae.csv")
out = sys.argv[2] if len(sys.argv) > 2 else os.path.join(ROOT, "figures", "fig5_mmae_vs_rhae_curves.png")

BG, INK, MUTED, GRID, SPINE = "#fcfcfb", "#52514e", "#898781", "#e1e0d9", "#c3c2b7"
ORANGE, BLUE, SHADE = "#eb6834", "#2a78d6", "#f1f0eb"
TITLE = "#0b0b0b"

row = next(r for r in csv.DictReader(open(csv_path)) if r["game"] == "m0r0" and r["level"] == "5")
floor = int(row["proven_floor"])
human = int(row["human_baseline"])
family = row["mechanic_family"]
assert floor == 39 and human == 500

# The callout is the action count drawn on the existing figure (MMAE prints as 0.025).
mark = 246
assert round((floor / mark) ** 2, 3) == 0.025

xs = np.linspace(floor, 900, 2000)
mmae = (floor / xs) ** 2
rhae = np.minimum(1.0, (human / xs) ** 2)

plt.rcParams["font.family"] = "DejaVu Sans"
fig, ax = plt.subplots(figsize=(10.5, 5.6), dpi=200, facecolor=BG)
ax.set_facecolor(BG)
ax.axvspan(floor, human, color=SHADE, zorder=0)
ax.axvline(floor, color=SPINE, ls=":", lw=1.0, zorder=1)
ax.axvline(human, color=SPINE, ls=":", lw=1.0, zorder=1)
ax.plot(xs, rhae, color=ORANGE, lw=2.2, zorder=3, label="RHAE (human baseline, Kaggle cap 1.0)")
ax.plot(xs, mmae, color=BLUE, lw=2.2, zorder=3, label="MMAE (proven minimum)")
ax.plot([mark, mark], [0.0, 0.42], color=MUTED, lw=1.0, zorder=2)

ax.set_xlim(0, 920)
ax.set_ylim(0, 1.08)
ax.set_xlabel("agent actions on the level", color=INK, fontsize=12)
ax.set_ylabel("level score", color=INK, fontsize=12)
ax.grid(True, color=GRID, lw=1.0, zorder=0)
ax.set_axisbelow(True)
for s in ("top", "right"):
    ax.spines[s].set_visible(False)
for s in ("left", "bottom"):
    ax.spines[s].set_color(SPINE)
    ax.spines[s].set_linewidth(0.9)
ax.tick_params(colors=MUTED, labelsize=11, length=0)
ax.legend(frameon=False, fontsize=11, labelcolor=TITLE, loc="center right")

ax.text(floor + 8, 1.02, f"proven minimum: {floor} actions", color=INK, fontsize=11, va="bottom")
ax.text(human + 8, 1.02, f"human baseline: {human} actions", color=INK, fontsize=11, va="bottom")
ax.text((floor + human) / 2, 0.84, "shaded: as good as the human baseline,\nbut far from the minimum",
        color=INK, fontsize=11, ha="center", va="center")
ax.text(mark + 8, 0.55, f"at {mark} actions:\nRHAE = 1.000\nMMAE = 0.025",
        color=TITLE, fontsize=11, va="center", fontweight="medium")

fig.suptitle(f"Same level, two yardsticks ({row['game']} level {row['level']}, {family})",
             fontsize=15, fontweight="bold", color=TITLE, x=0.02, ha="left")
fig.tight_layout(rect=(0, 0, 1, 0.94))
fig.savefig(out, dpi=200, facecolor=BG)
