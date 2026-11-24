#include "rsim.h"
// flat interface for ctypes: in arrays v,x,y of length n; out arrays; returns code; *on, *wt
int rclick_flat(int n,const int*v,const int*x,const int*y,int px,int py,int*on,int*ov,int*ox,int*oy,int*wt){
  S s; s.n=n; for(int i=0;i<n;i++){s.v[i]=v[i];s.x[i]=x[i];s.y[i]=y[i];} S o; int r=rclick(&s,px,py,&o,wt);
  *on=o.n; for(int i=0;i<o.n;i++){ov[i]=o.v[i];ox[i]=o.x[i];oy[i]=o.y[i];} return r; }
