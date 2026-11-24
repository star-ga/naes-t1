// census of h (full DP, no cap) over the distinct children of the level-3 start (depth 1). usage: census shard mod
#include "p1dp.h"
static int cmpq(const void*a,const void*b){return cmpst((const St*)a,(const St*)b);}
int main(int argc,char**argv){ int shard=atoi(argv[1]),mod=atoi(argv[2]); l3_setup(); St s0=l3_start(); static St k[4096]; int n=0;
  for(int cy=0;cy<64;cy++)for(int cx=0;cx<64;cx++){ St o;int ls; int r=click(&s0,cx,cy,&o,&ls); if(r==3){printf("WIN AT DEPTH 1 %d,%d\n",cx,cy);} if(r!=2||ls)continue; if(cmpst(&o,&s0)==0)continue; k[n++]=o; }
  qsort(k,n,sizeof(St),cmpq); int u=0; for(int i=0;i<n;i++){ if(i&&cmpst(&k[i],&k[i-1])==0)continue; k[u++]=k[i]; }
  int hist[256]={0}; for(int i=0;i<u;i++){ if(i%mod!=shard)continue; dpcap=254; int rule; int h=hbound(&k[i],3,&rule); hist[h]++; printf("child %d h=%d rule=%d\n",i,h,rule); fflush(stdout); }
  printf("CENSUS shard %d/%d distinct_children=%d hist:",shard,mod,u); for(int h=0;h<256;h++) if(hist[h]) printf(" h%d=%d",h,hist[h]); printf("\n"); return 0; }
