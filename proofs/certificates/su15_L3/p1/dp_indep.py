# Independent re-implementation (numpy, different algorithm: dense min-plus over a precomputed range matrix) of the relaxed lineage DP.
# Compares EVERY entry of every table D[T][pos] with the C tables dumped by h_dump.c.
import numpy as np, struct, sys
SZ=[1,2,3,4]; INF=255
f=open('logs/D_root.bin','rb'); n=struct.unpack('i',f.read(4))[0]
pieces=[struct.unpack('iii',f.read(12)) for _ in range(n)]
Cd={}
while True:
    b=f.read(4)
    if not b: break
    m=struct.unpack('i',b)[0]; Cd[m]=np.frombuffer(f.read(4096),dtype=np.uint8).copy()
P=[(cx,cy) for cx in range(64) for cy in range(64)]            # index cx*64+cy
Pcx=np.array([p[0] for p in P]); Pcy=np.array([p[1] for p in P])
clickok=(Pcy>=10)&(Pcy<=62)                                    # engine: click rows 10..62
R={}; valid={}; land={}
for v in range(4):
    s=SZ[v]
    qx=np.array([p[0] for p in P]); qy=np.array([p[1] for p in P])
    valid[v]=(qx<=64-s)&(qy>=10)&(qy<=63)&(qy<=64-s)
    # squared distance from click point to the s x s block with top-left q
    dx=np.maximum(np.maximum(qx[None,:]-Pcx[:,None],Pcx[:,None]-(qx[None,:]+s-1)),0)
    dy=np.maximum(np.maximum(qy[None,:]-Pcy[:,None],Pcy[:,None]-(qy[None,:]+s-1)),0)
    R[v]=((dx*dx+dy*dy)<=64)&clickok[:,None]&valid[v][None,:]
    lx=np.clip(Pcx-s//2,0,64-s); ly=np.minimum(np.minimum(np.maximum(Pcy-s//2,10),63),64-s)
    land[v]=lx*64+ly
def bestP(Dt,v):  # min over in-range valid q of D[q], for every click P
    out=np.full(4096,INF,dtype=np.int32); 
    for i in range(0,4096,256):
        sub=np.where(R[v][i:i+256],Dt[None,:].astype(np.int32),INF); out[i:i+256]=sub.min(axis=1)
    return out
M=1<<n; mass={m:sum(1<<pieces[i][0] for i in range(n) if m>>i&1) for m in range(1,M)}
vm={m:(mass[m].bit_length()-1) for m in mass if mass[m]&(mass[m]-1)==0}
D={}
order=sorted(vm,key=lambda m:bin(m).count('1'))
for T in order:
    v=vm[T]; Dt=np.full(4096,INF,dtype=np.int32)
    if bin(T).count('1')==1:
        i=T.bit_length()-1; Dt[pieces[i][1]*64+pieces[i][2]]=0
    elif v>0:
        pv=v-1; sub=(T-1)&T
        while sub>0:
            T1=sub; T2=T^T1; sub=(sub-1)&T
            if T1<T2 or vm.get(T1)!=pv or vm.get(T2)!=pv: continue
            b1=bestP(D[T1],pv); b2=bestP(D[T2],pv); tot=b1+b2+1; ok=(b1<INF)&(b2<INF)
            # merged piece: centre of the landed (v-1) piece, clamped as value v
            lp=land[pv]; lcx=lp//64+SZ[pv]//2; lcy=lp%64+SZ[pv]//2
            ax=np.clip(lcx-SZ[v]//2,0,64-SZ[v]); ay=np.minimum(np.minimum(np.maximum(lcy-SZ[v]//2,10),63),64-SZ[v])
            pos=ax*64+ay
            np.minimum.at(Dt,pos[ok],tot[ok])
    while True:  # moves to fixpoint
        b=bestP(Dt,v); ok=b<INF-1; new=Dt.copy(); np.minimum.at(new,land[v][ok],(b+1)[ok])
        if (new==Dt).all(): break
        Dt=new
    D[T]=np.minimum(Dt,INF)
bad=0; ntab=0; nent=0
for T in D:
    ntab+=1; c=Cd[T].astype(np.int32); d=D[T]; diff=(c!=d); nent+=4096
    if diff.any(): bad+=int(diff.sum()); print("DIFF table",hex(T),int(diff.sum()))
print("DP_INDEP tables",ntab,"c_tables",len(Cd),"entries_compared",nent,"mismatches",bad,"tables_equal_sets",set(D)==set(Cd))

# ---- independent evaluation of the final-click rules on the independent tables ----
ZONES=[(5,46,9,9),(19,46,9,9)]
def inzone(v,x,y):
    c0=x+SZ[v]//2; c1=y+SZ[v]//2; r=np.zeros(x.shape,bool)
    for zx,zy,zw,zh in ZONES: r|=(c0>=zx)&(c0<zx+zw)&(c1>=zy)&(c1<zy+zh)
    return r
qx=np.array([p[0] for p in P]); qy=np.array([p[1] for p in P])
def tickflags(v):  # [3][4096 P][4096 q] in-zone flags after tick 1..3 of a piece (top-left q) pulled toward P (full step rule incl. clamp)
    s=SZ[v]; X=np.broadcast_to(qx[None,:],(4096,4096)).copy(); Y=np.broadcast_to(qy[None,:],(4096,4096)).copy(); out=[]
    for t in range(3):
        dx=Pcx[:,None]-(X+s//2); dy=Pcy[:,None]-(Y+s//2)
        X=X+np.clip(dx,-4,4); Y=Y+np.clip(dy,-4,4)
        X=np.clip(X,0,64-s); Y=np.minimum(np.minimum(np.maximum(Y,10),63),64-s)
        out.append(inzone(v,X,Y))
    return out
TF={v:tickflags(v) for v in (2,3)}
def minq(Dt,v,flag):  # min D[q] over valid in-range q (optionally with tick flag) per P
    out=np.full(4096,INF,dtype=np.int32)
    for i in range(0,4096,256):
        m=R[v][i:i+256] if flag is None else (R[v][i:i+256]&flag[i:i+256])
        out[i:i+256]=np.where(m,Dt[None,:].astype(np.int32),INF).min(axis=1)
    return out
def zonemin(Dt,v):
    x=qx; y=qy; ok=valid[v]&inzone(v,x,y); return int(np.where(ok,Dt,INF).min())
full=M-1; res={'F0':999,'F1':999,'F2':999}
for T3 in D:
    if vm[T3]!=3: continue
    T2=full^T3
    if T2 not in D or vm[T2]!=2: continue
    c3=zonemin(D[T3],3); c2=zonemin(D[T2],2)
    if c3<INF and c2<INF: res['F0']=min(res['F0'],c3+c2)
    for (Tm,vv,oth) in ((T3,3,c2),(T2,2,c3)):
        if oth>=INF: continue
        best=min(int(minq(D[Tm],vv,TF[vv][t]).min()) for t in range(3))
        if best<INF: res['F2']=min(res['F2'],best+1+oth)
    for t in range(3):
        a=minq(D[T3],3,TF[3][t]); b=minq(D[T2],2,TF[2][t]); tot=(a+b+1)[(a<INF)&(b<INF)]
        if tot.size: res['F1']=min(res['F1'],int(tot.min()))
print("H_INDEP F0(end-state)",res['F0'],"F1(pair mid-flight)",res['F1'],"F2(single mid-flight)",res['F2'],"h=min",min(res.values()))
