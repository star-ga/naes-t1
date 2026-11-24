import ctypes, os
_L=ctypes.CDLL(os.path.join(os.path.dirname(os.path.abspath(__file__)),os.environ.get("SIMLIB","libsimc.so")))
_L.setup_l3()
A9=ctypes.c_int*9
def click(state,cx,cy):
    """state: tuple of sorted (v,x,y). returns (newstate,status,lossy) status in noop/fail/ok/win"""
    n=len(state); v=A9(*[f[0] for f in state]); x=A9(*[f[1] for f in state]); y=A9(*[f[2] for f in state])
    ov,ox,oy=A9(),A9(),A9(); on=ctypes.c_int(); ls=ctypes.c_int()
    r=_L.click_c(n,v,x,y,cx,cy,ov,ox,oy,ctypes.byref(on),ctypes.byref(ls))
    ns=tuple(sorted((ov[i],ox[i],oy[i]) for i in range(on.value)))
    return ns,('noop','fail','ok','win')[r],bool(ls.value)
