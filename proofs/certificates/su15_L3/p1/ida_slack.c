// SELF-ATTACK variant of ida.c (shorter-route lens): pruning weakened by SLACK clicks.
// P4 bounded search from the level 3 start state with h = relaxed lineage bound (p1dp.h).  Every expanded state is clicked at all 4096 positions (cx,cy in 0..63).
// usage: ida B flags mode seed shard mod [noroot]
//   mode "exh"  : exhaustive within the bound (nodes with depth+h>B pruned).  shard/mod splits the ROOT's children over processes.
//   mode "find" : randomised first-fit DFS over nodes with depth+h<=B (positive control: finds a route of length <= B).
// Rules (named in PROOF.md): R_MASS (state mass != 12 or a piece value >3 cannot win), R_H (depth+h>B), R_TT (state already refuted with >= remaining budget).
#include "p1dp.h"
#include <time.h>
static int B,FLAGS,MODE_FIND,SLACK=0; static uint64_t rng=88172645463325252ULL; static uint64_t rnd(){rng^=rng<<13;rng^=rng>>7;rng^=rng<<17;return rng;}
typedef struct{long expanded,children,pr_mass,pr_h,pr_tt,wins,clicks,noop,fail,lossy,hcalls; } Cnt; static Cnt C[16];
static int route[16][2]; static int found=0,foundlen=0,foundr[16][2];
#define TTN (1<<24)
static uint64_t ttk[TTN]; static uint8_t ttr[TTN];
static int tt_refuted(uint64_t h,int rem){ size_t k=h&(TTN-1); for(int p=0;p<8;p++){ size_t j=(k+p)&(TTN-1); if(ttk[j]==h) return ttr[j]>=rem; if(!ttk[j])return 0;} return 0; }
static void tt_put(uint64_t h,int rem){ size_t k=h&(TTN-1); for(int p=0;p<8;p++){ size_t j=(k+p)&(TTN-1); if(ttk[j]==h){ if(ttr[j]<rem)ttr[j]=rem; return;} if(!ttk[j]){ttk[j]=h;ttr[j]=rem;return;} } }
static int cmpq(const void*a,const void*b){return cmpst((const St*)a,(const St*)b);}
typedef struct{St s;int cx,cy;} Ch;
static int chcmp(const void*a,const void*b){return cmpst(&((const Ch*)a)->s,&((const Ch*)b)->s);}
static int children(const St*s,Ch*out,int d,int*winclick){ // distinct ok children (exact dedupe), all 4096 clicks classified
  int nl=0; *winclick=-1; for(int cy=0;cy<64;cy++)for(int cx=0;cx<64;cx++){ St o;int ls; int r=click(s,cx,cy,&o,&ls); C[d].clicks++;
    if(r==0){C[d].noop++;continue;} if(r==1){C[d].fail++;continue;} if(r==3){ if(*winclick<0)*winclick=cx*64+cy; C[d].wins++; continue; }
    if(ls){C[d].lossy++; C[d].pr_mass++; continue;} out[nl].s=o;out[nl].cx=cx;out[nl].cy=cy;nl++; }
  qsort(out,nl,sizeof(Ch),chcmp); int u=0; for(int k=0;k<nl;k++){ if(k&&cmpst(&out[k].s,&out[k-1].s)==0)continue; if(cmpst(&out[k].s,s)==0)continue; out[u++]=out[k]; } return u; }
static int dfs(const St*s,int d,int shard,int mod){ // returns 1 if a win within the bound was found below s
  C[d].expanded++; static Ch buf[16][4200]; Ch*ch=buf[d]; int wc; int n=children(s,ch,d,&wc);
  if(wc>=0){ route[d][0]=wc/64;route[d][1]=wc%64; found=1;foundlen=d+1; memcpy(foundr,route,sizeof route); return 1; }
  C[d].children+=n;
  if(MODE_FIND){ for(int i=n-1;i>0;i--){int j=rnd()%(i+1);Ch t=ch[i];ch[i]=ch[j];ch[j]=t;} }
  int rem=B-(d+1);
  for(int i=0;i<n;i++){ if(d==0&&mod>1&&(i%mod)!=shard)continue;
    if(rem<1){ C[d].pr_h++; continue; }  // a non-won child needs >=1 more click
    uint64_t hh=hst(&ch[i].s); if(tt_refuted(hh,rem)){C[d].pr_tt++;continue;}
    dpcap=rem+SLACK; int rule; int h=hbound(&ch[i].s,FLAGS,&rule); C[d].hcalls++;
    if(h>=INF){C[d].pr_mass++;continue;} if(h-SLACK>rem){C[d].pr_h++; tt_put(hh,rem); continue;}
    route[d][0]=ch[i].cx;route[d][1]=ch[i].cy;
    if(dfs(&ch[i].s,d+1,shard,mod)) return 1;
    tt_put(hh,rem); }
  return 0; }
int main(int argc,char**argv){ B=atoi(argv[1]); FLAGS=atoi(argv[2]); MODE_FIND=!strcmp(argv[3],"find"); rng^=(uint64_t)atoll(argv[4])*0x9E3779B97F4A7C15ULL; int shard=atoi(argv[5]),mod=atoi(argv[6]); int noroot=argc>7&&!strcmp(argv[7],"noroot"); if(argc>8)SLACK=atoi(argv[8]);
  l3_setup(); St s0=l3_start(); dpcap=B+SLACK; int rule; int h0=hbound(&s0,FLAGS,&rule); printf("ROOT h=%d (rule %d) bound B=%d flags=%d mode=%s %s SLACK=%d (prune only if h-SLACK>remaining: shorter-route attack, NOT a proof run)\n",h0,rule,B,FLAGS,argv[3],noroot?"root-prune-disabled":"",SLACK); fflush(stdout);
  int res=0; if(h0>B&&!noroot){ printf("ROOT PRUNED by R_H: depth0+h=%d > B=%d : no state expanded\n",h0,B); }
  else res=dfs(&s0,0,shard,mod);
  for(int d=0;d<12;d++) if(C[d].expanded) printf("DEPTH %d expanded=%ld clicks=%ld noop=%ld fail=%ld lossy=%ld distinct_children=%ld hcalls=%ld pruned_R_MASS=%ld pruned_R_H=%ld pruned_R_TT=%ld win_clicks=%ld\n",d,C[d].expanded,C[d].clicks,C[d].noop,C[d].fail,C[d].lossy,C[d].children,C[d].hcalls,C[d].pr_mass,C[d].pr_h,C[d].pr_tt,C[d].wins);
  if(found){ printf("WIN_FOUND len=%d route:",foundlen); for(int i=0;i<foundlen;i++)printf(" %d,%d",foundr[i][0],foundr[i][1]); printf("\n"); }
  else printf("NO_WIN within B=%d (shard %d/%d) exhausted=%d\n",B,shard,mod,!MODE_FIND);
  return 0; }
