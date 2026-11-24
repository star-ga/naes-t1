// exact fruit-only su15 simulator (levels without enemies), port of sim.py
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct { uint8_t n, v[9], x[9], y[9]; } St;
static const int SZ[9]={1,2,3,4,5,7,8,9,10};
#define RNG 7
#define STEP 4
#define NSTEP 4
#define YMIN 10
#define YMAX 63
#define WD 64
typedef struct {int zx,zy,zw,zh;} Zone;
static Zone ZONES[4]; static int NZ=0;
static int GV[3],GC[3],NG=0;
static inline int ctrx(int v,int x){return x+SZ[v]/2;}
static inline int inrange(int cx,int cy,int v,int x,int y){
  int s=SZ[v];
  int nx = (cx>=x&&cx<=x+s-1)?cx:(cx<x?x:x+s-1);
  int ny = (cy>=y&&cy<=y+s-1)?cy:(cy<y?y:y+s-1);
  int dx=cx-nx,dy=cy-ny; return dx*dx+dy*dy<=RNG*RNG;
}
static inline void clampp(int v,int*x,int*y){
  int s=SZ[v];
  if(*x<0)*x=0; if(*x>WD-s)*x=WD-s;
  if(*y<YMIN)*y=YMIN; if(*y>YMAX)*y=YMAX; if(*y>WD-s)*y=WD-s;
}
static inline int ovl(int va,int xa,int ya,int vb,int xb,int yb){
  int sa=SZ[va],sb=SZ[vb];
  return !(xa+sa<=xb||xb+sb<=xa||ya+sa<=yb||yb+sb<=ya);
}
static void canon(St*s){ // insertion sort by (v,x,y)
  for(int i=1;i<s->n;i++){
    uint8_t v=s->v[i],x=s->x[i],y=s->y[i]; int j=i-1;
    while(j>=0 && (s->v[j]>v||(s->v[j]==v&&(s->x[j]>x||(s->x[j]==x&&s->y[j]>y))))){
      s->v[j+1]=s->v[j];s->x[j+1]=s->x[j];s->y[j+1]=s->y[j];j--; }
    s->v[j+1]=v;s->x[j+1]=x;s->y[j+1]=y;
  }
}
static int won(const St*s);
// returns 0 noop, 1 fail, 2 ok, 3 win (mid-flight or final) ; *lossy set if a group>=3 or v>=8 vanished
static int click(const St*s,int cx,int cy,St*o,int*lossy){
  *lossy=0;
  if(cy<=YMIN-1||cy>=YMAX) return 0;
  int idx[9],m=0;
  for(int i=0;i<s->n;i++) if(inrange(cx,cy,s->v[i],s->x[i],s->y[i])) idx[m++]=i;
  if(!m) return 0;
  uint8_t mv[9],mx[9],my[9];
  for(int k=0;k<m;k++){int i=idx[k]; mv[k]=s->v[i];mx[k]=s->x[i];my[k]=s->y[i];}
  for(int t=0;t<NSTEP;t++){
    for(int k=0;k<m;k++){
      int v=mv[k],x=mx[k],y=my[k];
      int c0=x+SZ[v]/2,c1=y+SZ[v]/2; int dx=cx-c0,dy=cy-c1;
      int ix=dx>0?(dx<STEP?dx:STEP):(dx<0?(dx>-STEP?dx:-STEP):0);
      int iy=dy>0?(dy<STEP?dy:STEP):(dy<0?(dy>-STEP?dy:-STEP):0);
      x+=ix;y+=iy; clampp(v,&x,&y); mx[k]=x;my[k]=y;
    }
#ifndef NO_MIDFLIGHT
    if(t<NSTEP-1){ // engine checks win after every movement step
      St w; w.n=0; char isin0[9]={0}; for(int k=0;k<m;k++)isin0[idx[k]]=1;
      for(int i=0;i<s->n;i++) if(!isin0[i]){w.v[w.n]=s->v[i];w.x[w.n]=s->x[i];w.y[w.n]=s->y[i];w.n++;}
      for(int k=0;k<m;k++){w.v[w.n]=mv[k];w.x[w.n]=mx[k];w.y[w.n]=my[k];w.n++;}
      if(won(&w)){ *o=w; return 3; }
    }
#endif
  }
  int par[9]; for(int k=0;k<m;k++)par[k]=k;
  for(int i=0;i<m;i++)for(int j=i+1;j<m;j++) if(ovl(mv[i],mx[i],my[i],mv[j],mx[j],my[j])){
    if(mv[i]!=mv[j]) return 1;
    int a=i;while(par[a]!=a)a=par[a]; int b=j;while(par[b]!=b)b=par[b];
    if(a!=b)par[b]=a; }
  o->n=0;
  char isin[9]={0}; for(int k=0;k<m;k++)isin[idx[k]]=1;
  for(int i=0;i<s->n;i++) if(!isin[i]){o->v[o->n]=s->v[i];o->x[o->n]=s->x[i];o->y[o->n]=s->y[i];o->n++;}
  int root[9]; for(int k=0;k<m;k++){int a=k;while(par[a]!=a)a=par[a];root[k]=a;}
  for(int r=0;r<m;r++){
    int cnt=0,sx=0,sy=0,v=-1;
    for(int k=0;k<m;k++) if(root[k]==r){cnt++; sx+=mx[k]+SZ[mv[k]]/2; sy+=my[k]+SZ[mv[k]]/2; v=mv[k];}
    if(!cnt)continue;
    if(cnt==1){ for(int k=0;k<m;k++) if(root[k]==r){o->v[o->n]=mv[k];o->x[o->n]=mx[k];o->y[o->n]=my[k];o->n++;} continue;}
    if(cnt>=3) *lossy=1;
    if(v>=8){*lossy=1;continue;}
    int nv=v+1,s2=SZ[nv]; int ax=sx/cnt-s2/2, ay=sy/cnt-s2/2; clampp(nv,&ax,&ay);
    o->v[o->n]=nv;o->x[o->n]=ax;o->y[o->n]=ay;o->n++;
  }
  canon(o); return won(o)?3:2;
}
static int won(const St*s){
  int cnt[9]={0};
  for(int i=0;i<s->n;i++){
    int c0=s->x[i]+SZ[s->v[i]]/2,c1=s->y[i]+SZ[s->v[i]]/2;
    for(int z=0;z<NZ;z++) if(c0>=ZONES[z].zx&&c0<ZONES[z].zx+ZONES[z].zw&&c1>=ZONES[z].zy&&c1<ZONES[z].zy+ZONES[z].zh){cnt[s->v[i]]++;break;}
  }
  for(int g=0;g<NG;g++) if(cnt[GV[g]]!=GC[g]) return 0;
  return 1;
}
static uint64_t hst(const St*s){
  uint64_t h=1469598103934665603ULL;
  for(int i=0;i<s->n;i++){ h^=(uint64_t)s->v[i]|((uint64_t)s->x[i]<<8)|((uint64_t)s->y[i]<<16)|(1ULL<<30); h*=1099511628211ULL; h^=h>>29;}
  h^=s->n; h*=0x9E3779B97F4A7C15ULL; h^=h>>32; return h;
}
static int cmpst(const St*a,const St*b){ if(a->n!=b->n) return a->n<b->n?-1:1; for(int i=0;i<a->n;i++){ if(a->v[i]!=b->v[i])return a->v[i]<b->v[i]?-1:1; if(a->x[i]!=b->x[i])return a->x[i]<b->x[i]?-1:1; if(a->y[i]!=b->y[i])return a->y[i]<b->y[i]?-1:1;} return 0;}

static void sim_init(void){ NZ=2; ZONES[0]=(Zone){5,46,9,9}; ZONES[1]=(Zone){19,46,9,9}; NG=2; GV[0]=3;GC[0]=1;GV[1]=2;GC[1]=1; }
