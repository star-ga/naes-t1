#include "simc.h"
// ctypes wrapper around the exact simulator. in: n pieces (v,x,y arrays), click (cx,cy). out: new pieces, lossy flag, return status 0 noop 1 fail 2 ok 3 win
void setup_l3(void){ NZ=2; ZONES[0]=(Zone){5,46,9,9}; ZONES[1]=(Zone){19,46,9,9}; NG=2; GV[0]=3;GC[0]=1;GV[1]=2;GC[1]=1; }
void setup_l3_nomidflight_marker(void){}
int click_c(int n,const int*v,const int*x,const int*y,int cx,int cy,int*ov,int*ox,int*oy,int*on,int*lossy){
  St s; s.n=n; for(int i=0;i<n;i++){s.v[i]=v[i];s.x[i]=x[i];s.y[i]=y[i];} canon(&s);
  St o; int r=click(&s,cx,cy,&o,lossy); if(r==0||r==1){o=s;} *on=o.n; for(int i=0;i<o.n;i++){ov[i]=o.v[i];ox[i]=o.x[i];oy[i]=o.y[i];} return r; }
