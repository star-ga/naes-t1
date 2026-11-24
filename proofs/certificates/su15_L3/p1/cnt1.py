import sys; sys.path.insert(0,'.')
import csim, sim
st=sim.L3_START; a=set(); b=set(); st2={}
for cx in range(64):
    for cy in range(64):
        ns,s,l=csim.click(st,cx,cy); ns2,s2=sim.click(st,cx,cy); st2[s2]=st2.get(s2,0)+1
        if s=='ok': a.add(ns)
        if s2=='ok': b.add(ns2)
        assert (s==s2) and ns==ns2,(cx,cy,s,s2)
print(len(a),len(b),st2)
