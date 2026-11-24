#include "p1dp.h"
#include <time.h>
static double now(){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
int main(){ l3_setup(); srand(5); St s=l3_start(); for(int d=0;d<2;d++){ St kids[4096];int nk=0; for(int cy=0;cy<64;cy++)for(int cx=0;cx<64;cx++){St o;int ls;int r=click(&s,cx,cy,&o,&ls); if(r==2&&!ls)kids[nk++]=o;} s=kids[rand()%nk]; }
 for(int cap=8;cap<=254;cap+=123){ dpcap=cap>254?254:cap; double t=now(); int rule; int h=hbound(&s,3,&rule); printf("cap=%d h=%d %.2fs\n",dpcap,h,now()-t);} }
