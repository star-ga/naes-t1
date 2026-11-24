// Adversarial check: generate targeted engine test vectors (edge cases the proofs rely on). Output: "n v x y ... ; px py"
#include "rsim.h"
static unsigned long long rs=88172645463325252ULL; static unsigned rnd(){ rs^=rs<<13; rs^=rs>>7; rs^=rs<<17; return (unsigned)(rs>>11); }
static int okpos(int v,int x,int y){ int s=SZ[v]; return x>=0&&x<=64-s&&y>=10&&y<=63&&y<=64-s; }
static int d2(int px,int py,int v,int x,int y){ int s=SZ[v]; int nx=px<x?x:(px>x+s-1?x+s-1:px), ny=py<y?y:(py>y+s-1?y+s-1:py); return (px-nx)*(px-nx)+(py-ny)*(py-ny); }
static void pr(int n,const int*v,const int*x,const int*y,int px,int py){ printf("%d",n); for(int i=0;i<n;i++) printf(" %d %d %d",v[i],x[i],y[i]); printf(" ; %d %d\n",px,py); }
static int hits(int v,int px,int py,int*X,int*Y){ int n=0; for(int y=10;y<64;y++) for(int x=0;x<64;x++) if(okpos(v,x,y)&&d2(px,py,v,x,y)<=64){X[n]=x;Y[n]=y;n++;} return n; }
int main(int argc,char**argv){ const char*sec=argc>1?argv[1]:"all"; int X[4096],Y[4096];
 if(!strcmp(sec,"A")||!strcmp(sec,"all")){ // single piece: farthest hit, nearest miss, random hit; all P rows 9..63
  for(int v=0;v<4;v++) for(int py=9;py<=63;py++) for(int px=0;px<64;px++){
    int bx=-1,by=-1,bd=-1, mx=-1,my=-1,md=1<<30, nh=0;
    for(int y=10;y<64;y++) for(int x=0;x<64;x++){ if(!okpos(v,x,y)) continue; int d=d2(px,py,v,x,y);
      if(d<=64){ nh++; if(d>bd||(d==bd&&(rnd()&1))){bd=d;bx=x;by=y;} } else if(d<md||(d==md&&(rnd()&1))){md=d;mx=x;my=y;} }
    int vv[1]={v}; if(bx>=0){int xx[1]={bx},yy[1]={by}; pr(1,vv,xx,yy,px,py);} if(mx>=0){int xx[1]={mx},yy[1]={my}; pr(1,vv,xx,yy,px,py);}
    if(py>=10&&py<=62){ int n=hits(v,px,py,X,Y); if(n){ int k=rnd()%n; int xx[1]={X[k]},yy[1]={Y[k]}; pr(1,vv,xx,yy,px,py);} } } }
 if(!strcmp(sec,"B")||!strcmp(sec,"all")){ // merges: pair at two mutually far hit positions for every P, v=0..2; 3- and 4-groups
  for(int v=0;v<3;v++) for(int py=10;py<=62;py++) for(int px=0;px<64;px++){ int n=hits(v,px,py,X,Y); if(n<2) continue;
    int a=rnd()%n,b=-1,bd=-1; for(int k=0;k<n;k++){ int d=(X[k]-X[a])*(X[k]-X[a])+(Y[k]-Y[a])*(Y[k]-Y[a]); if(d>bd){bd=d;b=k;} }
    int vv[2]={v,v},xx[2]={X[a],X[b]},yy[2]={Y[a],Y[b]}; pr(2,vv,xx,yy,px,py); }
  for(int g=3;g<=4;g++) for(int v=0;v<3;v++) for(int r=0;r<300;r++){ int px=rnd()%64,py=10+rnd()%53; int n=hits(v,px,py,X,Y); if(n<g) continue;
    int vv[4],xx[4],yy[4]; for(int i=0;i<g;i++){ int k=rnd()%n; vv[i]=v;xx[i]=X[k];yy[i]=Y[k]; } pr(g,vv,xx,yy,px,py); } }
 if(!strcmp(sec,"C")||!strcmp(sec,"all")){ // mixed values hit together (collision), incl. at borders
  for(int va=0;va<4;va++) for(int vb=va+1;vb<4;vb++) for(int r=0;r<400;r++){ int px,py; if(r<100){ px=(r&1)?0:63; py=10+rnd()%53; } else if(r<200){ py=(r&1)?10:62; px=rnd()%64; } else { px=rnd()%64; py=10+rnd()%53; }
    int na=hits(va,px,py,X,Y); if(!na) continue; int ka=rnd()%na; int ax=X[ka],ay=Y[ka]; int nb=hits(vb,px,py,X,Y); if(!nb) continue; int kb=rnd()%nb;
    int vv[2]={va,vb},xx[2]={ax,X[kb]},yy[2]={ay,Y[kb]}; pr(2,vv,xx,yy,px,py); } }
 if(!strcmp(sec,"D")||!strcmp(sec,"all")){ // joint final click: {v3,v2} pre-states, wins at tick 1/2/3, single-hit pass-through, near misses
  int want[8]={700,700,700,500,500,300,300,900}, got[8]={0}; // 0..2 both-hit win tick1..3, 3 single v3 midflight, 4 single v2 midflight, 5/6 single landing in zone, 7 near miss
  for(int it=0; it<4000000; it++){ int px=rnd()%64, py=10+rnd()%53; if(py<30) py=30+rnd()%33;
    int a3x=rnd()%61, a3y=10+rnd()%51, b2x=rnd()%62, b2y=10+rnd()%52; int mode=rnd()%3;
    if(mode==1){ b2x=4+rnd()%9+( (rnd()&1)?14:0 ); b2y=45+rnd()%9; if(!okpos(2,b2x,b2y)) continue; }
    if(mode==2){ a3x=3+rnd()%9+((rnd()&1)?14:0); a3y=44+rnd()%9; if(!okpos(3,a3x,a3y)) continue; }
    int h3=d2(px,py,3,a3x,a3y)<=64, h2=d2(px,py,2,b2x,b2y)<=64; if(!h3&&!h2) continue;
    S s; s.n=2; s.v[0]=3;s.x[0]=a3x;s.y[0]=a3y; s.v[1]=2;s.x[1]=b2x;s.y[1]=b2y; if(won(&s)) continue;
    S o; int wt=0; int r=rclick(&s,px,py,&o,&wt); int cls=-1;
    if(h3&&h2){ if(r==R_WIN&&wt<=3) cls=wt-1; else { // near miss: each in zone at some tick 1..3 but never together
        int x3=a3x,y3=a3y,x2=b2x,y2=b2y,z3=0,z2=0; for(int t=1;t<=3;t++){ step1(px,py,3,&x3,&y3); step1(px,py,2,&x2,&y2); z3|=inzone(cen(3,x3),cen(3,y3)); z2|=inzone(cen(2,x2),cen(2,y2)); } if(z3&&z2) cls=7; } }
    else if(h3 && r==R_WIN) cls= wt<=3?3:5; else if(h2 && r==R_WIN) cls= wt<=3?4:6;
    if(cls<0||got[cls]>=want[cls]) continue; got[cls]++; int vv[2]={3,2},xx[2]={a3x,b2x},yy[2]={a3y,b2y}; pr(2,vv,xx,yy,px,py); }
  fprintf(stderr,"D got"); for(int i=0;i<8;i++) fprintf(stderr," %d",got[i]); fprintf(stderr,"\n"); }
 if(!strcmp(sec,"E")||!strcmp(sec,"all")){ S s; rstart(&s); for(int py=0;py<64;py++) for(int px=0;px<64;px++) pr(9,s.v,s.x,s.y,px,py); }
 return 0; }
