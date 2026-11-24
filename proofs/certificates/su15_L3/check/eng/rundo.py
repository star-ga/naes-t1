# Adversarial check UNDO / budget lens: random walks of clicks + undos on the REAL engine vs the model
# model: history stack starts [start]; every in-grid click pushes the pre-click state (also rows 0-9/63 no-ops);
# collision pops it (state restored) ; ACTION7 pops if non-empty. Budget: hit/no-hit in-row click -1, collision -(2+2*pen).
import sys,random,ctypes,time
import os; HERE=os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0,HERE)
from reng import *
lib=ctypes.CDLL(os.path.join(HERE,'..','src','librsim.so'))
A=ctypes.c_int*16
def sim(st,px,py):
    n=len(st); v=A(*[p[0] for p in st]); x=A(*[p[1] for p in st]); y=A(*[p[2] for p in st])
    on=ctypes.c_int(); ov=A(); ox=A(); oy=A(); wt=ctypes.c_int()
    r=lib.rclick_flat(n,v,x,y,px,py,ctypes.byref(on),ov,ox,oy,ctypes.byref(wt))
    return r, sorted((ov[i],ox[i],oy[i]) for i in range(on.value)), wt.value
seed=int(sys.argv[1]); W=int(sys.argv[2]); random.seed(seed)
g0=level3_game(); start=est(g0)
stats=dict(walks=0,actions=0,undos=0,undo_empty=0,clicks=0,noop_row=0,nohit=0,fail=0,ok=0,win=0,gameover=0,mism=0,undo_unvisited=0,budget_mism=0)
t0=time.time()
for w in range(W):
    g=copyg(g0); st=list(start); stack=[list(start)]; visited={tuple(start)}; steps=48; pen=0
    for k in range(random.randint(5,40)):
        stats['actions']+=1
        r=random.random()
        if r<0.3:
            do(g,GameAction.ACTION7); stats['undos']+=1
            if stack: st=stack.pop()
            else: stats['undo_empty']+=1
            if tuple(st) not in visited: stats['undo_unvisited']+=1
        else:
            stats['clicks']+=1
            q=random.random()
            if q<0.08: px,py=random.randint(0,63),random.choice([0,5,9,63])
            elif q<0.2: px,py=random.randint(0,63),random.randint(10,62)
            else:
                p=random.choice(st); px=max(0,min(63,p[1]+random.randint(-9,9))); py=max(10,min(62,p[2]+random.randint(-9,9)))
                if random.random()<0.4 and len(st)>1:
                    p2=random.choice(st); px=max(0,min(63,(p[1]+p2[1])//2+random.randint(-2,2))); py=max(10,min(62,(p[2]+p2[2])//2+random.randint(-2,2)))
            code,ns,wt=sim(st,px,py)
            do(g,GameAction.ACTION6,px,py)
            stack.append(list(st))
            if py<=9 or py>=63: stats['noop_row']+=1
            elif code==0: stats['nohit']+=1; steps-=1
            elif code==1: stats['fail']+=1; stack.pop(); steps-=2+2*pen; pen+=1
            elif code==2: stats['ok']+=1; st=ns; steps-=1
            elif code==3:
                stats['win']+=1
                if not (g.level_index!=2 or g._state==GameState.WIN): stats['mism']+=1; print("MISMATCH win",seed,w,k,px,py,st)
                break
            visited.add(tuple(st))
        if g._state==GameState.GAME_OVER:
            stats['gameover']+=1
            if steps>0: stats['budget_mism']+=1; print("BUDGET_MISMATCH gameover with model steps",steps)
            break
        if g.level_index!=2: stats['mism']+=1; print("MISMATCH engine won, sim not",seed,w,k); break
        e=est(g)
        if e!=sorted(st): stats['mism']+=1; print("MISMATCH state",seed,w,k,"sim",sorted(st),"eng",e); break
        c=g.step_counter_ui
        if c.current_steps!=max(steps,0) and steps>0: stats['budget_mism']+=1; print("BUDGET_MISMATCH",c.current_steps,steps,c.penalty,pen)
        if len(g.nscnqkkvg)!=len(stack): stats['mism']+=1; print("MISMATCH stack depth",len(g.nscnqkkvg),len(stack)); break
    stats['walks']+=1
print("RUNDO",seed,stats,"sec=%.0f"%(time.time()-t0))
