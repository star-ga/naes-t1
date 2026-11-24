// Adversarial check: lineage-relaxation bound h(s) for ARBITRARY states (same method as rdp.c, generalised).
// input: lines "n v x y v x y ..." ; output: "H <h> normal=<n> joint=<j> | <line>"
// h = INF(99) if mass != 12 or a piece above v3.
#include "rsim.h"
#define NP 4096
static int npos_ok(int v,int x,int y){ int s=SZ[v]; return x>=0 && x<=64-s && y>=10 && y<=63 && y<=64-s; }
static int *HL[4][NP]; static int HN[4][NP]; static int LAND[4][NP]; static int GM[4][NP];
static int lg(int ms){ return ms==1?0:ms==2?1:ms==4?2:ms==8?3:-1; }
static unsigned char Dt[512][NP], Mt[512][NP];
int main(int argc,char**argv){
  for(int v=0;v<4;v++) for(int P=0;P<NP;P++){ int px=P&63, py=P>>6; HN[v][P]=0; HL[v][P]=NULL; LAND[v][P]=-1; GM[v][P]=-1; if(py<10||py>62) continue;
    int cap=0,*buf=NULL,land=-1;
    for(int y=10;y<64;y++) for(int x=0;x<64;x++){ if(!npos_ok(v,x,y)||!hit(px,py,v,x,y)) continue; int xx=x,yy=y; for(int t=0;t<4;t++) step1(px,py,v,&xx,&yy); int L=yy*64+xx;
      if(land<0) land=L; else if(land!=L){ printf("ABORT landing\n"); return 2; }
      if(HN[v][P]==cap){ cap=cap?cap*2:64; buf=realloc(buf,cap*sizeof(int)); } buf[HN[v][P]++]=y*64+x; }
    HL[v][P]=buf; LAND[v][P]=land;
    if(land>=0){ int lx=land&63, ly=land>>6; int c=cen(v,lx), cy=cen(v,ly); int nv=v+1,s=SZ[nv]; int nx=c-s/2, ny=cy-s/2; clampxy(nv,&nx,&ny); GM[v][P]=ny*64+nx; } }
  // inverse hit lists: for value v and position p, the clicks P whose hit set contains p
  static int *IL[4][NP]; static int IN[4][NP]; { for(int v=0;v<4;v++){ for(int p=0;p<NP;p++) IN[v][p]=0; for(int P=0;P<NP;P++) for(int k=0;k<HN[v][P];k++) IN[v][HL[v][P][k]]++;
      for(int p=0;p<NP;p++){ IL[v][p]=malloc((IN[v][p]+1)*sizeof(int)); IN[v][p]=0; } for(int P=0;P<NP;P++) for(int k=0;k<HN[v][P];k++){ int p=HL[v][P][k]; IL[v][p][IN[v][p]++]=P; } } }
  // zone-tick masks per hit start (bit t-1 set if centred in a zone after tick t, t=1..3), and zone-relevant clicks
  static unsigned char *ZM[4][NP]; static int ZREL[NP]; { for(int v=2;v<4;v++) for(int P=0;P<NP;P++){ int px=P&63,py=P>>6; ZM[v][P]=malloc(HN[v][P]+1);
      for(int k=0;k<HN[v][P];k++){ int q=HL[v][P][k]; int x=q&63,y=q>>6; unsigned char m=0; for(int t=1;t<=3;t++){ step1(px,py,v,&x,&y); if(inzone(cen(v,x),cen(v,y))) m|=1<<(t-1); } ZM[v][P][k]=m; if(m) ZREL[P]=1; } } }
  static int bq[256][NP]; static int bn[256]; static unsigned char used[NP];
  char line[4096];
  while(fgets(line,sizeof line,stdin)){ char*p=line; int n=strtol(p,&p,10); if(n<=0) continue; int V[16],X[16],Y[16],ms=0,bad=0;
    for(int i=0;i<n;i++){ V[i]=strtol(p,&p,10); X[i]=strtol(p,&p,10); Y[i]=strtol(p,&p,10); ms+=1<<V[i]; if(V[i]>3) bad=1; }
    line[strcspn(line,"\n")]=0;
    if(bad||ms!=12||n>9){ printf("H 99 normal=99 joint=99 | %s\n",line); fflush(stdout); continue; }
    int N=1<<n; int massm[512]; for(int m=0;m<N;m++){ int s=0; for(int i=0;i<n;i++) if(m>>i&1) s+=1<<V[i]; massm[m]=s; }
    for(int ms2=1; ms2<=8; ms2*=2) for(int T=1;T<N;T++){ if(massm[T]!=ms2) continue; int v=lg(ms2);
      memset(Dt[T],255,NP);
      if(__builtin_popcount(T)==1){ int i=__builtin_ctz(T); if(V[i]!=v){ continue; } Dt[T][Y[i]*64+X[i]]=0; }
      else { int half=ms2/2; for(int A=(T-1)&T; A>0; A=(A-1)&T){ int B=T^A; if(A>B||massm[A]!=half) continue; int vv=v-1;
          for(int P=0;P<NP;P++){ if(GM[vv][P]<0||Mt[A][P]==255||Mt[B][P]==255) continue; int c=Mt[A][P]+Mt[B][P]+1; if(c>254)c=254; if(c<Dt[T][GM[vv][P]]) Dt[T][GM[vv][P]]=c; } } }
      // bucket-queue closure (each click used once, at the least cost of a position it hits) = same fixpoint as rdpS.c
      memset(bn,0,sizeof bn); memset(used,0,NP); memset(Mt[T],255,NP);
      for(int p=0;p<NP;p++) if(Dt[T][p]<255) bq[Dt[T][p]][bn[Dt[T][p]]++]=p;
      for(int c=0;c<255;c++){ for(int i=0;i<bn[c];i++){ int p=bq[c][i]; if(Dt[T][p]!=c) continue;
          for(int k=0;k<IN[v][p];k++){ int P=IL[v][p][k]; if(used[P]) continue; used[P]=1; Mt[T][P]=c; int L=LAND[v][P];
            if(c+1<254 && L>=0 && c+1<Dt[T][L]){ Dt[T][L]=c+1; bq[c+1][bn[c+1]++]=L; } } } } }
    int ALL=N-1, best=999,bN=999,bJ=999;
    for(int T3=1;T3<N;T3++){ if(massm[T3]!=8) continue; int T2=ALL^T3; if(massm[T2]!=4) continue;
      int z3=255,z2=255; for(int q=0;q<NP;q++){ int x=q&63,y=q>>6; if(npos_ok(3,x,y)&&inzone(cen(3,x),cen(3,y))&&Dt[T3][q]<z3) z3=Dt[T3][q]; if(npos_ok(2,x,y)&&inzone(cen(2,x),cen(2,y))&&Dt[T2][q]<z2) z2=Dt[T2][q]; }
      int normal=(z3<255&&z2<255)?z3+z2:999, joint=999;
#if MIDWIN
      for(int P=0;P<NP;P++){ if(!ZREL[P]) continue; int A[4]={255,255,255,255},B[4]={255,255,255,255},Aany=255,Bany=255;
        for(int k=0;k<HN[3][P];k++){ unsigned char m=ZM[3][P][k]; if(!m) continue; int c=Dt[T3][HL[3][P][k]]; if(c==255) continue; for(int t=1;t<=3;t++) if(m>>(t-1)&1){ if(c<A[t])A[t]=c; } if(c<Aany)Aany=c; }
        for(int k=0;k<HN[2][P];k++){ unsigned char m=ZM[2][P][k]; if(!m) continue; int c=Dt[T2][HL[2][P][k]]; if(c==255) continue; for(int t=1;t<=3;t++) if(m>>(t-1)&1){ if(c<B[t])B[t]=c; } if(c<Bany)Bany=c; }
        for(int t=1;t<=3;t++) if(A[t]<255&&B[t]<255&&A[t]+B[t]+1<joint) joint=A[t]+B[t]+1;
        if(Aany<255&&z2<255&&Aany+z2+1<joint) joint=Aany+z2+1; if(Bany<255&&z3<255&&Bany+z3+1<joint) joint=Bany+z3+1; }
#endif
      int b=normal<joint?normal:joint; if(b<best)best=b; if(normal<bN)bN=normal; if(joint<bJ)bJ=joint; }
    // a state that is already winning has h=0
    S s; s.n=n; for(int i=0;i<n;i++){s.v[i]=V[i];s.x[i]=X[i];s.y[i]=Y[i];} if(won(&s)) best=0;
    printf("H %d normal=%d joint=%d | %s\n",best,bN,bJ,line); fflush(stdout); }
  return 0; }
