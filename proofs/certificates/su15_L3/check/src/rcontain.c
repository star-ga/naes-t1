// Adversarial check lemma check: for every value v=0..8, every click P (rows 10..62), every valid start hit by P:
// (a) landing after 4 ticks is the same for all starts, (b) landing rectangle contains P.
// => any two hit pieces overlap at tick 4 (different values -> collision; >=3 same -> lossy group).
#include "rsim.h"
int main(){ long inst=0,bad_indep=0,bad_contain=0;
 for(int v=0;v<9;v++){ int s=SZ[v]; for(int py=10;py<=62;py++) for(int px=0;px<64;px++){ int L=-1;
  for(int y=10;y<=63&&y<=64-s;y++) for(int x=0;x<=64-s;x++){ if(!hit(px,py,v,x,y)) continue; int xx=x,yy=y; for(int t=0;t<4;t++) step1(px,py,v,&xx,&yy); inst++;
    int l=yy*64+xx; if(L<0)L=l; else if(L!=l) bad_indep++;
    if(!(px>=xx&&px<xx+s&&py>=yy&&py<yy+s)) bad_contain++; } } }
 printf("RCONTAIN values=0..8 instances=%ld landing_depends_on_start=%ld landing_misses_click=%ld\n",inst,bad_indep,bad_contain); return 0; }
