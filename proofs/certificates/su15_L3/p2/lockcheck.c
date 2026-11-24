// compares engine-produced vectors (gen_vectors.py) with the simulator. Prints LOCKCHECK <records> <mismatches> <wins> ...
#include "simp2.h"
int main(int argc,char**argv){ sim_init(); long tot=0,bad=0,wins=0,okc=0,noop=0,fail=0,merges=0,near=0,prewon=0; char buf[65536];
  for(int a=1;a<argc;a++){ FILE*f=fopen(argv[a],"r"); if(!f){perror(argv[a]);return 2;}
    while(fgets(buf,sizeof buf,f)){ char*p=buf; int n,off; if(sscanf(p,"%d%n",&n,&off)!=1)continue; p+=off;
      St s; s.n=n; for(int i=0;i<n;i++){int v,x,y;sscanf(p,"%d %d %d%n",&v,&x,&y,&off);p+=off;s.v[i]=v;s.x[i]=x;s.y[i]=y;}
      int cx,cy; sscanf(p,"%d %d%n",&cx,&cy,&off);p+=off; while(*p==' ')p++;
      int ew=(*p=='W'); St e; e.n=0; if(!ew){ p++; int n2; sscanf(p,"%d%n",&n2,&off);p+=off; e.n=n2; for(int i=0;i<n2;i++){int v,x,y;sscanf(p,"%d %d %d%n",&v,&x,&y,&off);p+=off;e.v[i]=v;e.x[i]=x;e.y[i]=y;} }
      if(won(&s)){ prewon++; continue; } // injected state already satisfies the win test: engine wins on ANY click; such a pre-state cannot occur on a route (the click that created it would have won)
      St o; int lossy; int r=click(&s,cx,cy,&o,&lossy); tot++;
      int good;
      if(ew) good=(r==3);
      else if(r==3) good=0;
      else { const St*ex=(r==2)?&o:&s; good=(cmpst(ex,&e)==0); }
      if(ew)wins++; else if(r==2){okc++; if(o.n<s.n)merges++;} else if(r==0)noop++; else fail++;
      if(!good){bad++; if(bad<=10){printf("MISMATCH rec %ld r=%d ew=%d click %d %d n=%d\n",tot,r,ew,cx,cy,s.n);} }
    } fclose(f); }
  printf("LOCKCHECK records=%ld mismatches=%ld engine_wins=%ld ok_clicks=%ld merges=%ld noop=%ld fail=%ld excluded_prestate_already_won=%ld\n",tot,bad,wins,okc,merges,noop,fail,prewon); return bad!=0; }
