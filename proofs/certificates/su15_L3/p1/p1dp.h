// p1dp.h : relaxed "lineage forest" lower bound h(state) on clicks to win su15 level 3, any state.
// Uses the exact simulator simc.h (hash recorded) for click geometry.  Relaxation R (see PROOF.md):
//   a real non-wasting click is (M) one piece moved to landc(v,P), or (G) two same-value pieces moved+merged at landc;
//   everything else (fail, lossy, noop) cannot lie on a shortest winning route; lineages are then independent.
#ifdef NOMID
#include "simc_nomid.h"
#else
#include "simc.h"
#endif
#define INF 255
#define NP 4096
#define FLAG_PAIR 1    // joint final click moves v3 and v2 together, win tested on ticks 1..3
#define FLAG_SINGLEMID 2 // final click moves one piece through a zone at ticks 1..3, other piece static in zone
static uint8_t D[512][NP]; static int vm[512]; static int dpcap=254; // dpcap: only costs <= dpcap are kept (search uses the remaining budget); 254 = no cap
static int offc[9]; static short offx[9][1024], offy[9][1024];
static inline int valid(int v,int x,int y){return x>=0&&x<=WD-SZ[v]&&y>=YMIN&&y<=YMAX&&y<=WD-SZ[v];}
static inline void landc(int v,int cx,int cy,int*lx,int*ly){ int x=cx-SZ[v]/2,y=cy-SZ[v]/2; clampp(v,&x,&y); *lx=x;*ly=y; }
static inline int inzone(int v,int x,int y){int c0=x+SZ[v]/2,c1=y+SZ[v]/2; for(int z=0;z<NZ;z++)if(c0>=ZONES[z].zx&&c0<ZONES[z].zx+ZONES[z].zw&&c1>=ZONES[z].zy&&c1<ZONES[z].zy+ZONES[z].zh)return 1; return 0;}
static void stepf(int v,int*x,int*y,int cx,int cy){int c0=*x+SZ[v]/2,c1=*y+SZ[v]/2;int dx=cx-c0,dy=cy-c1;
  int ix=dx>0?(dx<STEP?dx:STEP):(dx<0?(dx>-STEP?dx:-STEP):0); int iy=dy>0?(dy<STEP?dy:STEP):(dy<0?(dy>-STEP?dy:-STEP):0); *x+=ix;*y+=iy;clampp(v,x,y);}
static void dp_init(void){
  for(int v=0;v<9;v++){ offc[v]=0; for(int dx=-12;dx<=12;dx++)for(int dy=-12;dy<=12;dy++){
      int cx=20,cy=20; if(inrange(cx,cy,v,cx+dx,cy+dy)){offx[v][offc[v]]=dx;offy[v][offc[v]]=dy;offc[v]++;} } }
  // window check: nothing in range outside [-12,12]
  for(int v=0;v<4;v++)for(int dx=-30;dx<=30;dx++)for(int dy=-30;dy<=30;dy++){ if(dx>=-12&&dx<=12&&dy>=-12&&dy<=12)continue; if(inrange(40,40,v,40+dx,40+dy)){fprintf(stderr,"WINDOW VIOLATION v=%d %d %d\n",v,dx,dy);exit(3);} }
}
static void moves(int T,int v){
  int ch=1;
  while(ch){ ch=0;
    for(int cy=YMIN;cy<YMAX;cy++)for(int cx=0;cx<WD;cx++){
      int best=INF;
      for(int k=0;k<offc[v];k++){int x=cx+offx[v][k],y=cy+offy[v][k]; if(!valid(v,x,y))continue; int c=D[T][x*64+y]; if(c<best)best=c;}
      if(best>=INF-1||best+1>dpcap)continue;
      int lx,ly; landc(v,cx,cy,&lx,&ly); int pos=lx*64+ly;
      if(best+1<D[T][pos]){D[T][pos]=best+1;ch=1;}
    }
  }
}
static inline int pc9(int m){return __builtin_popcount(m);}
// compute D tables for the pieces of s.  returns 0 ok, -1 if state cannot win (mass != 12 or value >3)
static int dp_tables(const St*s){
  int n=s->n,M=1<<n; int mass=0; for(int i=0;i<n;i++){ if(s->v[i]>3)return -1; mass+=1<<s->v[i]; }
  if(mass!=12) return -1;
  for(int m=0;m<M;m++){int ms=0;for(int i=0;i<n;i++)if(m>>i&1)ms+=1<<s->v[i]; vm[m]=-1; if(m&&(ms&(ms-1))==0){int v=0;while((1<<v)<ms)v++; vm[m]=v;}}
  for(int m=0;m<M;m++) memset(D[m],INF,NP);
  for(int c=1;c<=n;c++)for(int T=1;T<M;T++){ if(pc9(T)!=c||vm[T]<0)continue; int v=vm[T];
    if(c==1){int i=__builtin_ctz(T); D[T][s->x[i]*64+s->y[i]]=0;}
    else { if(v==0)continue; int pv=v-1;
      for(int T1=(T-1)&T;T1>0;T1=(T1-1)&T){ int T2=T^T1; if(T1<T2)continue; if(vm[T1]!=pv||vm[T2]!=pv)continue;
        static uint8_t M1[NP],M2[NP];
        for(int cy=YMIN;cy<YMAX;cy++)for(int cx=0;cx<WD;cx++){ int b1=INF,b2=INF;
          for(int k=0;k<offc[pv];k++){int x=cx+offx[pv][k],y=cy+offy[pv][k]; if(!valid(pv,x,y))continue; int c1=D[T1][x*64+y]; if(c1<b1)b1=c1; c1=D[T2][x*64+y]; if(c1<b2)b2=c1;}
          M1[cx*64+cy]=b1;M2[cx*64+cy]=b2; }
        for(int cy=YMIN;cy<YMAX;cy++)for(int cx=0;cx<WD;cx++){ int b1=M1[cx*64+cy],b2=M2[cx*64+cy]; if(b1>=INF||b2>=INF||b1+b2+1>dpcap)continue;
          int lx,ly; landc(pv,cx,cy,&lx,&ly); int ax=lx+SZ[pv]/2-SZ[v]/2, ay=ly+SZ[pv]/2-SZ[v]/2; clampp(v,&ax,&ay); int pos=ax*64+ay;
          if(b1+b2+1<D[T][pos])D[T][pos]=b1+b2+1; }
      } }
    moves(T,v);
  }
  return 0;
}
static int zonemin(int T,int v){ int c=INF; for(int x=0;x<WD;x++)for(int y=YMIN;y<=YMAX;y++){ if(!valid(v,x,y))continue; if(D[T][x*64+y]<c&&inzone(v,x,y))c=D[T][x*64+y]; } return c; }
// h: min over partitions of final-click rules.  *rule gets which rule gave the min (0=end-state landing,1=pair,2=single-mid)
static int hbound(const St*s,int flags,int*rule){
  *rule=-1; if(dp_tables(s)<0) return INF;
  int n=s->n,M=1<<n,full=M-1,best=dpcap+1;
  for(int T3=1;T3<M;T3++){ if(vm[T3]!=3)continue; int T2=full^T3; if(!T2||vm[T2]!=2)continue;
    int c3=zonemin(T3,3),c2=zonemin(T2,2);
    if(c3<INF&&c2<INF&&c3+c2<best){best=c3+c2;*rule=0;}
    if(flags&FLAG_SINGLEMID){
      for(int w=0;w<2;w++){ int Tm=w?T2:T3,vmv=w?2:3,other=w?c3:c2; if(other>=INF)continue;
        for(int cy=YMIN;cy<YMAX;cy++)for(int cx=0;cx<WD;cx++) for(int k=0;k<offc[vmv];k++){int ax=cx+offx[vmv][k],ay=cy+offy[vmv][k]; if(!valid(vmv,ax,ay))continue; int ca=D[Tm][ax*64+ay]; if(ca>=INF||ca+1+other>=best)continue;
          int x1=ax,y1=ay,x2,y2,x3,y3; stepf(vmv,&x1,&y1,cx,cy); x2=x1;y2=y1; stepf(vmv,&x2,&y2,cx,cy); x3=x2;y3=y2; stepf(vmv,&x3,&y3,cx,cy);
          if(inzone(vmv,x1,y1)||inzone(vmv,x2,y2)||inzone(vmv,x3,y3)){best=ca+1+other;*rule=2;} } } }
    if(flags&FLAG_PAIR){
      for(int cy=YMIN;cy<YMAX;cy++)for(int cx=0;cx<WD;cx++)
        for(int k=0;k<offc[3];k++){int ax=cx+offx[3][k],ay=cy+offy[3][k]; if(!valid(3,ax,ay))continue; int ca=D[T3][ax*64+ay]; if(ca>=INF||ca+1>=best)continue;
          int a1x=ax,a1y=ay,a2x,a2y,a3x,a3y; stepf(3,&a1x,&a1y,cx,cy); a2x=a1x;a2y=a1y; stepf(3,&a2x,&a2y,cx,cy); a3x=a2x;a3y=a2y; stepf(3,&a3x,&a3y,cx,cy);
          int za[3]={inzone(3,a1x,a1y),inzone(3,a2x,a2y),inzone(3,a3x,a3y)}; if(!(za[0]|za[1]|za[2]))continue;
          for(int m=0;m<offc[2];m++){int bx=cx+offx[2][m],by=cy+offy[2][m]; if(!valid(2,bx,by))continue; int cb=D[T2][bx*64+by]; if(cb>=INF||ca+cb+1>=best)continue;
            int b1x=bx,b1y=by,b2x,b2y,b3x,b3y; stepf(2,&b1x,&b1y,cx,cy); b2x=b1x;b2y=b1y; stepf(2,&b2x,&b2y,cx,cy); b3x=b2x;b3y=b2y; stepf(2,&b3x,&b3y,cx,cy);
            int zb[3]={inzone(2,b1x,b1y),inzone(2,b2x,b2y),inzone(2,b3x,b3y)};
            if((za[0]&&zb[0])||(za[1]&&zb[1])||(za[2]&&zb[2])){best=ca+cb+1;*rule=1;} } } }
  }
  return best;
}
static void l3_setup(void){ NZ=2; ZONES[0]=(Zone){5,46,9,9}; ZONES[1]=(Zone){19,46,9,9}; NG=2; GV[0]=3;GC[0]=1;GV[1]=2;GC[1]=1; dp_init(); }
static St l3_start(void){ St s; s.n=9; int init[9][3]={{0,55,23},{0,61,23},{0,31,22},{0,31,15},{0,12,23},{0,8,28},{1,46,22},{1,30,32},{1,18,16}}; for(int i=0;i<9;i++){s.v[i]=init[i][0];s.x[i]=init[i][1];s.y[i]=init[i][2];} canon(&s); return s; }
