#include "simp2.h"
int sim_click(int n,int*arr,int cx,int cy,int*out){ static int inited=0; if(!inited){sim_init();inited=1;}
  St s; s.n=n; for(int i=0;i<n;i++){s.v[i]=arr[3*i];s.x[i]=arr[3*i+1];s.y[i]=arr[3*i+2];} canon(&s);
  St o; int lossy; int r=click(&s,cx,cy,&o,&lossy);
  if(r==2||r==3){ out[0]=o.n; for(int i=0;i<o.n;i++){out[1+3*i]=o.v[i];out[2+3*i]=o.x[i];out[3+3*i]=o.y[i];} }
  else { out[0]=s.n; for(int i=0;i<s.n;i++){out[1+3*i]=s.v[i];out[2+3*i]=s.x[i];out[3+3*i]=s.y[i];} }
  return r; }
