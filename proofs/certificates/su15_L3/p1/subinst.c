// P3 check of the relaxed tables D[T][pos] against exact BFS of the REAL simulator on the sub-instance made of the fruits of T only.
// Required: D[T][pos] <= bfs[pos] for every pos with bfs<=cap (relaxation never exceeds a real route). Reported: how many positions are equal / strictly smaller (interference-free gain).
#include "p1dp.h"
static int cmpq(const void*a,const void*b){return cmpst((const St*)a,(const St*)b);}
int main(int argc,char**argv){ int cap=atoi(argv[1]); int maxsz=atoi(argv[2]); l3_setup(); St s=l3_start(); if(dp_tables(&s)<0)return 1; int n=s.n;
  long tot_bad=0,tot_eq=0,tot_lt=0,tot_cmp=0; int ntab=0;
  for(int T=1;T<(1<<n);T++){ if(vm[T]<1)continue; int sz=pc9(T); if(sz<2||sz>maxsz)continue; int v=vm[T];
    St s0; s0.n=0; for(int i=0;i<n;i++) if(T>>i&1){s0.v[s0.n]=s.v[i];s0.x[s0.n]=s.x[i];s0.y[s0.n]=s.y[i];s0.n++;} canon(&s0);
    static int best[NP]; for(int i=0;i<NP;i++)best[i]=99;
    size_t cap_n=1<<22; St*fr=malloc(sizeof(St)*cap_n),*nx=malloc(sizeof(St)*cap_n*2); size_t nf=1; fr[0]=s0;
    for(int d=1;d<=cap;d++){ size_t nn=0; size_t cn=cap_n*2;
      for(size_t i=0;i<nf;i++){ static St loc[4096]; int nl=0; for(int cy=0;cy<64;cy++)for(int cx=0;cx<64;cx++){ St o;int ls; int r=click(&fr[i],cx,cy,&o,&ls); if(r!=2||ls)continue;
          if(d==cap){ if(o.n==1&&o.v[0]==v){int pos=o.x[0]*64+o.y[0]; if(best[pos]>d)best[pos]=d;} continue; } loc[nl++]=o; }
        qsort(loc,nl,sizeof(St),cmpq); for(int k=0;k<nl;k++){ if(k&&cmpst(&loc[k],&loc[k-1])==0)continue; if(nn<cn)nx[nn++]=loc[k]; else {fprintf(stderr,"overflow\n");return 2;} } }
      if(d==cap)break; qsort(nx,nn,sizeof(St),cmpq); size_t u=0; for(size_t k=0;k<nn;k++){ if(k&&cmpst(&nx[k],&nx[k-1])==0)continue; nx[u++]=nx[k]; }
      if(u>cap_n){fprintf(stderr,"overflow u\n");return 2;} memcpy(fr,nx,sizeof(St)*u); nf=u;
      for(size_t k=0;k<nf;k++) if(fr[k].n==1&&fr[k].v[0]==v){int pos=fr[k].x[0]*64+fr[k].y[0]; if(best[pos]>d)best[pos]=d;} }
    long bad=0,eq=0,lt=0,cmp=0; for(int pos=0;pos<NP;pos++){ int dd=D[T][pos]; if(best[pos]>cap&&dd>cap)continue; cmp++; if(dd>best[pos])bad++; else if(dd==best[pos])eq++; else lt++; }
    printf("SUBINST T=%03x size=%d value=%d cap=%d positions_with_either<=cap=%ld dp>bfs(UNSOUND)=%ld equal=%ld dp<bfs(relaxed)=%ld\n",T,sz,v,cap,cmp,bad,eq,lt); fflush(stdout);
    tot_bad+=bad;tot_eq+=eq;tot_lt+=lt;tot_cmp+=cmp;ntab++; free(fr);free(nx); }
  printf("SUBINST_TOTAL tables=%d positions=%ld unsound=%ld equal=%ld relaxed_smaller=%ld\n",ntab,tot_cmp,tot_bad,tot_eq,tot_lt); return tot_bad!=0; }
