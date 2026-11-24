import sys; sys.path.insert(0,'.')
from eng import *
env=start_env(); g0=env._game
for (x,y) in [(64,30),(70,30),(-1,30),(30,64),(30,-5),(200,200),(0,0),(63,63),(31,30)]:
    g=lean_copy(g0)
    try:
        reset_budget(g); g.perform_action(ActionInput(id=GameAction.ACTION6,data={"x":x,"y":y})); print((x,y),"ok",est(g)==L3_START, g._state.name)
    except Exception as e: print((x,y),"EXC",type(e).__name__,str(e)[:80])
# also via env.step (public API)
for (x,y) in [(64,30),(-1,30),(30,64)]:
    try:
        env2=start_env(); fd=env2.step(GameAction.ACTION6,data={"x":x,"y":y}); print("env.step",(x,y),"ok",fd is not None and est(env2._game)==L3_START)
    except Exception as e: print("env.step",(x,y),"EXC",type(e).__name__,str(e)[:100])
