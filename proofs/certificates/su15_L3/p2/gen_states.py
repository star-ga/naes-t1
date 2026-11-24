# sample lossless reachable states with n pieces (sim-driven random walks of successful, non-lossy clicks); writes one state per line
import sys,random,ctypes
sys.path.insert(0,'.')
lib=ctypes.CDLL("./libsim.so"); lib.sim_click.argtypes=[ctypes.c_int,ctypes.POINTER(ctypes.c_int),ctypes.c_int,ctypes.c_int,ctypes.POINTER(ctypes.c_int)]
def click(st,cx,cy):
    a=(ctypes.c_int*(3*len(st)))(*[t for p in st for t in p]); o=(ctypes.c_int*30)()
    r=lib.sim_click(len(st),a,cx,cy,o); n=o[0]; return r,tuple(sorted((o[1+3*i],o[2+3*i],o[3+3*i]) for i in range(n)))
START=tuple(sorted([(0,55,23),(0,61,23),(0,31,22),(0,31,15),(0,12,23),(0,8,28),(1,46,22),(1,30,32),(1,18,16)]))
SZ=(1,2,3,4,5,7,8,9,10)
def mass(st): return sum(1<<v for v,_,_ in st)
seed=int(sys.argv[1]); per=int(sys.argv[2]); ns=[int(t) for t in sys.argv[3].split(",")]; out=sys.argv[4]; rng=random.Random(seed)
want={n:per for n in ns}; got=[]
tries=0
while any(want.values()) and tries<2000000:
    tries+=1; st=START
    for step in range(12):
        # try merges preferentially (bias towards zones) else moves
        cands=[]
        for _ in range(30):
            v,x,y=rng.choice(st); s=SZ[v]; cx=min(63,max(0,x+s//2+rng.randint(-10,10))); cy=min(62,max(10,y+s//2+rng.randint(-10,10)))
            r,ns_=click(st,cx,cy)
            if r==2 and ns_!=st and mass(ns_)==12: cands.append(ns_)
        if not cands: break
        merged=[c for c in cands if len(c)<len(st)]
        st=rng.choice(merged if merged and rng.random()<0.8 else cands)
        n=len(st)
        if want.get(n,0)>0 and rng.random()<0.5:
            want[n]-=1; got.append(st)
with open(out,"w") as f:
    for st in got: f.write(f"{len(st)} "+" ".join(f"{v} {x} {y}" for v,x,y in st)+"\n")
print("GENSTATES",len(got),{n:per-want[n] for n in ns},"tries",tries)
