// count {v3,v2} both-hit click events and their wins (incl. already-won pre-states) - cross-check of p1 FP / p2 R4 counts
#include "rsim.h"
static int okpos(int v,int x,int y){ int s=SZ[v]; return x>=0&&x<=64-s&&y>=10&&y<=63&&y<=64-s; }
int main(){ long pairs=0,wins=0,wins_prewon=0,fails=0;
 static int AX[4096],AY[4096],BX[4096],BY[4096];
 for(int py=10;py<=62;py++) for(int px=0;px<64;px++){ int na=0,nb=0;
  for(int y=10;y<64;y++) for(int x=0;x<64;x++){ if(okpos(3,x,y)&&hit(px,py,3,x,y)){AX[na]=x;AY[na]=y;na++;} if(okpos(2,x,y)&&hit(px,py,2,x,y)){BX[nb]=x;BY[nb]=y;nb++;} }
  for(int i=0;i<na;i++) for(int j=0;j<nb;j++){ S s; s.n=2; s.v[0]=3;s.x[0]=AX[i];s.y[0]=AY[i]; s.v[1]=2;s.x[1]=BX[j];s.y[1]=BY[j]; S o; int wt; int r=rclick(&s,px,py,&o,&wt); pairs++;
    if(r==R_WIN){ wins++; if(won(&s)) wins_prewon++; } else if(r==R_FAIL) fails++; } }
 printf("RFPCOUNT both_hit_events=%ld wins=%ld (of which pre-state already won=%ld) fails=%ld\n",pairs,wins,wins_prewon,fails); return 0; }
