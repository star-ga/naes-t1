# p2 self-refutation (LEMMA lens): states at small true distance to a win: states of 3 engine-replayed 11-click wins
# (distance 1..6 by construction) plus random successful lossless children/grandchildren of them.
import sys,random,ctypes
lib=ctypes.CDLL("./libsim.so"); lib.sim_click.argtypes=[ctypes.c_int,ctypes.POINTER(ctypes.c_int),ctypes.c_int,ctypes.c_int,ctypes.POINTER(ctypes.c_int)]
def click(st,cx,cy):
    a=(ctypes.c_int*(3*len(st)))(*[t for p in st for t in p]); o=(ctypes.c_int*30)()
    r=lib.sim_click(len(st),a,cx,cy,o); n=o[0]; return r,tuple(sorted((o[1+3*i],o[2+3*i],o[3+3*i]) for i in range(n)))
START=tuple(sorted([(0,55,23),(0,61,23),(0,31,22),(0,31,15),(0,12,23),(0,8,28),(1,46,22),(1,30,32),(1,18,16)]))
routes=["55,28 47,31 14,24 34,22 33,30 22,24 40,36 27,31 23,39 32,42 23,46","55,28 46,28 12,31 31,23 27,30 18,25 19,34 17,43 39,34 32,40 25,46","4,23 12,22 13,31 17,39 28,21 33,27 55,27 47,30 39,34 31,40 23,46"]
def mass(st): return sum(1<<v for v,_,_ in st)
rng=random.Random(int(sys.argv[1])); K=int(sys.argv[2]); out=open(sys.argv[3],"w"); seen=set(); rows=0
def kids(st):
    res=set()
    for P in range(4096):
        r,o=click(st,P//64,P%64)
        if r==2 and o!=st and mass(o)==12: res.add(o)
    return sorted(res)
def emit(st,tag):
    global rows
    if st in seen: return
    seen.add(st); out.write(f"{len(st)} "+" ".join(f"{v} {x} {y}" for v,x,y in st)+"\n"); rows+=1
for ri,rt in enumerate(routes):
    st=START; sts=[st]
    for t in rt.split()[:-1]:
        x,y=map(int,t.split(",")); r,st=click(st,x,y); assert r==2; sts.append(st)
    for i in range(5,11): emit(sts[i],"route")
    for i in range(7,11):
        ks=kids(sts[i]); 
        for c in rng.sample(ks,min(K,len(ks))): emit(c,"child")
    for i in range(9,11):
        ks=kids(sts[i])
        for c in rng.sample(ks,min(3,len(ks))):
            gk=kids(c)
            for g in rng.sample(gk,min(4,len(gk))): emit(g,"grandchild")
print("NEARWIN_STATES",rows)
