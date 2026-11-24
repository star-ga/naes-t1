# Exhaustive R2 with the OWN trajectory model of indep_dp.py (checked tick-by-tick vs the real engine there):
# for every value 0..3, every click P (rows 10..62), every valid position hit by P: the landing equals the ENGINE-derived
# FP[v][P] (one engine call per (v,P)) and the landed rectangle contains P (so all hit pieces pairwise overlap at tick 4).
import sys, numpy as np
sys.argv=[sys.argv[0]]
src=open('indep/indep_dp.py').read().split('# ---------- lineage DP ----------')[0]
src=src.replace('rng = random.Random(5); tc = 0; tbad = 0\nfor it in range(1500):','rng = random.Random(5); tc = 0; tbad = 0\nfor it in range(0):')
exec(compile(src,'indep_dp_head','exec'))
inst=0; bad=0; contain_bad=0
for v in range(4):
    for P in CLICKS:
        for x,y in np.argwhere(hitmask(P,v)):
            inst+=1; t=traj(v,int(x),int(y),P); fx,fy=t[-1]; s=SZ[v]
            if (fx,fy)!=FP[v][P]: bad+=1
            if not (fx<=P[0]<=fx+s-1 and fy<=P[1]<=fy+s-1): contain_bad+=1
print(f"R2_OWNMODEL instances={inst} landing_mismatch_vs_engine_FP={bad} landed_rect_not_containing_P={contain_bad}")
