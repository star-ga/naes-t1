#include "p1dp.h"
int main(int argc,char**argv){ l3_setup(); St s=l3_start(); int flags=argc>1?atoi(argv[1]):3; int rule;
  int h=hbound(&s,flags,&rule); printf("flags=%d H_ROOT=%d rule=%d\n",flags,h,rule); return 0; }
