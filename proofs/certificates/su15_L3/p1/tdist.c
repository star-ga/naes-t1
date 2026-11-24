// LEMMA-lens self-attack: for random small mass-12 states near the zones, compute the TRUE distance to a win (<=3 clicks) by exhaustive
// breadth-first expansion with all 4096 clicks in the exact simulator simc.h, and compare with h (p1dp.h, flags=3, no cap). h > true distance = VIOLATION.
// usage: tdist seed nstates maxL2   (states whose depth-2 layer exceeds maxL2 are only checked to depth 2)
#include "p1dp.h"
static int cmpq(const void*a,const void*b){return cmpst((const St*)a,(const St*)b);}
static uint64_t rng=88172645463325252ULL; static uint64_t rnd(){rng^=rng<<13;rng^=rng>>7;rng^=rng<<17;return rng;}
static int prewon(const St*s){ return won(s); }
static St*layer(St*fr,size_t nf,size_t*no,int*win,int store){ size_t cap=nf*1200+16,nb=0; St*b=malloc(sizeof(St)*cap); *win=0;
  for(size_t i=0;i<nf;i++) for(int cy=0;cy<64;cy++)for(int cx=0;cx<64;cx++){ St o;int ls; int r=click(&fr[i],cx,cy,&o,&ls); if(r==3){*win=1; free(b); *no=0; return NULL;} if(r!=2||ls||!store)continue; if(nb==cap){cap*=2;b=realloc(b,sizeof(St)*cap);} b[nb++]=o; }
  qsort(b,nb,sizeof(St),cmpq); size_t u=0; for(size_t k=0;k<nb;k++){ if(k&&cmpst(&b[k],&b[k-1])==0)continue; b[u++]=b[k]; } *no=u; return b; }
int main(int argc,char**argv){ l3_setup(); rng^=(uint64_t)atoll(argv[1])*0x9E3779B97F4A7C15ULL; int N=atoi(argv[2]); size_t maxL2=atol(argv[3]);
  static const int shapes[][7]={{3,2,-1},{3,2,-1},{3,2,-1},{3,1,1,-1},{2,2,2,-1},{3,1,0,0,-1},{2,2,1,1,-1},{3,0,0,0,0,-1},{2,2,1,0,0,-1}}; int ns=9;
  long cnt[5]={0},tight[5]={0},viol=0,unk=0,inf_but_win=0; int zc[2][2]={{9,50},{23,50}};
  for(int it=0;it<N;it++){ St s; s.n=0; const int*sh=shapes[rnd()%ns];
    for(int k=0;sh[k]>=0;k++){ int v=sh[k],z=SZ[v]; int*c=zc[rnd()%2]; int sp=(rnd()%3==0)?14:7; int x=c[0]-z/2+(int)(rnd()%(2*sp+1))-sp, y=c[1]-z/2+(int)(rnd()%(2*sp+1))-sp;
      if(x<0)x=0; if(x>WD-z)x=WD-z; if(y<YMIN)y=YMIN; if(y>YMAX)y=YMAX; if(y>WD-z)y=WD-z; s.v[s.n]=v;s.x[s.n]=x;s.y[s.n]=y;s.n++; }
    canon(&s); if(prewon(&s)){it--;continue;}
    int d=-1,w; size_t n1,n2; St*L1=layer(&s,1,&n1,&w,1); if(w)d=1; St*L2=NULL;
    if(d<0){ L2=layer(L1,n1,&n2,&w,1); if(w)d=2; }
    if(d<0){ if(n2<=maxL2){ size_t n3; St*L3=layer(L2,n2,&n3,&w,0); if(w)d=3; else d=4; free(L3);} else d=-2; }
    free(L1); free(L2);
    dpcap=254; int rule; int h=hbound(&s,3,&rule);
    if(d==-2){unk++; continue;}
    int di=d>4?4:d; cnt[di]++; if(d<=3&&h==d)tight[di]++;
    if(d<=3&&h>d){ viol++; printf("VIOLATION h=%d true=%d state:",h,d); for(int i=0;i<s.n;i++)printf(" (%d,%d,%d)",s.v[i],s.x[i],s.y[i]); printf("\n"); }
    if(d<=3&&h>=INF)inf_but_win++;
    if(d==4&&h<=3){ printf("NOTE h=%d but no win within 3 (h not tight, allowed)\n",h); }
    printf("TDIST_RUNNING after_state=%d seed=%s states=%d true_d1=%ld(tight %ld) true_d2=%ld(tight %ld) true_d3=%ld(tight %ld) no_win_le3=%ld unknown_d3=%ld violations=%ld\n",it+1,argv[1],N,cnt[1],tight[1],cnt[2],tight[2],cnt[3],tight[3],cnt[4],unk,viol); fflush(stdout); }
  printf("TDIST seed=%s states=%d true_d1=%ld(tight %ld) true_d2=%ld(tight %ld) true_d3=%ld(tight %ld) no_win_le3=%ld unknown_d3=%ld violations=%ld\n",argv[1],N,cnt[1],tight[1],cnt[2],tight[2],cnt[3],tight[3],cnt[4],unk,viol);
  return viol!=0; }
