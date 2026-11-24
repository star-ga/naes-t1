# P2 undo soundness on the REAL engine: after ACTION7 the fruit configuration equals the pre-click configuration of the most recent
# not-yet-undone click (a state visited earlier in the same route). Random click/undo walks incl. undo on empty stack and undo of noop/fail clicks.
import sys,random,time; sys.path.insert(0,'.')
from eng import *
seed=int(sys.argv[1]); NW=int(sys.argv[2]); rng=random.Random(seed)
env=start_env(); g0=env._game; assert est(g0)==L3_START; S0=g0._score; LV0=g0.level_index
walks=clicks=undos=bad=notanc=wins=0; t0=time.time()
for w in range(NW):
    g=lean_copy(g0); hist=[]  # stack of pre-click states (clicks inside grid, incl. noop/fail)
    visited={est(g)}
    for k in range(rng.randint(2,20)):
        reset_budget(g)
        if rng.random()<0.3:
            before=est(g); top=tuple(sorted((int(t[1]),int(t[2]),int(t[3])) for t in g.nscnqkkvg[-1])) if g.nscnqkkvg else before
            if top not in visited: notanc+=1; print("STACK_TOP_NOT_VISITED")
            fast_action(g,GameAction.ACTION7); undos+=1
            exp=top
            if est(g)!=exp: bad+=1; print("UNDO_MISMATCH",walks,k,exp,est(g))
            if est(g) not in visited: notanc+=1
        else:
            st=est(g); v,x,y=rng.choice(st); s=(1,2,3,4,5,7,8,9,10)[v]
            cx=min(63,max(0,x+s//2+rng.randint(-10,10))); cy=min(63,max(0,y+s//2+rng.randint(-10,10)))
            fast_action(g,GameAction.ACTION6,cx,cy); clicks+=1
            if g.level_index!=LV0 or g._state==GameState.WIN: wins+=1; break
            visited.add(est(g))
    walks+=1
print(f"UNDOCHECK walks={walks} clicks={clicks} undos={undos} undo_state_mismatch={bad} undo_to_unvisited_state={notanc} wins={wins} sec={int(time.time()-t0)}")
