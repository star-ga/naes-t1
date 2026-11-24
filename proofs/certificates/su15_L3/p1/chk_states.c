// P2/P3 state-level checks. For every state of a set and EVERY click position (cx 0..63, cy 0..63 = 4096), classify the exact simulator's outcome:
//  |S|=0 noop unchanged | |S|=1 ok, non-lossy, hit piece -> (v,landc(v,P)), others unchanged | |S|=2 same value: ok merge at mean landing, non-lossy; different: fail or mid-flight win
//  |S|>=3 fail or lossy (never a clean ok). mass: clean ok conserves mass, lossy lowers it.  S = pieces in range (computed independently of click()).
// sets: "bfs2" = all distinct states reachable in <=2 clicks from the L3 start (also prints the counts per depth) ; "rand N seed" = N random clustered states.
#include "simc.h"
#include <omp.h>
static inline void landc(int v,int cx,int cy,int*lx,int*ly){ int x=cx-SZ[v]/2,y=cy-SZ[v]/2; clampp(v,&x,&y); *lx=x;*ly=y; }
static void setup(void){ NZ=2; ZONES[0]=(Zone){5,46,9,9}; ZONES[1]=(Zone){19,46,9,9}; NG=2; GV[0]=3;GC[0]=1;GV[1]=2;GC[1]=1; }
static int cmpq(const void*a,const void*b){return cmpst((const St*)a,(const St*)b);}
static int mass(const St*s){int m=0;for(int i=0;i<s->n;i++)m+=1<<s->v[i];return m;}
typedef struct{long c[8]; long wins; long bad; long massbad; long lossy;} Tally; // c[0..2]=|S|=0,1,2same ; c[3]=|S|=2 diff ; c[4]=|S|>=3
static void check_state(const St*s,Tally*T){
  for(int cy=0;cy<64;cy++)for(int cx=0;cx<64;cx++){
    int hit[9],m=0; if(cy>=YMIN&&cy<YMAX) for(int i=0;i<s->n;i++) if(inrange(cx,cy,s->v[i],s->x[i],s->y[i])) hit[m++]=i;
    St o;int ls; int r=click(s,cx,cy,&o,&ls); int ok=0;
    if(r==3){ T->wins++; int m0=mass(s); int s23=(s->n==2&&((s->v[0]==2&&s->v[1]==3)||(s->v[0]==3&&s->v[1]==2))); int o23=(o.n==2&&((o.v[0]==2&&o.v[1]==3)||(o.v[0]==3&&o.v[1]==2)));
      ok=(m0>=12)&&(m0!=12||s23||o23); if(!ok){T->bad++; if(T->bad<3)printf("WIN SHAPE FAIL\n");} continue; }
    if(m==0){ T->c[0]++; ok=(r==0); }
    else if(m==1){ T->c[1]++; int i=hit[0]; int lx,ly; landc(s->v[i],cx,cy,&lx,&ly);
      St e; e.n=0; for(int k=0;k<s->n;k++) if(k!=i){e.v[e.n]=s->v[k];e.x[e.n]=s->x[k];e.y[e.n]=s->y[k];e.n++;} e.v[e.n]=s->v[i];e.x[e.n]=lx;e.y[e.n]=ly;e.n++; canon(&e);
      ok=(r==2&&!ls&&cmpst(&e,&o)==0); }
    else if(m==2){ int i=hit[0],j=hit[1];
      if(s->v[i]==s->v[j]){ T->c[2]++; int v=s->v[i],nv=v+1; int lx,ly; landc(v,cx,cy,&lx,&ly); int ax=lx+SZ[v]/2-SZ[nv]/2, ay=ly+SZ[v]/2-SZ[nv]/2; clampp(nv,&ax,&ay);
        St e; e.n=0; for(int k=0;k<s->n;k++) if(k!=i&&k!=j){e.v[e.n]=s->v[k];e.x[e.n]=s->x[k];e.y[e.n]=s->y[k];e.n++;} e.v[e.n]=nv;e.x[e.n]=ax;e.y[e.n]=ay;e.n++; canon(&e);
        ok=(r==2&&!ls&&cmpst(&e,&o)==0); }
      else { T->c[3]++; ok=(r==1); } }
    else { T->c[4]++; ok=(r==1)||(r==2&&ls); }
    if(!ok){ T->bad++; if(T->bad<3)printf("CLASS FAIL m=%d r=%d P=(%d,%d)\n",m,r,cx,cy); }
    if(r==2){ int m0=mass(s),m1=mass(&o); if(ls){T->lossy++; if(!(m1<m0))T->massbad++;} else if(m1!=m0)T->massbad++; }
  }
}
static St start(void){ St s; s.n=9; int init[9][3]={{0,55,23},{0,61,23},{0,31,22},{0,31,15},{0,12,23},{0,8,28},{1,46,22},{1,30,32},{1,18,16}}; for(int i=0;i<9;i++){s.v[i]=init[i][0];s.x[i]=init[i][1];s.y[i]=init[i][2];} canon(&s); return s; }
static St* expand(St*fr,size_t nf,size_t*nout){ // all distinct states reachable by one click (r==2 or r==3 excluded? r==2 only; a win ends the game)
  size_t cap=nf*4100+10; St*buf=malloc(sizeof(St)*cap); size_t nb=0;
  for(size_t i=0;i<nf;i++){ St loc[4096]; int nl=0; for(int cy=0;cy<64;cy++)for(int cx=0;cx<64;cx++){ St o;int ls; int r=click(&fr[i],cx,cy,&o,&ls); if(r!=2)continue; loc[nl++]=o; }
    qsort(loc,nl,sizeof(St),cmpq); for(int k=0;k<nl;k++){ if(k&&cmpst(&loc[k],&loc[k-1])==0)continue; if(nb<cap)buf[nb++]=loc[k]; } }
  qsort(buf,nb,sizeof(St),cmpq); size_t u=0; for(size_t k=0;k<nb;k++){ if(k&&cmpst(&buf[k],&buf[k-1])==0)continue; buf[u++]=buf[k]; } *nout=u; return buf; }
int main(int argc,char**argv){ setup(); Tally tot; memset(&tot,0,sizeof tot); size_t n=0; St*set=NULL;
  if(!strcmp(argv[1],"bfs2")){ St s0=start(); size_t n1,n2; St*d1=expand(&s0,1,&n1); printf("DEPTH1 distinct_states=%zu\n",n1); St*d2=expand(d1,n1,&n2); printf("DEPTH2 distinct_states=%zu (all-time; may include depth1 states)\n",n2);
    int skip=argc>2?atoi(argv[2]):0, mod=argc>3?atoi(argv[3]):1; // optional sharding
    set=malloc(sizeof(St)*(1+n1+n2)); n=0; set[n++]=s0; for(size_t i=0;i<n1;i++)set[n++]=d1[i]; for(size_t i=0;i<n2;i++)set[n++]=d2[i];
    if(mod>1){ size_t k=0; for(size_t i=0;i<n;i++) if((int)(i%mod)==skip) set[k++]=set[i]; n=k; } }
  else if(!strcmp(argv[1],"mass12")){ n=atol(argv[2]); unsigned seed=atoi(argv[3]); srand(seed); set=malloc(sizeof(St)*n);
    static const int shapes[][9]={{3,2,-1},{3,1,1,-1},{2,2,2,-1},{3,1,0,0,-1},{2,2,1,1,-1},{3,0,0,0,0,-1},{2,2,1,0,0,-1},{3,2,-1},{3,2,-1},{2,1,1,1,1,-1},{3,1,1,-1}};
    int ns=sizeof(shapes)/sizeof(shapes[0]);
    for(size_t i=0;i<n;i++){ St s; s.n=0; const int*sh=shapes[rand()%ns]; int zc[2][2]={{9,50},{23,50}}; int zi=rand()%2; for(int k=0;sh[k]>=0;k++){ int v=sh[k],z=SZ[v]; int x,y; int r=rand()%3; if(r==0){x=zc[rand()%2][0]-z/2+rand()%13-6;y=zc[0][1]-z/2+rand()%13-6;} else if(r==1){x=zc[zi][0]-z/2+rand()%25-12;y=zc[zi][1]-z/2+rand()%25-12;} else {x=rand()%64;y=10+rand()%54;}
        if(x<0)x=0; if(x>64-z)x=64-z; if(y<YMIN)y=YMIN; if(y>63)y=63; if(y>64-z)y=64-z; s.v[s.n]=v;s.x[s.n]=x;s.y[s.n]=y;s.n++; } canon(&s); set[i]=s; } }
  else { n=atol(argv[2]); unsigned seed=atoi(argv[3]); srand(seed); set=malloc(sizeof(St)*n);
    for(size_t i=0;i<n;i++){ St s; s.n=2+rand()%8; int cxc=rand()%64,cyc=10+rand()%50; for(int k=0;k<s.n;k++){ s.v[k]=rand()%4; int z=SZ[s.v[k]]; int x=cxc+rand()%21-10,y=cyc+rand()%21-10; if(rand()%4==0){x=rand()%64;y=10+rand()%54;} if(x<0)x=0; if(x>64-z)x=64-z; if(y<YMIN)y=YMIN; if(y>63)y=63; if(y>64-z)y=64-z; s.x[k]=x;s.y[k]=y;} canon(&s); set[i]=s; } }
  #pragma omp parallel
  { Tally T; memset(&T,0,sizeof T);
    #pragma omp for schedule(dynamic,64)
    for(long i=0;i<(long)n;i++) check_state(&set[i],&T);
    #pragma omp critical
    { for(int k=0;k<8;k++)tot.c[k]+=T.c[k]; tot.bad+=T.bad; tot.wins+=T.wins; tot.massbad+=T.massbad; tot.lossy+=T.lossy; } }
  printf("CHK_STATES set=%s states=%zu clicks=%zu |S|0=%ld |S|1=%ld |S|2same=%ld |S|2diff=%ld |S|>=3=%ld win_clicks=%ld lossy_clicks=%ld class_violations=%ld mass_violations=%ld\n",argv[1],n,n*4096,tot.c[0],tot.c[1],tot.c[2],tot.c[3],tot.c[4],tot.wins,tot.lossy,tot.bad,tot.massbad);
  return (tot.bad||tot.massbad)!=0; }
