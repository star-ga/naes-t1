// The adversarial check's independent simulator of su15 level 3 (no enemies), written from su15.py
// (sha256 a5f91f7c...4651e) by the adversarial check. Not derived from the p1/p2 code.
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
static const int SZ[9]={1,2,3,4,5,7,8,9,10};
typedef struct { int n; int v[16], x[16], y[16]; } S;
#ifndef NOMID
#define MIDWIN 1
#else
#define MIDWIN 0
#endif
// zones of level 3: sprites 0008hngjwqibfi (9x9) at (5,46) and (19,46); test = center pixel inside bbox
static inline int inzone(int cx,int cy){ return cy>=46 && cy<55 && ((cx>=5 && cx<14) || (cx>=19 && cx<28)); }
static inline int cen(int v,int x){ return x+SZ[v]/2; }
static inline int hit(int px,int py,int v,int x,int y){ // kcqeohsztd with radius 8
  int s=SZ[v]; int nx = px<x? x : (px>x+s-1? x+s-1 : px); int ny = py<y? y : (py>y+s-1? y+s-1 : py);
  int dx=px-nx, dy=py-ny; return dx*dx+dy*dy<=64; }
static inline void clampxy(int v,int*x,int*y){ int s=SZ[v]; if(*x<0)*x=0; if(*x>64-s)*x=64-s; if(*y<10)*y=10; if(*y>63)*y=63; if(*y>64-s)*y=64-s; }
static inline void step1(int px,int py,int v,int*x,int*y){ // one tick of pkrdtzfrth (non-enemy branch)
  int s=SZ[v]; int dx=px-(*x+s/2), dy=py-(*y+s/2);
  int ix = dx>0 ? (dx<4?dx:4) : (dx<0 ? (dx>-4?dx:-4) : 0);
  int iy = dy>0 ? (dy<4?dy:4) : (dy<0 ? (dy>-4?dy:-4) : 0);
  int nx=*x+ix, ny=*y+iy; clampxy(v,&nx,&ny); *x=nx; *y=ny; }
static int won(const S*s){ // cbdhpcilgb for goal [[3,1],[2,1]]
  int c3=0,c2=0; for(int i=0;i<s->n;i++){ if(inzone(cen(s->v[i],s->x[i]),cen(s->v[i],s->y[i]))){ if(s->v[i]==3)c3++; else if(s->v[i]==2)c2++; } }
  return c3==1 && c2==1; }
static inline int ovl(int va,int xa,int ya,int vb,int xb,int yb){ int a=SZ[va],b=SZ[vb]; return !(xa+a<=xb || xb+b<=xa || ya+a<=yb || yb+b<=ya); }
enum { R_NOOP=0, R_FAIL=1, R_OK=2, R_WIN=3 };
// click: px,py grid coords 0..63. out = new state (unchanged copy for NOOP/FAIL). *wtick = tick of win (1..4)
static int rclick(const S*in,int px,int py,S*out,int*wtick){
  *out=*in; if(wtick)*wtick=0;
  if(py<=9 || py>=63 || px<0 || px>63) return R_NOOP;
  int h[16],m=0; for(int i=0;i<in->n;i++) if(hit(px,py,in->v[i],in->x[i],in->y[i])) h[m++]=i;
  S w=*in;
  for(int t=1;t<=4;t++){
    for(int k=0;k<m;k++){ int i=h[k]; step1(px,py,w.v[i],&w.x[i],&w.y[i]); }
    if(t<4){ if(MIDWIN && won(&w)){ *out=w; if(wtick)*wtick=t; return R_WIN; } }
  }
  if(m==0){ return R_NOOP; } // engine: nothing moves, step counter only
  // tick-4 merge (mdetahtgad) among hit pieces
  int par[16]; for(int k=0;k<m;k++) par[k]=k;
  for(int a=0;a<m;a++) for(int b=a+1;b<m;b++){ int i=h[a],j=h[b];
    if(!ovl(w.v[i],w.x[i],w.y[i],w.v[j],w.x[j],w.y[j])) continue;
    if(w.v[i]!=w.v[j]) return R_FAIL; // collision: penalty, snapshot restored
    int ra=a; while(par[ra]!=ra) ra=par[ra]; int rb=b; while(par[rb]!=rb) rb=par[rb]; if(ra!=rb) par[rb]=ra; }
  S o; o.n=0; int inh[16]={0}; for(int k=0;k<m;k++) inh[h[k]]=1;
  int cnt[16]={0}; for(int k=0;k<m;k++){ int r=k; while(par[r]!=r) r=par[r]; cnt[r]++; }
  // non-merged pieces (unhit + hit singletons) keep identity
  for(int i=0;i<w.n;i++){ if(!inh[i]){ o.v[o.n]=w.v[i]; o.x[o.n]=w.x[i]; o.y[o.n]=w.y[i]; o.n++; } }
  for(int k=0;k<m;k++){ int r=k; while(par[r]!=r) r=par[r]; if(cnt[r]==1){ int i=h[k]; o.v[o.n]=w.v[i]; o.x[o.n]=w.x[i]; o.y[o.n]=w.y[i]; o.n++; } }
  for(int r=0;r<m;r++){ if(par[r]!=r || cnt[r]<2) continue; int sx=0,sy=0,c=0,v=-1;
    for(int k=0;k<m;k++){ int q=k; while(par[q]!=q) q=par[q]; if(q==r){ int i=h[k]; sx+=cen(w.v[i],w.x[i]); sy+=cen(w.v[i],w.y[i]); c++; v=w.v[i]; } }
    if(v>=8) continue; // removed
    int nv=v+1, s=SZ[nv]; int nx=sx/c - s/2, ny=sy/c - s/2; clampxy(nv,&nx,&ny);
    o.v[o.n]=nv; o.x[o.n]=nx; o.y[o.n]=ny; o.n++; }
  *out=o;
  if(won(&o)){ if(wtick)*wtick=4; return R_WIN; }
  return R_OK; }
static void rstart(S*s){ static const int X[9]={55,61,31,31,12,8,46,30,18}, Y[9]={23,23,22,15,23,28,22,32,16}, V[9]={0,0,0,0,0,0,1,1,1};
  s->n=9; for(int i=0;i<9;i++){ s->v[i]=V[i]; s->x[i]=X[i]; s->y[i]=Y[i]; } }
