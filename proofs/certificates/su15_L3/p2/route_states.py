import sys,ctypes
lib=ctypes.CDLL("./libsim.so"); lib.sim_click.argtypes=[ctypes.c_int,ctypes.POINTER(ctypes.c_int),ctypes.c_int,ctypes.c_int,ctypes.POINTER(ctypes.c_int)]
def click(st,cx,cy):
    a=(ctypes.c_int*(3*len(st)))(*[t for p in st for t in p]); o=(ctypes.c_int*30)()
    r=lib.sim_click(len(st),a,cx,cy,o); n=o[0]; return r,tuple(sorted((o[1+3*i],o[2+3*i],o[3+3*i]) for i in range(n)))
START=tuple(sorted([(0,55,23),(0,61,23),(0,31,22),(0,31,15),(0,12,23),(0,8,28),(1,46,22),(1,30,32),(1,18,16)]))
routes=["55,28 47,31 14,24 34,22 33,30 22,24 40,36 27,31 23,39 32,42 23,46","55,28 46,28 12,31 31,23 27,30 18,25 19,34 17,43 39,34 32,40 25,46"]
out=open(sys.argv[1],"w")
for ri,rt in enumerate(routes):
    st=START; sts=[st]
    for i,t in enumerate(rt.split()):
        x,y=map(int,t.split(",")); r,st2=click(st,x,y)
        print("route",ri,"click",i+1,t,"status",r,"pieces",len(st2))
        if r==3: break
        assert r==2; st=st2; sts.append(st)
    for s in sts: out.write(f"{len(s)} "+" ".join(f"{v} {x} {y}" for v,x,y in s)+"\n")
