// admissibility along a verified winning route: for every prefix state s_k the true distance to a win is <= L-k, so h(s_k) must be <= L-k.
#include "p1dp.h"
int main(int argc,char**argv){ int flags=atoi(argv[1]); l3_setup(); St s=l3_start(); int L=0; int cl[20][2];
  for(int i=2;i<argc;i++){ int x,y; sscanf(argv[i],"%d,%d",&x,&y); cl[L][0]=x;cl[L][1]=y;L++; }
  int viol=0; for(int k=0;k<=L;k++){ int rule; dpcap=254; int h=hbound(&s,flags,&rule); int tr=L-k; printf("k=%d h=%d true_remaining<=%d %s\n",k,h,tr,(k<L&&h>tr)?"VIOLATION":"ok"); if(k<L&&h>tr)viol++;
    if(k==L)break; St o;int ls; int r=click(&s,cl[k][0],cl[k][1],&o,&ls); if(k==L-1){ printf("final click status=%d (3=win)\n",r); break;} if(r!=2){printf("route broken at %d r=%d\n",k,r);return 2;} s=o; }
  printf("ROUTE_ADMISSIBILITY flags=%d violations=%d\n",flags,viol); return viol!=0; }
