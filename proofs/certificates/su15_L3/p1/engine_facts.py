# prints the level-3 facts the proof hard-codes, read from the REAL engine after playing the L1, L2 certificate routes
import sys; sys.path.insert(0,'.')
from simlib import *
env=make_env(); fd=env.reset(); g=env._game
for l in (1,2):
    for (x,y) in our_route(l): fd=env.step(GameAction.ACTION6,data={"x":x,"y":y})
print("level_index",g.level_index)
print("pieces",sorted((int(g.kqywaxhmsb[s]),int(s.x),int(s.y),s.pixels.shape) for s in g.lkujttxgs))
print("zones",[(int(z.x),int(z.y),z.pixels.shape[1],z.pixels.shape[0]) for z in g.powykypsm])
print("goal",g.dsqlbvwaj)
print("enemies",len(g.fezhhzhih))
print("steps_budget",g.current_level.get_data("steps"),"range",g.kacsjmxae,"ticks",g.gdamdvokm,"speed",g.ikskfqldi,"click_rows",g.__class__.__module__)
sc=g.step_counter_ui; print("step_counter",{k:v for k,v in vars(sc).items() if isinstance(v,(int,float,bool))})
