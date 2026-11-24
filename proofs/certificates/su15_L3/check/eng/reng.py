# Check engine runner. Real engine, offline.
# input lines: "n v x y v x y ... ; px py"   output: same line + " => RES" with RES in {WIN, NOOP_SAME, n v x y ...}
import sys,json,copy,hashlib,time,os
import arc_agi
from arcengine.enums import GameAction, GameState
from arcengine import ActionInput
GAMES="games"
SHA="a5f91f7c963d6ca6447dae0ab21342b48a3f511601c40dfa8e972bdc59b4651e"
CERT="certificates"
def level3_game():
    assert hashlib.sha256(open(GAMES+"/su15/1944f8ab/su15.py","rb").read()).hexdigest()==SHA
    env=arc_agi.Arcade(operation_mode="offline",environments_dir=GAMES).make("su15-1944f8ab",seed=0,save_recording=False)
    fd=env.reset()
    for l in (1,2):
        for a in json.load(open(f"{CERT}/su15_L{l}.json"))["upper_bound"]["route"]:
            fd=env.step(GameAction.ACTION6,data={"x":a["x"],"y":a["y"]})
    assert fd.levels_completed==2
    return env._game
def copyg(g):
    memo={id(g._camera):g._camera, id(g._clean_levels):g._clean_levels, id(g.actions):g.actions}
    for lv in g._levels:
        if lv is not g.current_level: memo[id(lv)]=lv
    return copy.deepcopy(g,memo)
def est(g): return sorted((int(g.kqywaxhmsb[s]),int(s.x),int(s.y)) for s in g.lkujttxgs)
def inject(g,pieces):
    # engine's own restore path (vczehveskr pops a snapshot); then history emptied, budget reset
    g.nscnqkkvg=[[("zmlxwcvwb",int(v),int(x),int(y)) for v,x,y in pieces]]
    g.vczehveskr()
    c=g.step_counter_ui; c.current_steps=c.ccgddjelir; c.penalty=0
def do(g,aid,x=0,y=0):
    # public entry point (perform_action), same as env.step minus frame return
    g.perform_action(ActionInput(id=aid,data=({"x":x,"y":y} if aid==GameAction.ACTION6 else {})))
if __name__=="__main__":
    g0=level3_game(); print("START",est(g0),file=sys.stderr)
    g=copyg(g0); lv=g.level_index; t0=time.time(); n=0
    out=open(sys.argv[2],"w")
    for line in open(sys.argv[1]):
        line=line.strip()
        if not line or line.startswith("#"): continue
        a,b=line.split(";"); t=list(map(int,a.split())); px,py=map(int,b.split())
        k=t[0]; pcs=[(t[1+3*i],t[2+3*i],t[3+3*i]) for i in range(k)]
        if g.level_index!=lv or g._state!=GameState.NOT_FINISHED: g=copyg(g0)
        inject(g,pcs); assert est(g)==sorted(pcs),(est(g),pcs)
        do(g,GameAction.ACTION6,px,py); n+=1
        if g.level_index!=lv or g._state==GameState.WIN: res="WIN"
        elif g._state==GameState.GAME_OVER: res="GAMEOVER"
        else:
            e=est(g); res=f"{len(e)} "+" ".join(f"{v} {x} {y}" for v,x,y in e)
        out.write(f"{line} => {res}\n")
    out.close(); print(f"RENG done n={n} sec={time.time()-t0:.0f}",file=sys.stderr)
