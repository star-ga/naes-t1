# P1: lockstep of the exact simulator simc.h (via libsimc.so) against the REAL engine su15.py (sha checked in simlib).
# modes: walks <seed> <NW> <MAXLEN>   random walks incl. ACTION7 undo, compares state + undo-history stack each step
#        allpos <seed> <NS> <kind>    NS states x 64x64 click positions; kind: reach|near|rand|mix
import sys,random,time
sys.path.insert(0,'.')
from simlib import *
import csim
mode=sys.argv[1]; seed=int(sys.argv[2]); N=int(sys.argv[3]); random.seed(seed)
env=make_env(); fd=env.reset(); g0=env._game
for l in (1,2):
    for (x,y) in our_route(l): fd=env.step(GameAction.ACTION6,data={"x":x,"y":y})
S0=g0._score
def eng_state(g): return tuple(sorted((int(g.kqywaxhmsb[s]),int(s.x),int(s.y)) for s in g.lkujttxgs))
def eng_stack(g): return [tuple(sorted((v,x,y) for (t,v,x,y) in e if t=="zmlxwcvwb")) for e in g.nscnqkkvg]
L3=((0,8,28),(0,12,23),(0,31,15),(0,31,22),(0,55,23),(0,61,23),(1,18,16),(1,30,32),(1,46,22))
assert eng_state(g0)==L3
def set_state(g,state):
    g.nscnqkkvg.append([("zmlxwcvwb",v,x,y) for (v,x,y) in state]); g.vczehveskr()
    assert eng_state(g)==tuple(sorted(state)),(eng_state(g),state)
def ctr(f):
    from_sz=[1,2,3,4,5,7,8,9,10]; return f[1]+from_sz[f[0]]//2,f[2]+from_sz[f[0]]//2
def prewon(st):
    z=[(5,46,9,9),(19,46,9,9)]; SZ=[1,2,3,4,5,7,8,9,10]; cnt={}
    for f in st:
        c=(f[1]+SZ[f[0]]//2,f[2]+SZ[f[0]]//2)
        if any(zx<=c[0]<zx+zw and zy<=c[1]<zy+zh for zx,zy,zw,zh in z): cnt[f[0]]=cnt.get(f[0],0)+1
    return cnt.get(3,0)==1 and cnt.get(2,0)==1
def pick(st):
    r=random.random()
    if r<0.6 and st:
        f=random.choice(st); c=ctr(f)
        return min(63,max(0,c[0]+random.randint(-9,9))),min(65,max(8,c[1]+random.randint(-9,9)))
    return random.randint(0,63),random.randint(8,64)
t0=time.time()
if mode=="walks":
    MAXLEN=int(sys.argv[4]); walks=steps=mism=wins=gameover=undos=0; stat={'ok':0,'noop':0,'fail':0,'win':0,'undo':0}; lossy_seen=0
    for w in range(N):
        g=lean_copy(g0); cur=L3; stack=eng_stack(g); log=[]
        for k in range(random.randint(1,MAXLEN)):
            if random.random()<0.06:
                fast_action(g,GameAction.ACTION7); log.append('U'); steps+=1; stat['undo']+=1
                if stack: cur=stack.pop()
                status='undo'; win_s=False
            else:
                cx,cy=pick(cur); ns,status,ls=csim.click(cur,cx,cy); lossy_seen+=ls
                fast_action(g,GameAction.ACTION6,cx,cy); log.append((cx,cy,status)); steps+=1; stat[status]+=1
                win_s=(status=='win')
                if cx<=63 and cy<=63: stack.append(cur)
                if status=='fail': stack.pop()  # collision restores the snapshot and pops it
                if status in('ok',): cur=ns
            win_e=(g._score>S0)
            if g._state==GameState.GAME_OVER: gameover+=1; break
            if win_e or win_s:
                if win_e!=win_s: mism+=1; print("MISMATCH win",w,k,status,win_e,win_s)
                else: wins+=1
                break
            if eng_state(g)!=cur or eng_stack(g)!=stack:
                mism+=1
                if mism<=5: print("MISMATCH walk",w,"step",k,status,log,"\n sim",cur,"\n eng",eng_state(g),"\n simstack",stack,"\n engstack",eng_stack(g))
                break
        walks+=1
    print("LOCKSTEP_WALKS seed",seed,"walks",walks,"steps",steps,"mismatch",mism,"wins",wins,"gameover",gameover,stat,"lossy_clicks",lossy_seen,"sec",round(time.time()-t0))
else:
    kind=sys.argv[4]; tot=mism=0; wins_both=0; stat={'ok':0,'noop':0,'fail':0,'win':0}; lossy_seen=0; nstate=0
    def gen_state():
        k=kind if kind!="mix" else random.choice(["reach","near","rand"])
        if k=="reach":
            st=L3; 
            for _ in range(random.randint(0,10)):
                for _t in range(30):
                    f=random.choice(st); c=ctr(f); cx=min(63,max(0,c[0]+random.randint(-9,9))); cy=min(62,max(10,c[1]+random.randint(-9,9)))
                    ns,s,_l=csim.click(st,cx,cy)
                    if s=='ok': st=ns;break
            return st
        zones=[(5,46),(19,46)]
        if k=="near":
            shape=random.choice([[3,2],[3,1,1],[2,2,2],[3,2],[3,2],[3,1,0,0],[2,2,1,1],[3,2]]);
            st=[]
            for v in shape:
                zx,zy=random.choice(zones); s={0:1,1:2,2:3,3:4}[v]
                st.append((v,max(0,min(64-s,zx+random.randint(-9,9))),max(10,min(63-s,zy+random.randint(-9,9)))))
            return tuple(sorted(st))
        # rand: random multiset with mass 12 or random mass
        masses=random.choice([[3,2],[2,2,2],[3,1,1],[1]*6+[0]*0,[0]*6+[1]*3,[3,1,0,0],[1,1,1,1,1,1],[2,1,1,1,1]])
        st=[]
        for v in masses:
            s={0:1,1:2,2:3,3:4}[v]; st.append((v,random.randint(0,64-s),random.randint(10,min(63,64-s))))
        return tuple(sorted(st))
    while nstate<N:
        st=gen_state()
        if prewon(st) or len(st)>9: continue   # an already-won injected state is not a legal level state (engine declares the win on any action)
        g=lean_copy(g0)
        try: set_state(g,st)
        except AssertionError as e: print("setstate skip",e); continue
        nstate+=1
        for cx in range(64):
            for cy in range(64):
                c=lean_copy(g); fast_action(c,GameAction.ACTION6,cx,cy)
                ns,status,ls=csim.click(st,cx,cy); lossy_seen+=ls; stat[status]+=1
                win_e=c._score>S0; win_s=(status=='win'); tot+=1
                if c._state==GameState.GAME_OVER: continue
                if win_e!=win_s or (not win_e and eng_state(c)!=ns):
                    mism+=1
                    if mism<=5: print("MISMATCH state",st,"click",(cx,cy),status,"sim",ns,"eng",eng_state(c),win_e,win_s)
                elif win_e: wins_both+=1
        if nstate%25==0: print("progress",nstate,tot,mism,flush=True)
    print("LOCKSTEP_ALLPOS seed",seed,"kind",kind,"states",nstate,"clicks",tot,"mismatch",mism,"wins_matched",wins_both,stat,"lossy_clicks",lossy_seen,"sec",round(time.time()-t0))
