# P1: engine vs simulator on {v3,v2} states near the zones, all 4096 clicks: engine wins that exist ONLY mid-flight (the mutant simulator without the mid-flight win test says fail/no-win) must be reproduced by the exact simulator.
import sys,random,time,os
sys.path.insert(0,'.')
from simlib import *
import csim
os.environ["SIMLIB"]="libsimc_nomid.so"
import importlib.util
spec=importlib.util.spec_from_file_location("csim_nomid","csim.py"); cn=importlib.util.module_from_spec(spec); spec.loader.exec_module(cn)
N=int(sys.argv[1]); random.seed(int(sys.argv[2]))
env=make_env(); fd=env.reset(); g0=env._game
for l in (1,2):
    for (x,y) in our_route(l): fd=env.step(GameAction.ACTION6,data={"x":x,"y":y})
S0=g0._score
def eng_state(g): return tuple(sorted((int(g.kqywaxhmsb[s]),int(s.x),int(s.y)) for s in g.lkujttxgs))
def set_state(g,state):
    g.nscnqkkvg.append([("zmlxwcvwb",v,x,y) for (v,x,y) in state]); g.vczehveskr(); assert eng_state(g)==tuple(sorted(state))
zones=[(5,46),(19,46)]
tot=mism=ew=mid=both=0; ns_=0; t0=time.time()
while ns_<N:
    a=random.choice(zones); b=random.choice(zones)
    st=tuple(sorted([(3,max(0,min(60,a[0]+random.randint(-14,6))),max(10,min(59,a[1]+random.randint(-10,10)))),(2,max(0,min(61,b[0]+random.randint(-14,6))),max(10,min(60,b[1]+random.randint(-10,10))))]))
    g=lean_copy(g0); set_state(g,st)
    # skip pre-won states (engine declares them won on any action)
    zz=[(5,46,9,9),(19,46,9,9)]; SZ_=[1,2,3,4]; cc={}
    for f in st:
        c=(f[1]+SZ_[f[0]]//2,f[2]+SZ_[f[0]]//2)
        if any(zx<=c[0]<zx+zw and zy<=c[1]<zy+zh for zx,zy,zw,zh in zz): cc[f[0]]=cc.get(f[0],0)+1
    if cc.get(3,0)==1 and cc.get(2,0)==1: continue   # already-won injected state is not a legal level state
    ns_+=1
    for cx in range(64):
        for cy in range(10,63):
            c=lean_copy(g); fast_action(c,GameAction.ACTION6,cx,cy); we=c._score>S0
            s1,st1,_=csim.click(st,cx,cy); s2,st2,_=cn.click(st,cx,cy); ws=(st1=='win'); wn=(st2=='win'); tot+=1
            if we!=ws or (not we and eng_state(c)!=s1): mism+=1; print("MISMATCH",st,(cx,cy),we,ws,eng_state(c),s1)
            if we: ew+=1
            if we and not wn: mid+=1
            if we and wn: both+=1
print("LOCKSTEP_MIDFLIGHT states",ns_,"clicks",tot,"engine_wins",ew,"engine_wins_only_mid_flight(mutant says no)",mid,"engine_wins_also_end_state",both,"mismatch",mism,"sec",round(time.time()-t0))
