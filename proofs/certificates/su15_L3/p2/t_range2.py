import sys; sys.path.insert(0,'.')
from eng import *
env=start_env(); g0=env._game
def run(x,y,how):
    g=lean_copy(g0); reset_budget(g)
    if how=="fast": fast_action(g,GameAction.ACTION6,x,y)
    else: g.perform_action(ActionInput(id=GameAction.ACTION6,data={"x":x,"y":y}))
    return est(g)
print("fast (31,30):",run(31,30,"fast")[-3:])
print("perf (31,30):",run(31,30,"perf")[-3:])
for (x,y) in [(64,32),(65,32),(-1,32),(31,64),(31,-5),(100,32),(255,255)]:
    a=run(x,y,"fast"); print((x,y),"changed" if a!=L3_START else "noop")
env2=start_env()
for (x,y) in [(64,32)]:
    fd=env2.step(GameAction.ACTION6,data={"x":x,"y":y}); print("env.step",(x,y),"changed" if est(env2._game)!=L3_START else "noop")
