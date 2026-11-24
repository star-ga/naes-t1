# ENGINE lens (p2 self-refutation): {v3,v2} pre-states near the zones; for EVERY click hitting at least one piece, the
# real engine is stepped manually and the tick at which the win is detected is recorded; compared with the prediction
# of the own model used by indep/indep_dp.py (trajectory model + win test after ticks 1..3, merge/collision then win at tick 4).
import sys, random, time
import os; sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))
from eng import *
from arcengine import ActionInput
SZ = [1, 2, 3, 4]; ZONES = [(5, 46, 9, 9), (19, 46, 9, 9)]; R = 8; STEP = 4
def inz(v, x, y): cx, cy = x + SZ[v] // 2, y + SZ[v] // 2; return any(zx <= cx < zx + w and zy <= cy < zy + h for zx, zy, w, h in ZONES)
def hit(P, v, x, y):
    s = SZ[v]; nx = min(max(P[0], x), x + s - 1); ny = min(max(P[1], y), y + s - 1); return (P[0] - nx) ** 2 + (P[1] - ny) ** 2 <= R * R
def traj(v, x, y, P):
    out = []; s = SZ[v]
    for t in range(4):
        cx, cy = x + s // 2, y + s // 2; x += max(-STEP, min(STEP, P[0] - cx)); y += max(-STEP, min(STEP, P[1] - cy))
        x = max(0, min(x, 64 - s)); y = max(10, min(y, 63, 64 - s)); out.append((x, y))
    return out
def predict(st, P):
    (v2, x2, y2), (v3, x3, y3) = sorted(st)
    h2, h3 = hit(P, 2, x2, y2), hit(P, 3, x3, y3)
    if not (h2 or h3) or not (10 <= P[1] < 63): return 'noop'
    t2 = traj(2, x2, y2, P) if h2 else [(x2, y2)] * 4; t3 = traj(3, x3, y3, P) if h3 else [(x3, y3)] * 4
    for t in range(4):
        if t == 3 and h2 and h3: return 'nowin'   # tick 4: collision (overlap, different values) before win test
        if inz(2, *t2[t]) and inz(3, *t3[t]): return t + 1
    return 'nowin'
env = start_env(); g0 = env._game
def engine(st, P):
    c = lean_copy(g0); set_state(c, st); reset_budget(c)
    c._full_reset = False; c._set_action(ActionInput(id=GameAction.ACTION6, data={"x": P[0], "y": P[1]}))
    k = 0
    while not c.is_action_complete():
        k += 1; assert k < 200
        if c._next_level: return ('win_after_switch', k)
        c.step()
        if c.qygchysnh == "pkqcyfpgc": return k - 1   # step 1 registers the click; step k>=2 is tick k-1
    return 'nowin'
rng = random.Random(int(sys.argv[1])); NS = int(sys.argv[2])
cnt = {}; mism = 0; n = 0; t0 = time.time(); states = 0
while states < NS:
    # pre-state: v3 and v2 with centres within ~10 px of a zone (often inside), pieces not overlapping required? no: any valid
    z3 = rng.choice(ZONES); z2 = rng.choice(ZONES)
    x3 = min(60, max(0, z3[0] + 2 + rng.randint(-10, 10))); y3 = min(60, max(10, z3[1] + 2 + rng.randint(-10, 8)))
    x2 = min(61, max(0, z2[0] + 3 + rng.randint(-10, 10))); y2 = min(61, max(10, z2[1] + 3 + rng.randint(-10, 8)))
    st = ((2, x2, y2), (3, x3, y3))
    if inz(2, x2, y2) and inz(3, x3, y3): continue   # already winning pre-state cannot occur on a route
    states += 1
    for px in range(64):
        for py in range(64):
            P = (px, py); p = predict(st, P)
            if p == 'noop': continue
            e = engine(st, P); n += 1
            key = (p, e); cnt[key] = cnt.get(key, 0) + 1
            if p != e: mism += 1; print("MISMATCH", st, P, "model", p, "engine", e, flush=True)
print("TICKWIN states", states, "clicks", n, "mismatches", mism, "sec", round(time.time() - t0))
for k in sorted(cnt, key=str): print("TICKWIN outcome model/engine", k, cnt[k])
