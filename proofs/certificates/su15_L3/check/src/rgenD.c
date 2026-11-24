// Adversarial check: joint-final-click vectors by exhaustive enumeration + reservoir sampling per class
#include "rsim.h"
static unsigned long long rs=1234567ULL; static unsigned long long rnd(){ rs^=rs<<13; rs^=rs>>7; rs^=rs<<17; return rs; }
static int okpos(int v,int x,int y){ int s=SZ[v]; return x>=0&&x<=64-s&&y>=10&&y<=63&&y<=64-s; }
#define NC 10
static const char*CN[NC]={"both_t1","both_t2","both_t3","both_nearmiss","single3_mid","single3_t4","single2_mid","single2_t4","single3_nowin_otherinzone","single2_nowin_otherinzone"};
static int Q=600; static int cnt[NC]; static long seen[NC]; static int V[NC][600][6];
static void add(int c,int a3x,int a3y,int b2x,int b2y,int px,int py){ seen[c]++; int k; if(cnt[c]<Q) k=cnt[c]++; else { long r=rnd()%seen[c]; if(r>=Q) return; k=r; }
  int*t=V[c][k]; t[0]=a3x;t[1]=a3y;t[2]=b2x;t[3]=b2y;t[4]=px;t[5]=py; }
int main(){ static int AX[4096],AY[4096],BX[4096],BY[4096];
 for(int py=10;py<=62;py++) for(int px=0;px<64;px++){ int na=0,nb=0;
  for(int y=10;y<64;y++) for(int x=0;x<64;x++){ if(okpos(3,x,y)&&hit(px,py,3,x,y)){AX[na]=x;AY[na]=y;na++;} if(okpos(2,x,y)&&hit(px,py,2,x,y)){BX[nb]=x;BY[nb]=y;nb++;} }
  // per-start trajectories zone flags
  static int z3[4096][5], z2[4096][5];
  for(int i=0;i<na;i++){ int x=AX[i],y=AY[i]; z3[i][0]=inzone(cen(3,x),cen(3,y)); for(int t=1;t<=4;t++){ step1(px,py,3,&x,&y); z3[i][t]=inzone(cen(3,x),cen(3,y)); } }
  for(int i=0;i<nb;i++){ int x=BX[i],y=BY[i]; z2[i][0]=inzone(cen(2,x),cen(2,y)); for(int t=1;t<=4;t++){ step1(px,py,2,&x,&y); z2[i][t]=inzone(cen(2,x),cen(2,y)); } }
  int any3=0,any2=0; for(int i=0;i<na;i++) for(int t=1;t<=3;t++) any3|=z3[i][t]; for(int i=0;i<nb;i++) for(int t=1;t<=3;t++) any2|=z2[i][t];
  if(any3&&any2) for(int i=0;i<na;i++){ int m3=z3[i][1]|z3[i][2]|z3[i][3]; if(!m3) continue; for(int j=0;j<nb;j++){ if(z3[i][0]&&z2[j][0]) continue; // pre-state already won: excluded (cannot occur on a route)
      int w=0; for(int t=1;t<=3;t++) if(z3[i][t]&&z2[j][t]){w=t;break;}
      if(w) add(w-1,AX[i],AY[i],BX[j],BY[j],px,py); else if(z2[j][1]|z2[j][2]|z2[j][3]) add(3,AX[i],AY[i],BX[j],BY[j],px,py); } }
  // single hit: other piece in zone and NOT hit
  for(int y=10;y<64;y++) for(int x=0;x<64;x++){
    if(okpos(2,x,y)&&inzone(cen(2,x),cen(2,y))&&!hit(px,py,2,x,y)) for(int i=0;i<na;i++){ if(z3[i][0]) continue; int m=z3[i][1]|z3[i][2]|z3[i][3]; if(m) add(4,AX[i],AY[i],x,y,px,py); else if(z3[i][4]) add(5,AX[i],AY[i],x,y,px,py); else if((rnd()&63)==0) add(8,AX[i],AY[i],x,y,px,py); }
    if(okpos(3,x,y)&&inzone(cen(3,x),cen(3,y))&&!hit(px,py,3,x,y)) for(int j=0;j<nb;j++){ if(z2[j][0]) continue; int m=z2[j][1]|z2[j][2]|z2[j][3]; if(m) add(6,x,y,BX[j],BY[j],px,py); else if(z2[j][4]) add(7,x,y,BX[j],BY[j],px,py); else if((rnd()&63)==0) add(9,x,y,BX[j],BY[j],px,py); }
  } }
 for(int c=0;c<NC;c++){ fprintf(stderr,"class %s seen=%ld kept=%d\n",CN[c],seen[c],cnt[c]); for(int k=0;k<cnt[c];k++){ int*t=V[c][k]; printf("2 3 %d %d 2 %d %d ; %d %d\n",t[0],t[1],t[2],t[3],t[4],t[5]); } }
 return 0; }
