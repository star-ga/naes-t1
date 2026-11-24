#include "p1dp.h"
int main(){ l3_setup(); St s=l3_start(); if(dp_tables(&s)<0){puts("bad");return 1;} FILE*f=fopen("logs/D_root.bin","wb"); int n=s.n; fwrite(&n,4,1,f);
  for(int i=0;i<n;i++){int a[3]={s.v[i],s.x[i],s.y[i]}; fwrite(a,4,3,f);} int cnt=0;
  for(int m=1;m<(1<<n);m++) if(vm[m]>=0){ fwrite(&m,4,1,f); fwrite(D[m],1,NP,f); cnt++; }
  fclose(f); printf("dumped %d tables\n",cnt); return 0; }
