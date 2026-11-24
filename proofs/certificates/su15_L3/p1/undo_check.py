# P2 undo soundness, checked on the REAL engine: random walks mixing clicks (ACTION6) and undos (ACTION7).
# Invariant checked after EVERY action t: the engine's current piece state e was already reached in this walk by an undo-free click route
# of length dmin(e) <= t (dmin tracked from the route itself). An undo must land on an already-visited state (an ancestor/earlier state)
# and exactly on the state the stack model predicts. => a winning route using undo can be replaced by an undo-free one that is not longer.
import sys,random,time
sys.path.insert(0,'.')
from simlib import *
import csim
seed=int(sys.argv[1]); NW=int(sys.argv[2]); ML=int(sys.argv[3]); PU=float(sys.argv[4]); random.seed(seed)
env=make_env(); fd=env.reset(); g0=env._game
for l in (1,2):
    for (x,y) in our_route(l): fd=env.step(GameAction.ACTION6,data={"x":x,"y":y})
def eng_state(g): return tuple(sorted((int(g.kqywaxhmsb[s]),int(s.x),int(s.y)) for s in g.lkujttxgs))
L3=eng_state(g0)
def ctr(f): SZ=[1,2,3,4]; return f[1]+SZ[f[0]]//2,f[2]+SZ[f[0]]//2
t0=time.time(); undos=bad=acts=deep=0; maxstack=0; undo_to_nonvisited=0; undo_model_mism=0; inv_viol=0
for w in range(NW):
    g=lean_copy(g0); cur=L3; dmin={L3:0}; stack=[L3]  # on_set_level snapshot
    for t in range(1,ML+1):
        if random.random()<PU:
            fast_action(g,GameAction.ACTION7); undos+=1
            exp=stack.pop() if stack else cur
            e=eng_state(g)
            if e not in dmin: undo_to_nonvisited+=1; print("UNDO_TO_NONVISITED",w,t,e)
            if e!=exp: undo_model_mism+=1; print("UNDO_MODEL_MISMATCH",w,t,e,exp)
            cur=e
        else:
            if cur and random.random()<0.8:
                f=random.choice(cur); c=ctr(f); cx=min(63,max(0,c[0]+random.randint(-9,9))); cy=min(63,max(0,c[1]+random.randint(-9,9)))
            else: cx,cy=random.randint(0,63),random.randint(0,63)
            ns,status,ls=csim.click(cur,cx,cy)
            fast_action(g,GameAction.ACTION6,cx,cy)
            if status=='win' or g._state==GameState.GAME_OVER or g._score>g0._score: break
            stack.append(cur)
            if status=='fail': stack.pop()
            new=eng_state(g)
            if status=='ok' and new!=ns: bad+=1; print("SIM_MISMATCH",w,t)
            d=dmin[cur]+1 if new!=cur else dmin[cur]
            if new not in dmin or dmin[new]>d: dmin[new]=d
            cur=new
        acts+=1; maxstack=max(maxstack,len(stack))
        if dmin.get(cur,10**9)>t: inv_viol+=1; print("INVARIANT_VIOLATION",w,t)
        if t>=10: deep+=1
print("UNDO_CHECK seed",seed,"walks",NW,"actions",acts,"undos",undos,"actions_at_depth>=10",deep,"max_stack",maxstack,"undo_to_nonvisited",undo_to_nonvisited,"undo_model_mismatch",undo_model_mism,"invariant_dmin<=t_violations",inv_viol,"sim_mismatch",bad,"sec",round(time.time()-t0))
