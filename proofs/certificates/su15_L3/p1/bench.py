import sys,time,random
sys.path.insert(0,'.')
from simlib import *
import csim
env=make_env(); fd=env.reset(); g0=env._game
for l in (1,2):
    for (x,y) in our_route(l): fd=env.step(GameAction.ACTION6,data={"x":x,"y":y})
print(sorted((int(g0.kqywaxhmsb[s]),int(s.x),int(s.y)) for s in g0.lkujttxgs), "nscn",len(g0.nscnqkkvg), g0._score)
t=time.time()
for i in range(200):
    g=lean_copy(g0); fast_action(g,GameAction.ACTION6,random.randint(0,63),random.randint(10,62))
print("per click",(time.time()-t)/200)
