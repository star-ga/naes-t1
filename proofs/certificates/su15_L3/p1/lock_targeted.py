# ENGINE-lens self-attack: lockstep of simc.h (libsimc.so) vs the REAL engine on adversarial states, all 4096 clicks each:
#  kind border : pieces clamped on the borders (x=0, x=64-s, y=10, y=64-s) incl. mixed values
#  kind cluster: 3..6 same-value pieces packed within distance 8 (merge groups of 3+), plus a stray piece
#  kind d8     : {v3,v2} / {v2,v2,v3} pieces placed so that many clicks are at Euclidean distance exactly 8 from a piece edge, near the zones
import sys,random,time
sys.path.insert(0,'.')
from simlib import *
import csim
seed=int(sys.argv[1]); N=int(sys.argv[2]); kind=sys.argv[3]; random.seed(seed)
env=make_env(); fd=env.reset(); g0=env._game
for l in (1,2):
    for (x,y) in our_route(l): fd=env.step(GameAction.ACTION6,data={"x":x,"y":y})
S0=g0._score
def eng_state(g): return tuple(sorted((int(g.kqywaxhmsb[s]),int(s.x),int(s.y)) for s in g.lkujttxgs))
def set_state(g,state):
    g.nscnqkkvg.append([("zmlxwcvwb",v,x,y) for (v,x,y) in state]); g.vczehveskr(); assert eng_state(g)==tuple(sorted(state))
SZ=[1,2,3,4]; Z=[(5,46,9,9),(19,46,9,9)]
def prewon(st):
    cnt={}
    for v,x,y in st:
        c=(x+SZ[v]//2,y+SZ[v]//2)
        if any(zx<=c[0]<zx+zw and zy<=c[1]<zy+zh for zx,zy,zw,zh in Z): cnt[v]=cnt.get(v,0)+1
    return cnt.get(3,0)==1 and cnt.get(2,0)==1
def gen():
    if kind=="border":
        st=[]
        for v in random.choice([[3,2],[3,1,1],[2,2,2],[0,0,0,0,1,1,1],[3,1,0,0],[2,2,1,1]]):
            s=SZ[v]; side=random.randint(0,3)
            if side==0: x,y=0,random.randint(10,64-s)
            elif side==1: x,y=64-s,random.randint(10,64-s)
            elif side==2: x,y=random.randint(0,64-s),10
            else: x,y=random.randint(0,64-s),64-s
            st.append((v,x,y))
        return tuple(sorted(st))
    if kind=="cluster":
        v=random.choice([0,0,1,1,2]); k=random.randint(3,6 if v==0 else (6 if v==1 else 3)); cx,cy=random.randint(4,58),random.randint(14,56)
        st=[(v,max(0,min(64-SZ[v],cx+random.randint(-5,5))),max(10,min(64-SZ[v],cy+random.randint(-5,5)))) for _ in range(k)]
        w=random.choice([0,1,2,3]); st.append((w,max(0,min(64-SZ[w],cx+random.randint(-9,9))),max(10,min(64-SZ[w],cy+random.randint(-9,9)))))
        return tuple(sorted(st))
    # d8
    st=[]
    for v in random.choice([[3,2],[3,2],[2,2,3],[3,1,1]]):
        zx,zy,_,_=random.choice(Z); s=SZ[v]
        st.append((v,max(0,min(64-s,zx+random.choice([-8,-7,0,4,8,9,12]))),max(10,min(64-s,zy+random.choice([-8,-7,0,4,8,9,12])))))
    return tuple(sorted(st))
t0=time.time(); tot=mism=0; stat={'ok':0,'noop':0,'fail':0,'win':0}; lossy=0; n=0; d8clicks=0
while n<N:
    st=gen()
    if prewon(st) or len(set(st))<len(st) and kind!="cluster": continue
    g=lean_copy(g0)
    try: set_state(g,st)
    except AssertionError: continue
    n+=1
    for cx in range(64):
        for cy in range(64):
            for v,x,y in st:
                nx=min(max(cx,x),x+SZ[v]-1); ny=min(max(cy,y),y+SZ[v]-1)
                if (cx-nx)**2+(cy-ny)**2==64: d8clicks+=1
            c=lean_copy(g); fast_action(c,GameAction.ACTION6,cx,cy)
            ns,status,ls=csim.click(st,cx,cy); lossy+=ls; stat[status]+=1; tot+=1
            win_e=c._score>S0; win_s=(status=='win')
            if c._state==GameState.GAME_OVER: print("GAMEOVER",st,cx,cy); continue
            if win_e!=win_s or (not win_e and eng_state(c)!=ns):
                mism+=1
                if mism<=5: print("MISMATCH",st,(cx,cy),status,ns,eng_state(c),win_e,win_s)
print("LOCK_TARGETED kind",kind,"seed",seed,"states",n,"clicks",tot,"mismatch",mism,stat,"lossy_clicks",lossy,"piece_click_pairs_at_distance_exactly_8",d8clicks,"sec",round(time.time()-t0))
