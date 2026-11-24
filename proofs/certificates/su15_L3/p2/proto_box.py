# prototype box-DP lower bound at root
import itertools,sys
SZ=[1,2,3,4,5,7,8,9,10]
def clampx(s,x): return max(0,min(x,64-s))
def clampy(s,y): return max(10,min(y,63,64-s))
def prange(s,lo,hi,axis):  # P-range in click coords for piece top-left interval
    a=lo-8; b=hi+s-1+8
    if axis==0: return max(0,a),min(63,b)
    return max(10,a),min(62,b)
def land(s,P,axis):  # top-left from P for size s
    return clampx(s,P-s//2) if axis==0 else clampy(s,P-s//2)
def move_box(v,B):
    s=SZ[v]; out=[]
    for ax in (0,1):
        plo,phi=prange(s,B[2*ax],B[2*ax+1],ax)
        out+= [land(s,plo,ax),land(s,phi,ax)]
    return tuple(out)
def merge_box(u,A,B):  # children value u, result value u+1
    s=SZ[u]; s2=SZ[u+1]; out=[]
    for ax in (0,1):
        a=prange(s,A[2*ax],A[2*ax+1],ax); b=prange(s,B[2*ax],B[2*ax+1],ax)
        lo=max(a[0],b[0]); hi=min(a[1],b[1])
        if lo>hi: return None
        def f(P):
            c=land(s,P,ax)+s//2
            x=c-s2//2
            return clampx(s2,x) if ax==0 else clampy(s2,x)
        out+=[f(lo),f(hi)]
    return tuple(out)
def hull(A,B):
    if A is None: return B
    if B is None: return A
    return (min(A[0],B[0]),max(A[1],B[1]),min(A[2],B[2]),max(A[3],B[3]))
def solve(pieces,K,zones,joint=True):
    n=len(pieces)
    vm={}
    for m in range(1,1<<n):
        mass=sum(1<<pieces[i][0] for i in range(n) if m>>i&1)
        if mass&(mass-1)==0: vm[m]=mass.bit_length()-1
    masks=sorted(vm,key=lambda m:bin(m).count('1'))
    B={}
    for m in masks:
        v=vm[m]
        arr=[None]*(K+1)
        if bin(m).count('1')==1:
            i=m.bit_length()-1; _,x,y=pieces[i]; arr[0]=(x,x,y,y)
        else:
            if v==0: continue
        for k in range(0,K+1):
            cur=arr[k]
            if k>=1:
                if arr[k-1] is not None: cur=hull(cur,move_box(v,arr[k-1]))
                if bin(m).count('1')>1:
                    sub=(m-1)&m
                    while sub:
                        o=m^sub
                        if sub>o and vm.get(sub)==v-1 and vm.get(o)==v-1:
                            for k1 in range(0,k):
                                k2=k-1-k1
                                A=B[sub][k1];C=B[o][k2]
                                if A is None or C is None: continue
                                mb=merge_box(v-1,A,C)
                                cur=hull(cur,mb)
                        sub=(sub-1)&m
            arr[k]=cur
        B[m]=arr
    full=(1<<n)-1
    best=99
    # zone-centre box check: piece of size s top-left in zone iff centre in zone
    def inz_box(v,Bx):
        s=SZ[v]
        for (zx,zy,zw,zh) in zones:
            # centre = x+s//2 in [zx,zx+zw-1]
            if Bx[0]+s//2<=zx+zw-1 and Bx[1]+s//2>=zx and Bx[2]+s//2<=zy+zh-1 and Bx[3]+s//2>=zy: return True
        return False
    def inz_exp(v,Bx,e):
        s=SZ[v]
        for (zx,zy,zw,zh) in zones:
            if Bx[0]+s//2<=zx+zw-1+e and Bx[1]+s//2>=zx-e and Bx[2]+s//2<=zy+zh-1+e and Bx[3]+s//2>=zy-e: return True
        return False
    parts=[]
    for T3 in masks:
        if vm[T3]!=3: continue
        T2=full^T3
        if T2 not in vm or vm[T2]!=2: continue
        c3=next((k for k in range(K+1) if B[T3][k] is not None and inz_box(3,B[T3][k])),99)
        c2=next((k for k in range(K+1) if B[T2][k] is not None and inz_box(2,B[T2][k])),99)
        tot=c3+c2
        if joint:
            # pre-final boxes: within expanded zone, then +1 click
            Ex=8+2+1
            j3=next((k for k in range(K+1) if B[T3][k] is not None and inz_exp(3,B[T3][k],Ex)),99)
            j2=next((k for k in range(K+1) if B[T2][k] is not None and inz_exp(2,B[T2][k],Ex)),99)
            tot=min(tot,j3+j2+1)
        best=min(best,tot)
    return best
if __name__=="__main__":
    st=[(0,55,23),(0,61,23),(0,31,22),(0,31,15),(0,12,23),(0,8,28),(1,46,22),(1,30,32),(1,18,16)]
    z=[(5,46,9,9),(19,46,9,9)]
    print(solve(st,14,z,False),solve(st,14,z,True))
