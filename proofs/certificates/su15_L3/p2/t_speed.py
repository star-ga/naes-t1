import sys,time; sys.path.insert(0,'.')
from eng import *
env=start_env(); g=env._game
assert est(g)==L3_START
t=time.time(); n=0
for k in range(300):
    set_state(g,L3_START); assert est(g)==L3_START
    w,s=act(g,40,24); n+=1
    if w or s!=GameState.NOT_FINISHED: print("stop",k,w,s); break
print(n/(time.time()-t),"clicks/s", est(g), g.step_counter_ui.current_steps, g.step_counter_ui.penalty, g._state)
