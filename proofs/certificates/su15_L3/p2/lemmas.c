// Exhaustive per-piece / per-pair lemma checks against the simulator (simp2.h).
//  reach   : every hit piece (all values 0..8, all positions, all click points) ends with a rect containing the click point and top-left == landing(v,P) (independent of start position)
//  pair V  : two pieces of value V (V=0..2), ALL ordered position pairs hit by the same click P: result is ONE piece of value V+1 at a position independent of the pair; never lossy
//  mixed U W: ALL pairs of different values (U<W<=3) hit by the same click: status is FAIL(1); for (U,W)=(2,3) status is FAIL or WIN only
//  triple V: random same-value triples hit by one click are flagged lossy (mass decreases)
#include "simp2.h"
#include <omp.h>
static int valid_v(int v,int x,int y){int s=SZ[v]; return x>=0&&x<=WD-s&&y>=YMIN&&y<=YMAX&&y<=WD-s;}
static uint16_t *HL[9][4096]; static int HN[9][4096];
static void hits(int vmax){ for(int v=0;v<=vmax;v++)for(int P=0;P<4096;P++){ int cx=P/64,cy=P%64; HN[v][P]=0; HL[v][P]=NULL; if(cy<YMIN||cy>=YMAX)continue; uint16_t*L=malloc(2*1500);int k=0;
   for(int x=0;x<64;x++)for(int y=0;y<64;y++) if(valid_v(v,x,y)&&inrange(cx,cy,v,x,y)) L[k++]=x*64+y; HL[v][P]=L;HN[v][P]=k; } }
int main(int argc,char**argv){ sim_init(); const char*mode=argv[1]; long total=0,bad=0;
  if(!strcmp(mode,"reach")){ hits(8); long cases=0,nohit_calls=0;
    #pragma omp parallel for schedule(dynamic,16) reduction(+:cases,bad,total)
    for(int P=0;P<4096;P++){ int cx=P/64,cy=P%64; if(cy<YMIN||cy>=YMAX)continue;
      for(int v=0;v<=8;v++){ int first=-1,fx=0,fy=0; int s=SZ[v];
        for(int k=0;k<HN[v][P];k++){ int p=HL[v][P][k]; St a; a.n=1;a.v[0]=v;a.x[0]=p/64;a.y[0]=p%64; St o;int lossy; int r=click(&a,cx,cy,&o,&lossy); total++;
          if(r!=2||o.n!=1||o.v[0]!=v||lossy){bad++;continue;}
          int ox=o.x[0],oy=o.y[0]; if(!(ox<=cx&&cx<=ox+s-1&&oy<=cy&&cy<=oy+s-1)){bad++;}
          if(first<0){first=1;fx=ox;fy=oy;} else if(ox!=fx||oy!=fy) bad++; cases++; } } }
    printf("LEMMA_REACH instances=%ld mismatches=%ld\n",total,bad); return bad!=0; }
  if(!strcmp(mode,"pair")){ int V=atoi(argv[2]); hits(V); int fx[4096],fy[4096]; for(int P=0;P<4096;P++)fx[P]=-1;
    #pragma omp parallel for schedule(dynamic,4) reduction(+:bad,total)
    for(int P=0;P<4096;P++){ int cx=P/64,cy=P%64; if(cy<YMIN||cy>=YMAX||!HN[V][P])continue; int ex=-1,ey=-1;
      for(int i=0;i<HN[V][P];i++)for(int j=0;j<HN[V][P];j++){ int p=HL[V][P][i],q=HL[V][P][j]; St a;a.n=2;a.v[0]=V;a.x[0]=p/64;a.y[0]=p%64;a.v[1]=V;a.x[1]=q/64;a.y[1]=q%64; canon(&a);
        St o;int lossy; int r=click(&a,cx,cy,&o,&lossy); total++;
        if(r!=2||o.n!=1||o.v[0]!=V+1||lossy){bad++;continue;}
        if(ex<0){ex=o.x[0];ey=o.y[0];} else if(o.x[0]!=ex||o.y[0]!=ey) bad++; } }
    printf("LEMMA_PAIR v=%d instances=%ld mismatches=%ld\n",V,total,bad); return bad!=0; }
  if(!strcmp(mode,"mixed")){ int U=atoi(argv[2]),Wv=atoi(argv[3]); hits(3); long wins=0;
    #pragma omp parallel for schedule(dynamic,4) reduction(+:bad,total,wins)
    for(int P=0;P<4096;P++){ int cx=P/64,cy=P%64; if(cy<YMIN||cy>=YMAX)continue;
      for(int i=0;i<HN[U][P];i++)for(int j=0;j<HN[Wv][P];j++){ int p=HL[U][P][i],q=HL[Wv][P][j]; St a;a.n=2;a.v[0]=U;a.x[0]=p/64;a.y[0]=p%64;a.v[1]=Wv;a.x[1]=q/64;a.y[1]=q%64; canon(&a);
        St o;int lossy; int r=click(&a,cx,cy,&o,&lossy); total++;
        if(r==3){ if(U==2&&Wv==3)wins++; else bad++; } else if(r!=1) bad++; } }
    printf("LEMMA_MIXED u=%d w=%d instances=%ld mismatches=%ld win_events=%ld\n",U,Wv,total,bad,wins); return bad!=0; }
  if(!strcmp(mode,"triple")){ hits(2); long lossyc=0; unsigned long long seed=12345;
    for(int V=0;V<=2;V++){ for(long it=0;it<3000000;it++){ seed=seed*6364136223846793005ULL+1442695040888963407ULL; int P=(seed>>33)%4096; int cy=P%64; if(cy<YMIN||cy>=YMAX||HN[V][P]==0)continue;
        St a;a.n=3; for(int k=0;k<3;k++){ seed=seed*6364136223846793005ULL+1442695040888963407ULL; int p=HL[V][P][(seed>>33)%HN[V][P]]; a.v[k]=V;a.x[k]=p/64;a.y[k]=p%64;} canon(&a);
        St o;int lossy; int r=click(&a,P/64,cy,&o,&lossy); total++; if(r==2&&lossy&&o.n==1&&o.v[0]==V+1) lossyc++; else bad++; } }
    printf("LEMMA_TRIPLE sampled=%ld lossy_single_piece=%ld mismatches=%ld\n",total,lossyc,bad); return bad!=0; }
  return 1; }
