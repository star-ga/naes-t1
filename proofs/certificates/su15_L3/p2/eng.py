# Engine helpers for p2 lockstep (real engine, offline). Python = python
import sys,json,copy,hashlib,time,random,os
import arc_agi
from arcengine.enums import GameAction, GameState
from arcengine import ActionInput
GAMES=os.environ.get("SU15_GAMES","games")
SHA="a5f91f7c963d6ca6447dae0ab21342b48a3f511601c40dfa8e972bdc59b4651e"
CERT=os.environ.get("SU15_CERT","certificates")
L3_START=((0,8,28),(0,12,23),(0,31,15),(0,31,22),(0,55,23),(0,61,23),(1,18,16),(1,30,32),(1,46,22))
def start_env():
    assert hashlib.sha256(open(GAMES+"/su15/1944f8ab/su15.py","rb").read()).hexdigest()==SHA
    env=arc_agi.Arcade(operation_mode="offline",environments_dir=GAMES).make("su15-1944f8ab",seed=0,save_recording=False)
    env.reset()
    for l in (1,2):
        for a in json.load(open(f"{CERT}/su15_L{l}.json"))["upper_bound"]["route"]:
            env.step(GameAction.ACTION6,data={"x":a["x"],"y":a["y"]})
    return env
def est(g): return tuple(sorted((int(g.kqywaxhmsb[s]),int(s.x),int(s.y)) for s in g.lkujttxgs))
def act(g,x,y):
    """one ACTION6 via the public perform_action; returns (won, state)"""
    lv=g.level_index
    g.perform_action(ActionInput(id=GameAction.ACTION6,data={"x":x,"y":y}))
    return (g.level_index!=lv or g._state==GameState.WIN), g._state
def undo(g):
    g.perform_action(ActionInput(id=GameAction.ACTION7,data={}))
def set_state(g,pieces):
    """inject an arbitrary fruit configuration through the engine's own undo-restore path"""
    g.nscnqkkvg=[[("zmlxwcvwb",int(v),int(x),int(y)) for v,x,y in pieces]]
    g.vczehveskr()
def fast_action(g, aid, x=0, y=0):
    """same loop as ARCBaseGame.perform_action minus frame rendering (checked equal to perform_action)"""
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
def lean_copy(g):
    memo={id(g._camera):g._camera, id(g._clean_levels):g._clean_levels, id(g.actions):g.actions}
    for lv in g._levels:
        if lv is not g.current_level: memo[id(lv)]=lv
    memo[id(g._levels)]=None
    del memo[id(g._levels)]
    return copy.deepcopy(g,memo)
def reset_budget(g):
    c=g.step_counter_ui; c.current_steps=c.ccgddjelir; c.penalty=0
    if g._state==GameState.GAME_OVER: g._state=GameState.NOT_FINISHED
def fmt(st): return f"{len(st)} "+" ".join(f"{v} {x} {y}" for v,x,y in st)
