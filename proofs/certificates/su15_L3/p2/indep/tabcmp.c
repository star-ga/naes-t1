// compare simulator (simp2.h) single-move and pair-merge landings with the ENGINE-derived tables dumped by indep_dp.py
#include "../simp2.h"
static int valid_v(int v,int x,int y){int s=SZ[v]; return x>=0&&x<=WD-s&&y>=YMIN&&y<=YMAX&&y<=WD-s;}
int main(int argc,char**argv){ sim_init(); FILE*f=fopen(argv[1],"r"); char t; int v,px,py,ex,ey; long n=0,bad=0,inst=0;
  while(fscanf(f," %c %d %d %d %d %d",&t,&v,&px,&py,&ex,&ey)==6){ n++;
    // every valid start position hit by P (single) / every hit position used for both pieces of the pair is compared
    for(int x=0;x<64;x++)for(int y=0;y<64;y++){ if(!valid_v(v,x,y)||!inrange(px,py,v,x,y)) continue; St s,o; int lossy; inst++;
      if(t=='F'){ s.n=1; s.v[0]=v;s.x[0]=x;s.y[0]=y; int r=click(&s,px,py,&o,&lossy); if(r!=2||o.n!=1||o.v[0]!=v||o.x[0]!=ex||o.y[0]!=ey) bad++; }
      else { s.n=2; for(int i=0;i<2;i++){s.v[i]=v;s.x[i]=x;s.y[i]=y;} int r=click(&s,px,py,&o,&lossy); if(r!=2||o.n!=1||o.v[0]!=v+1||o.x[0]!=ex||o.y[0]!=ey||lossy) bad++; } } }
  printf("TABCMP engine_table_entries=%ld sim_instances=%ld mismatches=%ld\n",n,inst,bad); return bad!=0; }
