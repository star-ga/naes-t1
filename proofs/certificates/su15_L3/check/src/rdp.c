// The adversarial check's independent lineage-relaxation lower bound for su15 L3.
// Written from scratch; uses only rsim.h. No cost cap: move closure iterated to convergence.
// Usage: rdp            -> prints landing-independence check, root bound (normal / joint), per-partition table
#include "rsim.h"
#include <omp.h>
#define NP 4096
#define INF 1000
static int npos_ok(int v,int x,int y){ int s=SZ[v]; return x>=0 && x<=64-s && y>=10 && y<=63 && y<=64-s; }
// hit lists: for value v and click P (py 10..62), list of positions p (index y*64+x) hit
static int *HL[4][NP]; static int HN[4][NP]; static int LAND[4][NP]; static int GM[4][NP];
static unsigned char *D[512]; // D[mask][pos] cost (255=INF)
static int mass(int m){ return __builtin_popcount(m&0x3F) + 2*__builtin_popcount(m>>6); }
static int lg(int ms){ return ms==1?0:ms==2?1:ms==4?2:ms==8?3:-1; }
static const int SX[9]={55,61,31,31,12,8,46,30,18}, SY[9]={23,23,22,15,23,28,22,32,16};
int main(int argc,char**argv){
  long indep_bad=0, nhit=0;
  for(int v=0;v<4;v++) for(int P=0;P<NP;P++){ int px=P&63, py=P>>6; HN[v][P]=0; HL[v][P]=NULL; LAND[v][P]=-1; GM[v][P]=-1;
    if(py<10 || py>62) continue;
    int cap=0; int *buf=NULL; int land=-1;
    for(int y=10;y<64;y++) for(int x=0;x<64;x++){ if(!npos_ok(v,x,y)) continue; if(!hit(px,py,v,x,y)) continue;
      int xx=x,yy=y; for(int t=0;t<4;t++) step1(px,py,v,&xx,&yy); int L=yy*64+xx;
      if(land<0) land=L; else if(land!=L) indep_bad++;
      if(HN[v][P]==cap){ cap=cap?cap*2:64; buf=realloc(buf,cap*sizeof(int)); } buf[HN[v][P]++]=y*64+x; nhit++; }
    HL[v][P]=buf; LAND[v][P]=land;
    if(land>=0){ int lx=land&63, ly=land>>6; int c=cen(v,lx), cy=cen(v,ly); int nv=v+1,s=SZ[nv]; int nx=c-s/2, ny=cy-s/2; clampxy(nv,&nx,&ny); GM[v][P]=ny*64+nx; }
  }
  printf("LANDING_INDEP instances=%ld mismatches=%ld\n", nhit, indep_bad); fflush(stdout);
  if(indep_bad){ printf("ABORT: landing depends on start\n"); return 2; }
  // subsets in order of mass
  int order[512], no=0; for(int ms=1; ms<=8; ms*=2) for(int m=1;m<512;m++) if(mass(m)==ms) order[no++]=m;
  printf("VALID_SUBSETS %d\n", no);
  static unsigned char Mtab[512][NP]; // M[T][P]=min over hit positions
  for(int oi=0;oi<no;oi++){ int T=order[oi]; int v=lg(mass(T)); D[T]=malloc(NP); memset(D[T],255,NP);
    if(__builtin_popcount(T)==1){ int i=__builtin_ctz(T); D[T][SY[i]*64+SX[i]]=0; }
    else { // merge sources: split T = A u B, mass(A)=mass(B)=mass(T)/2
      int half=mass(T)/2;
      for(int A=(T-1)&T; A>0; A=(A-1)&T){ int B=T^A; if(A>B) continue; if(mass(A)!=half) continue;
        int vv=v-1;
        for(int P=0;P<NP;P++){ if(GM[vv][P]<0) continue; int c=Mtab[A][P]+Mtab[B][P]+1; if(Mtab[A][P]==255||Mtab[B][P]==255) continue; if(c>254) c=254;
          if(c<D[T][GM[vv][P]]) D[T][GM[vv][P]]=c; } } }
    // move closure to convergence
    int iter=0; for(;;){ int ch=0; iter++;
      for(int P=0;P<NP;P++){ int mn=255; for(int k=0;k<HN[v][P];k++){ int c=D[T][HL[v][P][k]]; if(c<mn) mn=c; } Mtab[T][P]=mn;
        if(mn<254 && LAND[v][P]>=0 && mn+1<D[T][LAND[v][P]]){ D[T][LAND[v][P]]=mn+1; ch=1; } }
      if(!ch) break; }
  }
  // root: partitions T3 (mass 8) + T2 (mass 4)
  int ALL=511; int best=1000,bestN=1000,bestJ=1000; int hist[64]={0};
  for(int T3=1;T3<512;T3++){ if(mass(T3)!=8) continue; int T2=ALL^T3; if(mass(T2)!=4){ printf("BUG\n"); return 3; }
    int z3=255,z2=255; for(int p=0;p<NP;p++){ int x=p&63,y=p>>6; if(npos_ok(3,x,y)&&inzone(cen(3,x),cen(3,y))&&D[T3][p]<z3) z3=D[T3][p]; if(npos_ok(2,x,y)&&inzone(cen(2,x),cen(2,y))&&D[T2][p]<z2) z2=D[T2][p]; }
    int normal=z3+z2; int joint=1000;
#if MIDWIN
    for(int P=0;P<NP;P++){ int px=P&63,py=P>>6; if(py<10||py>62) continue;
      int A[4]={255,255,255,255},B[4]={255,255,255,255}; int Aany=255,Bany=255;
      for(int k=0;k<HN[3][P];k++){ int p=HL[3][P][k]; int c=D[T3][p]; if(c==255) continue; int x=p&63,y=p>>6; for(int t=1;t<=3;t++){ step1(px,py,3,&x,&y); if(inzone(cen(3,x),cen(3,y))){ if(c<A[t])A[t]=c; if(c<Aany)Aany=c; } } }
      for(int k=0;k<HN[2][P];k++){ int p=HL[2][P][k]; int c=D[T2][p]; if(c==255) continue; int x=p&63,y=p>>6; for(int t=1;t<=3;t++){ step1(px,py,2,&x,&y); if(inzone(cen(2,x),cen(2,y))){ if(c<B[t])B[t]=c; if(c<Bany)Bany=c; } } }
      for(int t=1;t<=3;t++) if(A[t]<255&&B[t]<255&&A[t]+B[t]+1<joint) joint=A[t]+B[t]+1;
      if(Aany<255 && z2<255 && Aany+z2+1<joint) joint=Aany+z2+1;
      if(Bany<255 && z3<255 && Bany+z3+1<joint) joint=Bany+z3+1; }
#endif
    int b=normal<joint?normal:joint; hist[b<63?b:63]++;
    if(b<best) best=b; if(normal<bestN) bestN=normal; if(joint<bestJ) bestJ=joint;
    printf("PART T3=%03x T2=%03x normal=%d joint=%d\n",T3,T2,normal,joint);
  }
  printf("ROOT_BOUND %d normal=%d joint=%d midwin=%d\n",best,bestN,bestJ,MIDWIN);
  printf("PART_HIST"); for(int i=0;i<64;i++) if(hist[i]) printf(" %d:%d",i,hist[i]); printf("\n");
  // cost histogram over all (subset,pos)
  long ch[256]={0}; for(int oi=0;oi<no;oi++){ int T=order[oi]; for(int p=0;p<NP;p++) ch[D[T][p]]++; }
  printf("COST_HIST"); for(int i=0;i<255;i++) if(ch[i]) printf(" %d:%ld",i,ch[i]); printf(" INF:%ld\n",ch[255]);
  return 0; }
