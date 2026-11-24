// p2 SHORTER-ROUTE lens: beam search for a win in <=10 clicks on the exact simulator simp2.h (lockstep-checked),
// heuristics that do NOT use the lineage bound. Usage: beam10 B mode seed lambda   (mode 0: zone distance, 1: partner distance, 2: random)
#include "../simp2.h"
#include <math.h>
typedef struct { St s; int par; short cx, cy; float f; } Node;
static int B, MODE; static double LAM; static uint64_t rs;
static double rnd(void){ rs=rs*6364136223846793005ULL+1442695040888963407ULL; return (rs>>11)*(1.0/9007199254740992.0); }
static double score(const St*s){
  double f=s->n*1.0, d=0;
  if(MODE==2) return rnd();
  for(int i=0;i<s->n;i++){ int c0=s->x[i]+SZ[s->v[i]]/2, c1=s->y[i]+SZ[s->v[i]]/2;
    if(MODE==0){ double best=1e9; for(int z=0;z<NZ;z++){ double dx=c0-(ZONES[z].zx+4), dy=c1-(ZONES[z].zy+4); double e=sqrt(dx*dx+dy*dy); if(e<best)best=e; } d+=best; }
    else { double best=1e9; for(int j=0;j<s->n;j++){ if(j==i) continue; if(s->v[j]!=s->v[i]) continue; double dx=c0-(s->x[j]+SZ[s->v[j]]/2), dy=c1-(s->y[j]+SZ[s->v[j]]/2); double e=sqrt(dx*dx+dy*dy); if(e<best)best=e; }
      if(best>1e8){ double b2=1e9; for(int z=0;z<NZ;z++){ double dx=c0-(ZONES[z].zx+4), dy=c1-(ZONES[z].zy+4); double e=sqrt(dx*dx+dy*dy); if(e<b2)b2=e;} best=b2; } d+=best; } }
  return f + LAM*d/16.0 + 0.01*rnd();
}
static int cmpn(const void*a,const void*b){ float x=((const Node*)a)->f, y=((const Node*)b)->f; return x<y?-1:x>y; }
int main(int argc,char**argv){ sim_init(); B=atoi(argv[1]); MODE=atoi(argv[2]); rs=strtoull(argv[3],0,10)*2654435761ULL+1; LAM=atof(argv[4]); int MAXD=argc>5?atoi(argv[5]):10;
  static const int START[9][3]={{0,55,23},{0,61,23},{0,31,22},{0,31,15},{0,12,23},{0,8,28},{1,46,22},{1,30,32},{1,18,16}};
  Node* lev[16]; int nl[16]; lev[0]=malloc(sizeof(Node)); nl[0]=1; St s0; s0.n=9; for(int i=0;i<9;i++){s0.v[i]=START[i][0];s0.x[i]=START[i][1];s0.y[i]=START[i][2];} canon(&s0); lev[0][0].s=s0; lev[0][0].par=-1;
  size_t TN=1ULL<<25; uint64_t*T=malloc(TN*8);
  long gen_total=0;
  for(int d=1; d<=MAXD; d++){
    size_t cap=(size_t)nl[d-1]*1100+16; if(cap>60000000) cap=60000000; Node*C=malloc(sizeof(Node)*cap); size_t nc=0; memset(T,0,TN*8);
    for(int p=0;p<nl[d-1];p++){ const St*s=&lev[d-1][p].s;
      for(int P=0;P<4096;P++){ St o;int lossy; int r=click(s,P/64,P%64,&o,&lossy); gen_total++;
        if(r==3){ // WIN: print route
          int cx[16],cy[16]; cx[d-1]=P/64; cy[d-1]=P%64; int q=p; for(int k=d-1;k>=1;k--){ cx[k-1]=lev[k][q].cx; cy[k-1]=lev[k][q].cy; q=lev[k][q].par; }
          printf("BEAM_WIN depth %d route",d); for(int k=0;k<d;k++) printf(" %d,%d",cx[k],cy[k]); printf("\n"); return 0; }
        if(r!=2||lossy) continue;
        uint64_t h=hst(&o)|1; size_t k=h&(TN-1); int dup=0; while(T[k]){ if(T[k]==h){dup=1;break;} k=(k+1)&(TN-1);} if(dup) continue; T[k]=h;
        if(nc<cap){ C[nc].s=o; C[nc].par=p; C[nc].cx=P/64; C[nc].cy=P%64; C[nc].f=score(&o); nc++; } } }
    qsort(C,nc,sizeof(Node),cmpn); int keep=nc<(size_t)B?nc:B; lev[d]=malloc(sizeof(Node)*(keep+1)); memcpy(lev[d],C,sizeof(Node)*keep); nl[d]=keep; free(C);
    fprintf(stderr,"depth %d children %zu kept %d best_f %.3f minpieces %d\n",d,nc,keep,keep?lev[d][0].f:0.0,keep?lev[d][0].s.n:0);
    printf("BEAM depth %d distinct_children %zu kept %d\n",d,nc,keep); fflush(stdout); }
  printf("BEAM_NO_WIN within %d (B=%d mode=%d lam=%.2f) clicks_simulated=%ld\n",MAXD,B,MODE,LAM,gen_total); return 0; }
