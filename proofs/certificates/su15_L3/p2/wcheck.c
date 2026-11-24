// Every ENGINE win on a 2-piece {v3,v2} pre-state must be in the relation W (built from the simulator); also counts engine wins by pre-state size.
#include "simp2.h"
int main(int argc,char**argv){ sim_init(); static uint64_t W[4096][64]; FILE*f=fopen(argv[1],"rb"); if(!f||fread(W,512,4096,f)!=4096){fprintf(stderr,"W load\n");return 2;} fclose(f);
  long wins=0,w2=0,miss=0,w3=0,excl=0; char buf[65536];
  for(int a=2;a<argc;a++){ FILE*g=fopen(argv[a],"r"); if(!g){perror(argv[a]);return 2;}
    while(fgets(buf,sizeof buf,g)){ char*p=buf; int n,off; if(sscanf(p,"%d%n",&n,&off)!=1)continue; p+=off; int v[9],x[9],y[9]; for(int i=0;i<n;i++){sscanf(p,"%d %d %d%n",&v[i],&x[i],&y[i],&off);p+=off;}
      int cx,cy; sscanf(p,"%d %d%n",&cx,&cy,&off);p+=off; while(*p==' ')p++; if(*p!='W')continue; { St pre; pre.n=n; for(int i=0;i<n;i++){pre.v[i]=v[i];pre.x[i]=x[i];pre.y[i]=y[i];} if(won(&pre)){excl++;continue;} } wins++;
      if(n==2){ w2++; int ia=(v[0]==3)?0:1, ib=1-ia; if(v[ia]==3&&v[ib]==2){ int A=x[ia]*64+y[ia],B=x[ib]*64+y[ib]; if(!((W[A][B/64]>>(B%64))&1)) miss++; } else miss++; } else w3++; }
    fclose(g); }
  printf("WCHECK engine_wins=%ld on_2piece_prestate=%ld not_in_W=%ld other_prestates=%ld excluded_prestate_already_won=%ld\n",wins,w2,miss,w3,excl); return miss!=0; }
