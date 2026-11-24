#include "p1dp.h"
#include <time.h>
static double now(){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
int main(){ l3_setup(); St s=l3_start(); double t=now(); dp_tables(&s); printf("tables %.2fs\n",now()-t); int r; t=now(); int h=hbound(&s,0,&r); printf("h0 %d total %.2fs\n",h,now()-t); t=now(); h=hbound(&s,3,&r); printf("h3 %d total %.2fs\n",h,now()-t); return 0;}
