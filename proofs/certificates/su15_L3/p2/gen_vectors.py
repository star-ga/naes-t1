# usage: gen_vectors.py MODE SEED N OUT   MODE in walk|allpos|nearwin|undo
# Each output line: pre-state | cx cy | engine outcome: "W" (level completed) or "S <post-state>"
import sys,random,time,subprocess,ctypes
sys.path.insert(0,'.')
from eng import *
mode,seed,N,out=sys.argv[1],int(sys.argv[2]),int(sys.argv[3]),sys.argv[4]
rng=random.Random(seed)
env=start_env(); g0=env._game; assert est(g0)==L3_START
S0=g0._score; LV0=g0.level_index
Z=[(5,46,9,9),(19,46,9,9)]
f=open(out,"w")
# sim helper (C library, project simulator) used ONLY to pick clicks; correctness is judged against the engine
lib=ctypes.CDLL("./libsim.so"); 
lib.sim_click.argtypes=[ctypes.c_int,ctypes.POINTER(ctypes.c_int),ctypes.c_int,ctypes.c_int,ctypes.POINTER(ctypes.c_int)]
def sim_click(st,cx,cy):
    a=(ctypes.c_int*(3*len(st)))(*[t for p in st for t in p]); o=(ctypes.c_int*30)()
    r=lib.sim_click(len(st),a,cx,cy,o); n=o[0]
    return r,tuple(sorted((o[1+3*i],o[2+3*i],o[3+3*i]) for i in range(n)))
def eclick(g,st,cx,cy):
    reset_budget(g)
    fast_action(g,GameAction.ACTION6,cx,cy)
    won=(g.level_index!=LV0 or g._state==GameState.WIN or g._score>S0)
    return won
def record(st,cx,cy,won,post):
    f.write(fmt(st)+f" {cx} {cy} "+("W" if won else "S "+fmt(post))+"\n")
def smart(st):
    for _ in range(60):
        v,x,y=rng.choice(st); s=(1,2,3,4,5,7,8,9,10)[v]; c=(x+s//2,y+s//2)
        cx=min(63,max(0,c[0]+rng.randint(-10,10))); cy=min(63,max(0,c[1]+rng.randint(-10,10)))
        r,ns=sim_click(st,cx,cy)
        if r==2 and ns!=st: return cx,cy
    return rng.randrange(64),rng.randrange(64)
def rand_pick(st):
    if rng.random()<0.25: return rng.randrange(64),rng.randrange(64)
    v,x,y=rng.choice(st); s=(1,2,3,4,5,7,8,9,10)[v]
    return min(63,max(0,x+s//2+rng.randint(-11,11))),min(63,max(0,y+s//2+rng.randint(-11,11)))
def deep_state(maxk):
    """random walk of successful clicks (engine-driven); returns (g,st) or None"""
    g=lean_copy(g0); st=L3_START
    for k in range(rng.randint(0,maxk)):
        cx,cy=smart(st) if rng.random()<0.85 else rand_pick(st)
        won=eclick(g,st,cx,cy)
        post=est(g)
        record(st,cx,cy,won,post)
        if won: return None
        st=post
    return g,st
def allclicks(g,st):
    n=0
    for cx in range(64):
        for cy in range(64):
            c=lean_copy(g); won=eclick(c,st,cx,cy); record(st,cx,cy,won,est(c)); n+=1
    return n
def allclicks_fast(g,st):
    # inject state through the engine restore path, reuse one engine object; re-copy after a win
    n=0; c=lean_copy(g)
    for cx in range(64):
        for cy in range(64):
            set_state(c,st); assert est(c)==tuple(sorted(st)),(est(c),st)
            won=eclick(c,st,cx,cy); record(st,cx,cy,won,est(c)); n+=1
            if won: c=lean_copy(g)
    return n
t0=time.time(); tot=0
if mode=="walk":
    done=0
    while done<N:
        r=deep_state(14); done+=1
    tot=done
elif mode=="allpos":
    states=0
    while states<N:
        r=deep_state(10)
        if r is None: continue
        g,st=r
        if len(st)>9: continue
        tot+=allclicks_fast(g,st); states+=1
elif mode=="nearwin":
    states=0
    while states<N:
        kind=rng.randrange(4)
        zx,zy=rng.choice([(5,46),(19,46)]); zx2,zy2=rng.choice([(5,46),(19,46)])
        def near(zx,zy,s,r): return min(64-s,max(0,zx+4-s//2+rng.randint(-r,r))),min(63,64-s,max(10,zy+4-s//2+rng.randint(-r,r)))
        r=rng.choice([3,8,14,20])
        if kind==0: st=[(3,)+near(zx,zy,4,r),(2,)+near(zx2,zy2,3,r)]
        elif kind==1: st=[(3,)+near(zx,zy,4,r),(1,)+near(zx2,zy2,2,r),(1,)+near(zx2,zy2,2,r)]
        elif kind==2: st=[(2,)+near(zx,zy,3,r),(2,)+near(zx2,zy2,3,r),(2,)+near(zx2,zy2,3,r)]
        else: st=[(3,)+near(zx,zy,4,r),(2,)+near(zx2,zy2,3,r),(0,)+near(zx,zy,1,r+6)]
        st=tuple(sorted(st))
        tot+=allclicks_fast(g0,st); states+=1
print("GEN",mode,seed,"records",tot,"sec",int(time.time()-t0),flush=True)
