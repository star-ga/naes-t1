// count distinct states at depth 1 and 2 with the adversarial check sim (rsim.h)
#include "rsim.h"
#include <search.h>
typedef struct { uint64_t k[3]; } K; // packed sorted state
static void key(const S*s,K*k){ int a[16]; for(int i=0;i<s->n;i++) a[i]=(s->v[i]<<12)|(s->x[i]<<6)|s->y[i];
  for(int i=1;i<s->n;i++){int t=a[i],j=i-1; while(j>=0&&a[j]>t){a[j+1]=a[j];j--;} a[j+1]=t;}
  memset(k,0,sizeof(*k)); for(int i=0;i<s->n;i++){ k->k[i/4] |= (uint64_t)(a[i]&0xFFFF)<<(16*(i%4)); } k->k[2]|=(uint64_t)s->n<<60; }
static int cmpk(const void*a,const void*b){ return memcmp(a,b,sizeof(K)); }
int main(){ S s0; rstart(&s0);
  K *L1=malloc(sizeof(K)*4096); int n1=0; S *st1=malloc(sizeof(S)*4096); int nl=0; int stat[4]={0}; int lossless1=0;
  for(int P=0;P<4096;P++){ S o; int r=rclick(&s0,P&63,P>>6,&o,NULL); stat[r]++; if(r==R_OK){ key(&o,&L1[n1]); st1[n1]=o; n1++; } }
  qsort(L1,n1,sizeof(K),cmpk); int d1=0; for(int i=0;i<n1;i++) if(i==0||cmpk(&L1[i],&L1[i-1])) d1++;
  printf("DEPTH1 clicks noop=%d fail=%d ok=%d win=%d distinct_ok_states=%d\n",stat[0],stat[1],stat[2],stat[3],d1);
  // dedupe st1
  K *L2=malloc(sizeof(K)*(size_t)4096*4096); long n2=0; int seen=0;
  // unique depth-1 states
  K *u=malloc(sizeof(K)*n1); S *us=malloc(sizeof(S)*n1); int nu=0;
  for(int i=0;i<n1;i++){ K k; key(&st1[i],&k); int dup=0; for(int j=0;j<nu;j++) if(!cmpk(&k,&u[j])){dup=1;break;} if(!dup){u[nu]=k; us[nu]=st1[i]; nu++;} }
  for(int i=0;i<nu;i++) for(int P=0;P<4096;P++){ S o; int r=rclick(&us[i],P&63,P>>6,&o,NULL); if(r==R_OK){ key(&o,&L2[n2++]); } }
  qsort(L2,n2,sizeof(K),cmpk); long d2=0; for(long i=0;i<n2;i++) if(i==0||cmpk(&L2[i],&L2[i-1])) d2++;
  // also count union with depth1 and start
  printf("DEPTH2 from %d distinct d1 states: ok_children=%ld distinct=%ld\n",nu,n2,d2);
  { long newc=0, lossless=0; K ks; key(&s0,&ks); for(long i=0;i<n2;i++){ if(i>0&&!cmpk(&L2[i],&L2[i-1])) continue; int ind1=0; for(int j=0;j<nu;j++) if(!cmpk(&L2[i],&u[j])){ind1=1;break;} if(!ind1 && cmpk(&L2[i],&ks)) newc++; }
    printf("DEPTH2 distinct_not_in_d1_not_start=%ld\n",newc); }
  { int nll=0; for(int j=0;j<nu;j++){ int m=0; for(int i=0;i<us[j].n;i++) m+=1<<us[j].v[i]; if(m==12) nll++; } printf("DEPTH1 lossless=%d of %d\n",nll,nu); }
  return 0; }
