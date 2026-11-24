# all distinct lossless successors of the level-3 start (simulator), plus the start itself; random sample of depth-1 states
import sys,random,ctypes
lib=ctypes.CDLL("./libsim.so"); lib.sim_click.argtypes=[ctypes.c_int,ctypes.POINTER(ctypes.c_int),ctypes.c_int,ctypes.c_int,ctypes.POINTER(ctypes.c_int)]
def click(st,cx,cy):
    a=(ctypes.c_int*(3*len(st)))(*[t for p in st for t in p]); o=(ctypes.c_int*30)()
    r=lib.sim_click(len(st),a,cx,cy,o); n=o[0]; return r,tuple(sorted((o[1+3*i],o[2+3*i],o[3+3*i]) for i in range(n)))
START=tuple(sorted([(0,55,23),(0,61,23),(0,31,22),(0,31,15),(0,12,23),(0,8,28),(1,46,22),(1,30,32),(1,18,16)]))
fmt=lambda s:f"{len(s)} "+" ".join(f"{v} {x} {y}" for v,x,y in s)
kids=set(); allr=set()
for P in range(4096):
    r,o=click(START,P//64,P%64)
    if r==2 and sum(1<<v for v,_,_ in o)==12 and o!=START: kids.add(o)
kids=sorted(kids); print("DEPTH1_DISTINCT_LOSSLESS_CHANGED",len(kids))
open("lemma_lens/root_state.txt","w").write(fmt(START)+"\n")
rng=random.Random(int(sys.argv[1])); smp=rng.sample(kids,int(sys.argv[2]))
open("lemma_lens/depth1_sample.txt","w").write("".join(fmt(s)+"\n" for s in smp))
