# ENGINE-lens self-attack: which tick (1,2,3 mid-flight; 4 = end state) does each REAL-engine win happen at, and does simc.h agree on every click?
# states {v3,v2} (and {v3,v1,v1},{v2,v2,v2} for end-state merges) near the zones, all clicks with rows 10..62.
import sys,random,time
sys.path.insert(0,'.')
from simlib import *
import csim
N=int(sys.argv[1]); random.seed(int(sys.argv[2]))
env=make_env(); fd=env.reset(); g0=env._game
for l in (1,2):
    for (x,y) in our_route(l): fd=env.step(GameAction.ACTION6,data={"x":x,"y":y})
S0=g0._score
def eng_state(g): return tuple(sorted((int(g.kqywaxhmsb[s]),int(s.x),int(s.y)) for s in g.lkujttxgs))
def set_state(g,state):
    g.nscnqkkvg.append([("zmlxwcvwb",v,x,y) for (v,x,y) in state]); g.vczehveskr(); assert eng_state(g)==tuple(sorted(state))
SZ=[1,2,3,4]; Z=[(5,46,9,9),(19,46,9,9)]
def inz(v,x,y): c=(x+SZ[v]//2,y+SZ[v]//2); return any(zx<=c[0]<zx+zw and zy<=c[1]<zy+zh for zx,zy,zw,zh in Z)
def won(st):
    cnt={}
    for v,x,y in st:
        if inz(v,x,y): cnt[v]=cnt.get(v,0)+1
    return cnt.get(3,0)==1 and cnt.get(2,0)==1
def clamp(v,x,y):
    s=SZ[v]; x=max(0,min(64-s,x)); y=max(10,min(63,y)); y=min(64-s,y); return x,y
def inr(cx,cy,v,x,y):
    s=SZ[v]; nx=min(max(cx,x),x+s-1); ny=min(max(cy,y),y+s-1); return (cx-nx)**2+(cy-ny)**2<=64
def first_tick(st,cx,cy):
    cur=list(st); hit=[i for i,(v,x,y) in enumerate(st) if inr(cx,cy,v,x,y)]
    for t in range(1,4):
        for i in hit:
            v,x,y=cur[i]; c0,c1=x+SZ[v]//2,y+SZ[v]//2; dx=max(-4,min(4,cx-c0)); dy=max(-4,min(4,cy-c1)); x,y=clamp(v,x+dx,y+dy); cur[i]=(v,x,y)
        if won(cur): return t
    return 4
t0=time.time(); tot=mism=0; ticks={1:0,2:0,3:0,4:0}; n=0
while n<N:
    shape=random.choice([[3,2],[3,2],[3,2],[3,1,1],[2,2,2]]); st=[]
    for v in shape:
        zx,zy,_,_=random.choice(Z); s=SZ[v]; st.append((v,max(0,min(64-s,zx+random.randint(-12,12))),max(10,min(64-s,zy+random.randint(-12,12)))))
    st=tuple(sorted(st))
    if won(st): continue
    g=lean_copy(g0); set_state(g,st); n+=1
    for cx in range(64):
        for cy in range(10,63):
            c=lean_copy(g); fast_action(c,GameAction.ACTION6,cx,cy); we=c._score>S0
            ns,status,_=csim.click(st,cx,cy); ws=(status=='win'); tot+=1
            if we!=ws or (not we and eng_state(c)!=ns): mism+=1; print("MISMATCH",st,(cx,cy),we,ws)
            if we: ticks[first_tick(st,cx,cy)]+=1
print("LOCK_TICKS states",n,"clicks",tot,"mismatch",mism,"engine_wins_by_first_winning_tick",ticks,"sec",round(time.time()-t0))
