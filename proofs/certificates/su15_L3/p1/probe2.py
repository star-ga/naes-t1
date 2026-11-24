import sys; sys.path.insert(0,'.')
from simlib import *
import csim,random
env=make_env(); fd=env.reset(); g0=env._game
for l in (1,2):
    for (x,y) in our_route(l): fd=env.step(GameAction.ACTION6,data={"x":x,"y":y})
def es(g): return tuple(sorted((int(g.kqywaxhmsb[s]),int(s.x),int(s.y)) for s in g.lkujttxgs))
g=lean_copy(g0)
print("start",len(g.nscnqkkvg))
for a in ["u","u","c20,20","u","u","u"]:
    if a=="u": fast_action(g,GameAction.ACTION7)
    else:
        x,y=map(int,a[1:].split(',')); fast_action(g,GameAction.ACTION6,x,y)
    print(a,len(g.nscnqkkvg),es(g)==es(g0))
