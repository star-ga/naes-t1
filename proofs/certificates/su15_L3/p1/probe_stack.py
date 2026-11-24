import sys; sys.path.insert(0,'.')
from simlib import *
import csim
env=make_env(); fd=env.reset(); g0=env._game
for l in (1,2):
    for (x,y) in our_route(l): fd=env.step(GameAction.ACTION6,data={"x":x,"y":y})
def es(g): return tuple(sorted((int(g.kqywaxhmsb[s]),int(s.x),int(s.y)) for s in g.lkujttxgs))
st=es(g0); res={}
for (cx,cy) in [(2,2),(2,40),(63,63),(0,63),(40,9),(40,10),(40,62),(40,63),(28,33),(55,23),(46,31),(20,20)]:
    g=lean_copy(g0); n0=len(g.nscnqkkvg); fast_action(g,GameAction.ACTION6,cx,cy); ns,s,_=csim.click(st,cx,cy); print((cx,cy),s,"stack",n0,"->",len(g.nscnqkkvg))
