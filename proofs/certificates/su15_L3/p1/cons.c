// P3 sampled consistency: along random click walks from the L3 start, every real transition s->s' (ok, non-lossy) must satisfy h(s) <= 1 + h(s').
// (h(s') = INF when s' cannot win, which only makes the inequality easier.)  usage: cons flags seed nwalks maxlen
#include "p1dp.h"
static uint64_t rng=88172645463325252ULL; static uint64_t rnd(){rng^=rng<<13;rng^=rng>>7;rng^=rng<<17;return rng;}
int main(int argc,char**argv){ int flags=atoi(argv[1]); rng^=(uint64_t)atoll(argv[2])*0x9E3779B97F4A7C15ULL; int nw=atoi(argv[3]),ml=atoi(argv[4]); l3_setup(); long checks=0,viol=0,tight=0; int rule;
  for(int w=0;w<nw;w++){ St s=l3_start(); dpcap=254; int hs=hbound(&s,flags,&rule);
    for(int k=0;k<ml;k++){ St kids[4096]; int nk=0; for(int cy=0;cy<64;cy++)for(int cx=0;cx<64;cx++){St o;int ls;int r=click(&s,cx,cy,&o,&ls); if(r==2&&!ls)kids[nk++]=o;} if(!nk)break;
      // prefer clicks that do something different each time: random child
      St c=kids[rnd()%nk]; dpcap=254; int hc=hbound(&c,flags,&rule); checks++; if(hs>=INF){ /* parent cannot win: nothing to check */ } else if(hs>hc+1&&hc<INF){ viol++; printf("VIOLATION walk %d step %d h(s)=%d h(s')=%d\n",w,k,hs,hc);} if(hs<INF&&hc<INF&&hs==hc+1)tight++;
      if(hc>=INF)break; s=c; hs=hc; }
  }
  printf("CONS flags=%d seed=%s walks=%d transitions_checked=%ld violations=%ld tight(h(s)=h(s')+1)=%ld\n",flags,argv[2],nw,checks,viol,tight); return viol!=0; }
