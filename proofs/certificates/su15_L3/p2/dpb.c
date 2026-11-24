// p2: interference-free lineage relaxation, tables derived from the EXACT simulator (simp2.h) by calling click(), no analytic formulas.
// Modes: buildW | root | eval FILE | cons FILE | exh
#include "simp2.h"
#include <omp.h>
#define INF 255
#define CAPC 14
static int posok[4][4096]; static uint16_t *HitL[4][4096]; static int HitN[4][4096];
static int FP[4][4096], GP[4][4096];           // move landing / merge landing per value and click P (-1 none)
static uint64_t (*W)[64];                       // W[a][b/64] bit: v3@a & v2@b win with one click (some P), per simulator
static int valid_v(int v,int x,int y){int s=SZ[v]; return x>=0&&x<=WD-s&&y>=YMIN&&y<=YMAX&&y<=WD-s;}
static int zc(int v,int x,int y){int c0=x+SZ[v]/2,c1=y+SZ[v]/2; for(int z=0;z<NZ;z++) if(c0>=ZONES[z].zx&&c0<ZONES[z].zx+ZONES[z].zw&&c1>=ZONES[z].zy&&c1<ZONES[z].zy+ZONES[z].zh) return 1; return 0;}
static void build_tables(void){
  for(int v=0;v<4;v++){ for(int x=0;x<64;x++)for(int y=0;y<64;y++) posok[v][x*64+y]=valid_v(v,x,y);
    for(int P=0;P<4096;P++){ int cx=P/64,cy=P%64; HitN[v][P]=0; FP[v][P]=-1; GP[v][P]=-1;
      if(cy<=YMIN-1||cy>=YMAX){ HitL[v][P]=NULL; continue; }
      uint16_t*L=malloc(sizeof(uint16_t)*1200); int k=0;
      for(int x=0;x<64;x++)for(int y=0;y<64;y++) if(posok[v][x*64+y]&&inrange(cx,cy,v,x,y)) L[k++]=x*64+y;
      HitL[v][P]=L; HitN[v][P]=k;
      if(k){ int p=L[0]; St s; s.n=1; s.v[0]=v; s.x[0]=p/64; s.y[0]=p%64; St o; int lossy; int r=click(&s,cx,cy,&o,&lossy);
        if(r==2 && o.n==1 && o.v[0]==v) FP[v][P]=o.x[0]*64+o.y[0];
        if(v<3){ St t; t.n=2; for(int i=0;i<2;i++){t.v[i]=v;t.x[i]=p/64;t.y[i]=p%64;} r=click(&t,cx,cy,&o,&lossy); if(r==2&&o.n==1&&o.v[0]==v+1&&!lossy) GP[v][P]=o.x[0]*64+o.y[0]; }
      } } }
}
static void buildW(const char*fn){
  W=calloc(4096,sizeof(uint64_t)*64); long wins=0;
  #pragma omp parallel for schedule(dynamic,8) reduction(+:wins)
  for(int P=0;P<4096;P++){ int cx=P/64,cy=P%64; if(cy<YMIN||cy>=YMAX)continue;
    for(int ia=0;ia<HitN[3][P];ia++){ int a=HitL[3][P][ia];
      for(int ib=0;ib<HitN[2][P];ib++){ int b=HitL[2][P][ib]; St s; s.n=2; s.v[0]=2;s.x[0]=b/64;s.y[0]=b%64; s.v[1]=3;s.x[1]=a/64;s.y[1]=a%64; canon(&s);
        St o;int lossy; if(click(&s,cx,cy,&o,&lossy)==3){
          #pragma omp atomic
          W[a][b/64]|=(1ULL<<(b%64)); wins++; } }
      // a hit, b stationary in a zone (b not hit)
      for(int b=0;b<4096;b++){ if(!posok[2][b]||!zc(2,b/64,b%64))continue; int hitb=0; for(int ib=0;ib<HitN[2][P];ib++) if(HitL[2][P][ib]==b){hitb=1;break;} if(hitb)continue;
        St s; s.n=2; s.v[0]=2;s.x[0]=b/64;s.y[0]=b%64; s.v[1]=3;s.x[1]=a/64;s.y[1]=a%64; canon(&s); St o;int lossy; if(click(&s,cx,cy,&o,&lossy)==3){
          #pragma omp atomic
          W[a][b/64]|=(1ULL<<(b%64)); wins++; } } }
    for(int ib=0;ib<HitN[2][P];ib++){ int b=HitL[2][P][ib];
      for(int a=0;a<4096;a++){ if(!posok[3][a]||!zc(3,a/64,a%64))continue; int hita=0; for(int ia=0;ia<HitN[3][P];ia++) if(HitL[3][P][ia]==a){hita=1;break;} if(hita)continue;
        St s; s.n=2; s.v[0]=2;s.x[0]=b/64;s.y[0]=b%64; s.v[1]=3;s.x[1]=a/64;s.y[1]=a%64; canon(&s); St o;int lossy; if(click(&s,cx,cy,&o,&lossy)==3){
          #pragma omp atomic
          W[a][b/64]|=(1ULL<<(b%64)); wins++; } } } }
  long pairs=0; for(int a=0;a<4096;a++)for(int w=0;w<64;w++)pairs+=__builtin_popcountll(W[a][w]);
  FILE*f=fopen(fn,"wb"); fwrite(W,sizeof(uint64_t)*64,4096,f); fclose(f);
  printf("BUILDW events=%ld distinct_pairs=%ld file=%s\n",wins,pairs,fn);
}
static void loadW(const char*fn){ W=malloc(sizeof(uint64_t)*64*4096); FILE*f=fopen(fn,"rb"); if(!f){fprintf(stderr,"no %s\n",fn);exit(3);} if(fread(W,sizeof(uint64_t)*64,4096,f)!=4096){fprintf(stderr,"short W\n");exit(3);} fclose(f); }

static __thread uint8_t D[512][4096], A[512][4096]; static __thread int vm[512];
typedef struct { long cells[CAPC+1]; int normal, joint, best; int c3,c2; int T3,T2; } Res;
static long g_cells[CAPC+1];
static uint16_t *InvL[4][4096]; static int InvN[4][4096];
static void build_inv(void){ for(int v=0;v<4;v++){ int cnt[4096]={0}; for(int P=0;P<4096;P++) for(int k=0;k<HitN[v][P];k++) cnt[HitL[v][P][k]]++;
    for(int p=0;p<4096;p++){ InvL[v][p]=malloc(2*(cnt[p]+1)); InvN[v][p]=0; }
    for(int P=0;P<4096;P++) for(int k=0;k<HitN[v][P];k++){ int p=HitL[v][P][k]; InvL[v][p][InvN[v][p]++]=P; } } }
// unit-cost Dijkstra over positions: p --(click P with p in Hit(P))--> FP[P]; exact same relation as the min-over-hits relaxation
static void closure(int m,int v){
  for(int k=0;k<CAPC;k++){ for(int p=0;p<4096;p++) if(D[m][p]==k){ for(int i=0;i<InvN[v][p];i++){ int q=FP[v][InvL[v][p][i]]; if(q>=0&&k+1<D[m][q]) D[m][q]=k+1; } } }
  for(int P=0;P<4096;P++){ int best=INF; for(int k=0;k<HitN[v][P];k++){int d=D[m][HitL[v][P][k]]; if(d<best)best=d;} A[m][P]=best; }
}
// returns lower bound (relaxed) on clicks to win from state s (lossless states only); INF if none
static int g_sub=0;
static int bound(const St*s,Res*R){
  int n=s->n; memset(R,0,sizeof *R); R->best=INF; R->normal=R->joint=INF;
  int tot=0; for(int i=0;i<n;i++){ if(s->v[i]>3) return INF; tot+=1<<s->v[i]; } if(tot!=12&&!g_sub) return INF;
  int full=(1<<n)-1;
  for(int m=1;m<=full;m++){ int mass=0; for(int i=0;i<n;i++) if(m>>i&1) mass+=1<<s->v[i]; vm[m]=(mass&(mass-1))?-1:__builtin_ctz(mass); if(vm[m]>3)vm[m]=-1; }
  for(int pc=1;pc<=n;pc++) for(int m=1;m<=full;m++){ if(__builtin_popcount(m)!=pc||vm[m]<0)continue; int v=vm[m];
    memset(D[m],INF,4096);
    if(pc==1){ int i=__builtin_ctz(m); D[m][s->x[i]*64+s->y[i]]=0; }
    else { if(v==0) continue;
      for(int t1=(m-1)&m;t1>0;t1=(t1-1)&m){ int t2=m^t1; if(t1<t2)continue; if(vm[t1]!=v-1||vm[t2]!=v-1)continue;
        for(int P=0;P<4096;P++){ int q=GP[v-1][P]; if(q<0)continue; int c=A[t1][P]+A[t2][P]+1; if(A[t1][P]>=INF||A[t2][P]>=INF)continue; if(c<D[m][q]&&c<=CAPC)D[m][q]=c; } } }
    closure(m,v); }
  if(g_sub) return 0;
  for(int T3=1;T3<=full;T3++){ if(vm[T3]!=3)continue; int T2=full^T3; if(T2==0||vm[T2]!=2)continue;
    int c3=INF,c2=INF; for(int p=0;p<4096;p++){ if(posok[3][p]&&zc(3,p/64,p%64)&&D[T3][p]<c3)c3=D[T3][p]; if(posok[2][p]&&zc(2,p/64,p%64)&&D[T2][p]<c2)c2=D[T2][p]; }
    int normal=(c3>=INF||c2>=INF)?INF:c3+c2;
    int joint=INF; if(W){ uint64_t fin2[64]={0}; for(int b=0;b<4096;b++) if(D[T2][b]<INF) fin2[b/64]|=1ULL<<(b%64);
      for(int a=0;a<4096;a++){ if(D[T3][a]>=INF)continue; for(int w=0;w<64;w++){ uint64_t x=W[a][w]&fin2[w]; while(x){ int b=w*64+__builtin_ctzll(x); x&=x-1; int c=D[T3][a]+D[T2][b]+1; if(c<joint)joint=c; } } } }
    int bst=normal<joint?normal:joint;
    if(bst<R->best){R->best=bst;R->normal=normal;R->joint=joint;R->c3=c3;R->c2=c2;R->T3=T3;R->T2=T2;}
    }
  return R->best; }

static int cmpq(const void*a,const void*b){ return cmpst((const St*)a,(const St*)b); }
// exact BFS distance (real simulator, all 4096 clicks, exact dedup) up to dmax or layer cap; returns dist or -1 (> reached depth)
static int bfs_dist(const St*s0,int dmax,long cap,int*reached,long*nodes){
  St*cur=malloc(sizeof(St)*cap),*nxt=malloc(sizeof(St)*cap); long nc=1; cur[0]=*s0; *reached=0; *nodes=0; int res=-1;
  for(int d=1;d<=dmax&&res<0;d++){ long nn=0; int over=0;
    for(long i=0;i<nc&&res<0;i++) for(int P=0;P<4096;P++){ St o;int lossy; int r=click(&cur[i],P/64,P%64,&o,&lossy); if(r==3){res=d;break;} if(r!=2||lossy)continue; if(nn<cap)nxt[nn++]=o; else over=1; }
    if(res>=0)break; if(over){break;}
    qsort(nxt,nn,sizeof(St),cmpq); long k=0; for(long i=0;i<nn;i++) if(i==0||cmpst(&nxt[i],&nxt[i-1])!=0) nxt[k++]=nxt[i]; nn=k;
    *nodes+=nn; *reached=d; St*t=cur;cur=nxt;nxt=t; nc=nn; }
  free(cur);free(nxt); return res; }

// ---- IDA*: bound B from state, h = bound(); children = every distinct real successor over all 4096 clicks (lossless only) ----
typedef struct { St s; int h; int cx,cy; } Ch;
static long nodes_d[16], pruned_h[16], children_d[16], tt_hits[16]; static int path_cx[16],path_cy[16]; static uint64_t *TT; static size_t TTN;
static int tt_seen(const St*s,int g){ uint64_t h=hst(s)^((uint64_t)(g+1)*0x9E3779B97F4A7C15ULL); size_t k=h&(TTN-1); while(TT[k]){ if(TT[k]==(h|1)) return 1; k=(k+1)&(TTN-1);} TT[k]=h|1; return 0; }
static int ida(const St*s,int g,int B){
  Res R; int h=bound(s,&R); nodes_d[g]++; if(g+h>B){ pruned_h[g]++; return 0; }
  Ch*ch=malloc(sizeof(Ch)*4096); int nc=0; St*seen=malloc(sizeof(St)*4096); int ns=0;
  for(int P=0;P<4096;P++){ St o;int lossy; int r=click(s,P/64,P%64,&o,&lossy); if(r==3){ path_cx[g]=P/64;path_cy[g]=P%64; if(g+1<=B){ printf("IDA_WIN depth %d\n",g+1); free(ch);free(seen); return g+1; } continue; }
    if(r!=2||lossy)continue; int dup=0; for(int i=0;i<ns;i++) if(cmpst(&seen[i],&o)==0){dup=1;break;} if(dup)continue; seen[ns++]=o; ch[nc].s=o; ch[nc].cx=P/64; ch[nc].cy=P%64; nc++; }
  children_d[g]+=nc;
  #pragma omp parallel for schedule(dynamic,1)
  for(int i=0;i<nc;i++){ Res R2; ch[i].h=bound(&ch[i].s,&R2); }
  // order by h ascending
  for(int i=1;i<nc;i++){ Ch t=ch[i]; int j=i-1; while(j>=0&&ch[j].h>t.h){ch[j+1]=ch[j];j--;} ch[j+1]=t; }
  int res=0;
  for(int i=0;i<nc&&!res;i++){ if(g+1+ch[i].h>B){ pruned_h[g+1]++; nodes_d[g+1]++; continue; }
    if(tt_seen(&ch[i].s,g+1)){ tt_hits[g+1]++; continue; }
    path_cx[g]=ch[i].cx; path_cy[g]=ch[i].cy; res=ida(&ch[i].s,g+1,B); }
  free(ch); free(seen); return res; }

// sub-instance check: D[full][pos] (relaxed lineage cost to ONE piece at pos) vs exact BFS min clicks to a single piece at pos (real simulator, all 4096 clicks)
static int subcheck(const St*s0,int dmax,long cap){
  g_sub=1; Res R; bound(s0,&R); g_sub=0; int full=(1<<s0->n)-1; int v=vm[full]; int bf[4096]; for(int i=0;i<4096;i++)bf[i]=99;
  St*cur=malloc(sizeof(St)*cap),*nxt=malloc(sizeof(St)*cap); long nc=1; cur[0]=*s0;
  for(int d=1;d<=dmax;d++){ long nn=0;
    for(long i=0;i<nc;i++) for(int P=0;P<4096;P++){ St o;int lossy; int r=click(&cur[i],P/64,P%64,&o,&lossy); if(r!=2||lossy)continue; if(nn<cap)nxt[nn++]=o; if(o.n==1&&o.v[0]==v){int p=o.x[0]*64+o.y[0]; if(bf[p]>d)bf[p]=d;} }
    qsort(nxt,nn,sizeof(St),cmpq); long k=0; for(long i=0;i<nn;i++) if(i==0||cmpst(&nxt[i],&nxt[i-1])!=0) nxt[k++]=nxt[i]; nn=k;
    St*t=cur;cur=nxt;nxt=t; nc=nn; }
  long cmp=0,eq=0,less=0,viol=0,dponly=0; for(int p=0;p<4096;p++){ int dd=D[full][p]; int b=bf[p]; if(b<=dmax){ cmp++; if(dd>b)viol++; else if(dd==b)eq++; else less++; } else if(dd<=dmax) dponly++; }
  printf("SUBCHECK n=%d value=%d dmax=%d positions_reached_by_BFS=%ld dp_equal=%ld dp_less=%ld dp_greater(VIOLATION)=%ld dp_only_within_dmax=%ld\n",s0->n,v,dmax,cmp,eq,less,viol,dponly);
  free(cur);free(nxt); return viol!=0; }
static void readstate(const char*line,St*s){ int off,n; const char*p=line; sscanf(p,"%d%n",&n,&off); p+=off; s->n=n; for(int i=0;i<n;i++){int v,x,y;sscanf(p,"%d %d %d%n",&v,&x,&y,&off);p+=off;s->v[i]=v;s->x[i]=x;s->y[i]=y;} canon(s); }
static const int START[9][3]={{0,55,23},{0,61,23},{0,31,22},{0,31,15},{0,12,23},{0,8,28},{1,46,22},{1,30,32},{1,18,16}};
int main(int argc,char**argv){ setvbuf(stdout,NULL,_IOLBF,0); sim_init(); const char*mode=argv[1]; build_tables(); build_inv();
  if(!strcmp(mode,"tables")){ long mv=0,mg=0,nohit=0; for(int v=0;v<4;v++)for(int P=0;P<4096;P++){ if(FP[v][P]>=0)mv++; if(v<3&&GP[v][P]>=0)mg++; if(P%64&&0)nohit++; } printf("TABLES move_entries=%ld merge_entries=%ld\n",mv,mg); return 0; }
  if(!strcmp(mode,"buildW")){ buildW(argv[2]); return 0; }
  loadW(argv[2]);
  if(!strcmp(mode,"root")){ St s; s.n=9; for(int i=0;i<9;i++){s.v[i]=START[i][0];s.x[i]=START[i][1];s.y[i]=START[i][2];} canon(&s); Res R; int b=bound(&s,&R);
    printf("ROOT_BOUND %d normal=%d joint=%d c3=%d c2=%d T3=%03x T2=%03x\n",b,R.normal,R.joint,R.c3,R.c2,R.T3,R.T2);
    long cells[CAPC+1]={0}; int full=511; for(int m=1;m<=full;m++){ if(vm[m]<0)continue; for(int p=0;p<4096;p++) if(D[m][p]<=CAPC)cells[D[m][p]]++; }
    printf("EXH cells(T,pos) by cost:"); for(int k=0;k<=CAPC;k++)printf(" %d:%ld",k,cells[k]); printf("\n");
    int nm=0; for(int m=1;m<=full;m++) if(vm[m]>=0)nm++; printf("EXH valid_subsets=%d\n",nm); return 0; }
  if(!strcmp(mode,"ida")){ int B=atoi(argv[3]); TTN=1ULL<<26; TT=calloc(TTN,8); St s; s.n=9; for(int i=0;i<9;i++){s.v[i]=START[i][0];s.x[i]=START[i][1];s.y[i]=START[i][2];} canon(&s);
    if(argc>4){ readstate(argv[4],&s); }
    double t0=omp_get_wtime(); int r=ida(&s,0,B);
    printf("IDA bound=%d result=%s\n",B,r?"WIN_FOUND":"NO_WIN_WITHIN_BOUND");
    for(int d=0;d<=B;d++) printf("IDA depth %d: nodes=%ld pruned_by_h=%ld children_generated=%ld tt_hits=%ld\n",d,nodes_d[d],pruned_h[d],children_d[d],tt_hits[d]);
    if(r){ printf("IDA_ROUTE"); for(int d=0;d<r;d++) printf(" %d,%d",path_cx[d],path_cy[d]); printf("\n"); }
    printf("IDA sec=%.0f\n",omp_get_wtime()-t0); return 0; }
  if(!strcmp(mode,"sub")){ FILE*f=fopen(argv[3],"r"); int dmax=atoi(argv[4]); long cap=atol(argv[5]); char buf[4096]; int bad=0; while(fgets(buf,sizeof buf,f)){ St s; readstate(buf,&s); printf("state: %s",buf); bad|=subcheck(&s,dmax,cap); } return bad; }
  if(!strcmp(mode,"bfs")){ FILE*f=fopen(argv[3],"r"); int dmax=atoi(argv[4]); long cap=atol(argv[5]); char buf[4096]; long nst=0,viol=0,eq=0,lt=0,unk=0;
    while(fgets(buf,sizeof buf,f)){ St s; readstate(buf,&s); Res R; int h=bound(&s,&R); int reached; long nodes; int d=bfs_dist(&s,dmax,cap,&reached,&nodes); nst++;
      if(d>=0){ if(h>d){viol++; printf("VIOLATION h=%d exact=%d: %s",h,d,buf);} else if(h==d)eq++; else lt++; printf("BFSROW n=%d h=%d exact=%d nodes=%ld\n",s.n,h,d,nodes); }
      else { unk++; if(h>reached+1){viol++; printf("VIOLATION h=%d but no win within depth %d: %s",h,reached,buf);} printf("BFSROW n=%d h=%d exact=>%d nodes=%ld\n",s.n,h,reached,nodes); } }
    printf("BFSCHECK states=%ld exact_found_h_equal=%ld exact_found_h_less=%ld not_found_within_depth=%ld violations=%ld\n",nst,eq,lt,unk,viol); return viol!=0; }
  if(!strcmp(mode,"eval")||!strcmp(mode,"cons")){ FILE*f=fopen(argv[3],"r"); char buf[4096]; long nst=0,viol=0,children=0;
    while(fgets(buf,sizeof buf,f)){ St s; readstate(buf,&s); Res R; int h=bound(&s,&R); nst++;
      if(!strcmp(mode,"eval")){ printf("H %d\n",h>=INF?-1:h); continue; }
      // consistency h(s) <= 1 + h(s') for every real successor
      St seen[4096]; int ns=0;
      for(int P=0;P<4096;P++){ St o; int lossy; int r=click(&s,P/64,P%64,&o,&lossy); if(r!=2&&r!=3)continue; if(r==2&&lossy)continue; int hs;
        if(r==3) hs=0; else { int dup=0; for(int i=0;i<ns;i++) if(cmpst(&seen[i],&o)==0){dup=1;break;} if(dup)continue; seen[ns++]=o; Res R2; hs=bound(&o,&R2); }
        children++; if(h<INF && hs<INF && h>hs+1){ viol++; if(viol<=10) printf("VIOLATION h=%d child_h=%d click=%d,%d r=%d state: %s",h,hs,P/64,P%64,r,buf); }
        if(h>=INF && hs<INF){ viol++; if(viol<=10) printf("VIOLATION parent INF child %d state: %s",hs,buf); } }
      if(nst%10==0){fprintf(stderr,"cons %ld states %ld children %ld viol\n",nst,children,viol);} }
    printf("CONS states=%ld children=%ld violations=%ld\n",nst,children,viol); return viol!=0; }
  return 1; }
