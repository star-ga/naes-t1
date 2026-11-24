import sys,json,time,copy,hashlib,os
import numpy as np
import arc_agi
from arcengine.enums import GameAction, GameState
from arcengine import ActionInput
GAMES="games"
SU15=GAMES+"/su15/1944f8ab/su15.py"
CERT="certificates"
def check_sha():
    h=hashlib.sha256(open(SU15,'rb').read()).hexdigest()
    assert h=="a5f91f7c963d6ca6447dae0ab21342b48a3f511601c40dfa8e972bdc59b4651e",h
def make_env():
    check_sha()
    return arc_agi.Arcade(operation_mode="offline",environments_dir=GAMES).make("su15-1944f8ab",seed=0,save_recording=False)
def our_route(l):
    c=json.load(open(f"{CERT}/su15_L{l}.json"))
    return [(a['x'],a['y']) for a in c['upper_bound']['route']]
def fast_action(g, aid, x=0, y=0):
    """Same loop as ARCBaseGame.perform_action, minus frame rendering."""
    ai = ActionInput(id=aid, data=({"x":x,"y":y} if aid==GameAction.ACTION6 else {}))
    g._full_reset=False
    if g._state in (GameState.GAME_OVER, GameState.WIN): return
    g._set_action(ai)
    n=0
    while not g.is_action_complete():
        n+=1
        assert n<=2000
        if g._next_level: g._really_set_next_level()
        else: g.step()
def key(g):
    """Full future-deciding state: ordered fruit list, ordered enemy list, step counter, level, special sprite."""
    fr=tuple((int(g.kqywaxhmsb.get(s,0)),int(s.x),int(s.y)) for s in g.lkujttxgs)
    en=tuple((g.kcuphgwar.get(s),int(s.x),int(s.y)) for s in g.fezhhzhih)
    sc=g.step_counter_ui
    sp=(int(g.bicnaxoxq._x),) if getattr(g,'bicnaxoxq',None) is not None else ()
    return (g._score, fr, en, sp)
def counters(g):
    sc=g.step_counter_ui
    return (sc.current_steps, sc.penalty)

def lean_copy(g):
    """deepcopy sharing never-mutated heavy members (camera, clean levels, action list, non-current levels)."""
    memo={id(g._camera):g._camera, id(g._clean_levels):g._clean_levels, id(g.actions):g.actions}
    for lv in g._levels:
        if lv is not g.current_level: memo[id(lv)]=lv
    memo[id(g._levels)]=None
    # _levels list must be copied (holds current level) -> do not share list itself
    del memo[id(g._levels)]
    return copy.deepcopy(g,memo)
