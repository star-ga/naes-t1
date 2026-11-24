# p2 INDEPENDENT re-derivation of the lineage lower bound h(start), written from the engine source (su15.py), NOT from simp2.h/dpb.c.
# * constants (radius, step, ticks, rows, sizes, zones, goal) are read from the live engine at level 3
# * single-piece landing FP[v][P] and pair-merge landing GP[v][P] are obtained by CALLING THE REAL ENGINE (set_state + ACTION6)
# * mid-flight positions: own numpy model of pkrdtzfrth, checked tick-by-tick against the real engine on a sample (TRAJCHECK)
# * joint final click: decomposed per (P, tick) instead of the W table: a {v3,v2} pre-state wins at tick t<4 iff both
#   pieces that are hit have centre in a zone at tick t and every non-hit piece already has centre in a zone.
import sys, time, random, itertools
import numpy as np
import os; sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))
import eng
from arcengine.enums import GameAction
env = eng.start_env(); g = env._game; gl = type(g).axaxyjxqoe.__globals__
R = g.kacsjmxae; STEP = g.ikskfqldi; TICKS = g.gdamdvokm; YMIN = gl['gvvyzrusqq']; YMAX = gl['qsqeqpepjy']; WD = g.ojzsjmpic; HT = g.fchredntr
SZ = [gl['sprites'][gl['pqxlzdqwbo'][v]].pixels.shape[0] for v in range(4)]
ZONES = [(z.x, z.y, z.pixels.shape[1], z.pixels.shape[0]) for z in g.powykypsm]
GOAL = {int(a): int(b) for a, b in g.dsqlbvwaj}
START = eng.est(g)
print("CONSTS R", R, "STEP", STEP, "TICKS", TICKS, "YMIN", YMIN, "YMAX", YMAX, "W", WD, "H", HT, "SZ", SZ, "ZONES", ZONES, "GOAL", GOAL, "START", START, flush=True)
assert GOAL == {3: 1, 2: 1}
INF = 99
def valid(v, x, y): s = SZ[v]; return 0 <= x <= WD - s and YMIN <= y <= min(YMAX, HT - s)
def inrange(P, v, x, y):  # engine kcqeohsztd
    px, py = P; s = SZ[v]
    nx = x if px < x else (x + s - 1 if px > x + s - 1 else px)
    ny = y if py < y else (y + s - 1 if py > y + s - 1 else py)
    return (px - nx) ** 2 + (py - ny) ** 2 <= R * R
def inzone_c(cx, cy): return any(zx <= cx < zx + zw and zy <= cy < zy + zh for zx, zy, zw, zh in ZONES)
CLICKS = [(px, py) for px in range(64) for py in range(64) if YMIN <= py < YMAX]  # rows outside do nothing (axaxyjxqoe)
VALID = {v: np.array([[valid(v, x, y) for y in range(64)] for x in range(64)]) for v in range(4)}
# offsets (dx,dy)=(P-x,P-y) for which a piece at (x,y) is hit by P
OFF = {v: [(dx, dy) for dx in range(-20, 21) for dy in range(-20, 21) if inrange((dx, dy), v, 0, 0)] for v in range(4)}
def hitmask(P, v):
    m = np.zeros((64, 64), bool)
    for dx, dy in OFF[v]:
        x, y = P[0] - dx, P[1] - dy
        if 0 <= x < 64 and 0 <= y < 64 and VALID[v][x, y]: m[x, y] = True
    return m
# ---------- engine-derived landing tables ----------
def eng_click(pieces, P):
    eng.reset_budget(g); eng.set_state(g, pieces); before = eng.est(g)
    lv = g.level_index; eng.fast_action(g, GameAction.ACTION6, P[0], P[1])
    won = (g.level_index != lv) or (g._state.name == 'WIN')
    after = eng.est(g)
    if won:  # restart env to come back to level 3
        rebuild()
    return won, before, after
def rebuild():
    global env, g
    env = eng.start_env(); g = env._game
t0 = time.time()
FP = {v: {} for v in range(4)}; GP = {v: {} for v in range(3)}; calls = 0
for v in range(4):
    for P in CLICKS:
        hm = hitmask(P, v); idx = np.argwhere(hm)
        if len(idx) == 0: continue
        x0, y0 = map(int, idx[0])
        won, b, a = eng_click([(v, x0, y0)], P); calls += 1
        assert not won and len(a) == 1 and a[0][0] == v, (v, P, a)
        FP[v][P] = (a[0][1], a[0][2])
        if v < 3:
            won, b, a = eng_click([(v, x0, y0), (v, x0, y0)], P); calls += 1
            assert not won and len(a) == 1 and a[0][0] == v + 1, (v, P, a)
            GP[v][P] = (a[0][1], a[0][2])
    print("ENGINE_TABLES v", v, "moves", len(FP[v]), "merges", len(GP.get(v, {})), "engine_calls", calls, "sec", round(time.time() - t0), flush=True)
import json, os
if os.environ.get('DUMP_TABLES'):
    with open(os.environ['DUMP_TABLES'], 'w') as f:
        for v in range(4):
            for P in CLICKS:
                if P in FP[v]: f.write(f"F {v} {P[0]} {P[1]} {FP[v][P][0]} {FP[v][P][1]}\n")
                if v < 3 and P in GP[v]: f.write(f"G {v} {P[0]} {P[1]} {GP[v][P][0]} {GP[v][P][1]}\n")
# ---------- own trajectory model, checked against engine tick by tick ----------
def traj(v, x, y, P):
    out = []; s = SZ[v]
    for t in range(TICKS):
        cx, cy = x + s // 2, y + s // 2; dx, dy = P[0] - cx, P[1] - cy
        x += max(-STEP, min(STEP, dx)); y += max(-STEP, min(STEP, dy))
        x = max(0, min(x, WD - s)); y = max(YMIN, min(y, YMAX, HT - s)); out.append((x, y))
    return out
def eng_traj(v, x, y, P):
    from arcengine import ActionInput
    eng.reset_budget(g); eng.set_state(g, [(v, x, y)])
    ai = ActionInput(id=GameAction.ACTION6, data={"x": P[0], "y": P[1]}); g._full_reset = False; g._set_action(ai)
    pos = []; n = 0
    while not g.is_action_complete():
        n += 1; assert n < 100
        g.step()
        sp = g.lkujttxgs[0]; pos.append((int(sp.x), int(sp.y)))
    return pos
rng = random.Random(5); tc = 0; tbad = 0
for it in range(1500):
    v = rng.randrange(4); P = rng.choice(CLICKS); idx = np.argwhere(hitmask(P, v))
    if len(idx) == 0: continue
    x, y = map(int, idx[rng.randrange(len(idx))]); e = eng_traj(v, x, y, P); m = traj(v, x, y, P); tc += 1
    # engine: 1st step() only registers the click (axaxyjxqoe, no movement); steps 2..5 are ticks 1..4
    if e[0] != (x, y) or e[1:1 + TICKS] != m or m[-1] != FP[v][P]: tbad += 1; print("TRAJ_MISMATCH", v, x, y, P, e, m)
print("TRAJCHECK samples", tc, "mismatches", tbad, flush=True)
# ---------- lineage DP ----------
def shiftmin(D, v):  # A[P] = min over pieces positions p hit by P of D[p]
    A = np.full((64, 64), INF, np.int32); Dp = np.full((64 + 48, 64 + 48), INF, np.int32); Dp[24:88, 24:88] = D
    for dx, dy in OFF[v]:
        # A[Px,Py] = min(A, D[Px-dx, Py-dy])
        A = np.minimum(A, Dp[24 - dx:88 - dx, 24 - dy:88 - dy])
    rowok = np.zeros((64, 64), bool); rowok[:, YMIN:YMAX] = True
    A[~rowok] = INF
    return A
FPa = {v: np.array([FP[v][P] for P in CLICKS if P in FP[v]]) for v in range(4)}
FPsrc = {v: np.array([P for P in CLICKS if P in FP[v]]) for v in range(4)}
GPa = {v: np.array([GP[v][P] for P in CLICKS if P in GP[v]]) for v in range(3)}
GPsrc = {v: np.array([P for P in CLICKS if P in GP[v]]) for v in range(3)}
def closure(D, v):
    while True:
        A = shiftmin(D, v); src = FPsrc[v]; dst = FPa[v]
        cand = A[src[:, 0], src[:, 1]] + 1
        D2 = D.copy(); np.minimum.at(D2, (dst[:, 0], dst[:, 1]), cand)
        D2 = np.minimum(D2, INF)
        if (D2 == D).all(): return D
        D = D2
pieces = list(START); n = len(pieces)
mass = lambda m: sum(1 << pieces[i][0] for i in range(n) if m >> i & 1)
val = {}
for m in range(1, 1 << n):
    ms = mass(m)
    if ms & (ms - 1) == 0 and ms <= 8: val[m] = ms.bit_length() - 1
D = {}
for m in sorted(val, key=lambda m: bin(m).count('1')):
    v = val[m]; Dm = np.full((64, 64), INF, np.int32)
    if bin(m).count('1') == 1:
        i = m.bit_length() - 1; Dm[pieces[i][1], pieces[i][2]] = 0
    else:
        if v == 0: continue
        sub = (m - 1) & m
        while sub:
            t1, t2 = sub, m ^ sub
            if t1 > t2 and val.get(t1) == v - 1 and val.get(t2) == v - 1 and t1 in D and t2 in D:
                A1 = shiftmin(D[t1], v - 1); A2 = shiftmin(D[t2], v - 1); src = GPsrc[v - 1]; dst = GPa[v - 1]
                cand = A1[src[:, 0], src[:, 1]] + A2[src[:, 0], src[:, 1]] + 1
                np.minimum.at(Dm, (dst[:, 0], dst[:, 1]), cand)
            sub = (sub - 1) & m
        Dm = np.minimum(Dm, INF)
    D[m] = closure(Dm, v)
print("DP subsets", len(D), "sec", round(time.time() - t0), flush=True)
# zone masks per value (centre in zone)
ZM = {v: np.array([[VALID[v][x, y] and inzone_c(x + SZ[v] // 2, y + SZ[v] // 2) for y in range(64)] for x in range(64)]) for v in range(4)}
# mid-flight zone masks per (v,P,t): start positions a hit by P whose position after tick t has centre in a zone
def midmasks(v, P, hm):
    ms = [np.zeros((64, 64), bool) for t in range(TICKS)]
    for x, y in np.argwhere(hm):
        for t, (tx, ty) in enumerate(traj(v, int(x), int(y), P)):
            if inzone_c(tx + SZ[v] // 2, ty + SZ[v] // 2): ms[t][x, y] = True
    return ms
full = (1 << n) - 1; best = (INF, None)
# precompute per P: masks
pre = []
for P in CLICKS:
    h3 = hitmask(P, 3); h2 = hitmask(P, 2)
    if not h3.any() and not h2.any(): continue
    mids3 = midmasks(3, P, h3); mids2 = midmasks(2, P, h2)
    pre.append((P, h3, h2, mids3, mids2))
print("PRE clicks", len(pre), "sec", round(time.time() - t0), flush=True)
res_rows = []
for T3 in D:
    if val[T3] != 3: continue
    T2 = full ^ T3
    if T2 not in D or val.get(T2) != 2: continue
    D3, D2 = D[T3], D[T2]
    normal = int(D3[ZM[3]].min()) + int(D2[ZM[2]].min())
    joint = INF
    for P, h3, h2, mids3, mids2 in pre:
        for t in range(TICKS - 1):  # ticks 1..3: win test after movement, before merge/collision
            a = D3[mids3[t]]; b = D2[mids2[t]]
            if a.size and b.size: joint = min(joint, int(a.min()) + int(b.min()) + 1)   # both hit
            if a.size:  # only v3 hit, v2 standing in a zone and not hit
                bs = D2[ZM[2] & ~h2]
                if bs.size: joint = min(joint, int(a.min()) + int(bs.min()) + 1)
            if b.size:
                as_ = D3[ZM[3] & ~h3]
                if as_.size: joint = min(joint, int(as_.min()) + int(b.min()) + 1)
        # tick 4 (one piece hit, lands in zone, other standing in zone): already inside 'normal' (move counted in D)
    tot = min(normal, joint); res_rows.append((tot, normal, joint, T3, T2))
    if tot < best[0]: best = (tot, (normal, joint, T3, T2))
res_rows.sort()
print("PARTITIONS", len(res_rows), "best5", res_rows[:5])
nm = min(r[1] for r in res_rows); jm = min(r[2] for r in res_rows)
print(f"INDEP_ROOT_BOUND {best[0]} normal={nm} joint={jm} T3={best[1][2]:03x} T2={best[1][3]:03x} sec={round(time.time()-t0)}")
