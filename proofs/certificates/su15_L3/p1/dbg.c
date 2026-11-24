#include "simc.h"
int main(){ NZ=2; ZONES[0]=(Zone){5,46,9,9}; ZONES[1]=(Zone){19,46,9,9}; NG=2; GV[0]=3;GC[0]=1;GV[1]=2;GC[1]=1;
 St s; s.n=9; int init[9][3]={{0,55,23},{0,61,23},{0,31,22},{0,31,15},{0,12,23},{0,8,28},{1,46,22},{1,30,32},{1,18,16}}; for(int i=0;i<9;i++){s.v[i]=init[i][0];s.x[i]=init[i][1];s.y[i]=init[i][2];} canon(&s);
 int c2=0,c3=0,c1=0,c0=0; for(int cy=0;cy<64;cy++)for(int cx=0;cx<64;cx++){St o;int ls;int r=click(&s,cx,cy,&o,&ls); if(r==2)c2++; if(r==3)c3++; if(r==1)c1++; if(r==0)c0++;} printf("%d %d %d %d\n",c0,c1,c2,c3);
 static St all[5000]; int na=0; for(int cy=0;cy<64;cy++)for(int cx=0;cx<64;cx++){St o;int ls;int r=click(&s,cx,cy,&o,&ls); if(r==2)all[na++]=o;}
 int u=0; for(int i=0;i<na;i++){int dup=0; for(int j=0;j<i;j++) if(cmpst(&all[i],&all[j])==0){dup=1;break;} if(!dup)u++;} printf("distinct %d\n",u); }
