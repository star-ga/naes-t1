// Adversarial check LEMMA lens: EXACT distance-to-win on the COMPLETE 2-piece state space {v3 at a, v2 at b} of su15 L3,
// computed with the full simulator rclick() (every click that can hit a piece), by value iteration.
// From {v3,v2} every successful click either wins or moves exactly one piece, so this space is closed.
// Output: histogram of exact distances, and a stratified sample of states with their exact distance (for h comparison).
#include "rsim.h"
#include <omp.h>
static unsigned char *Dd;
static inline int ok3(int x,int y){ return x>=0&&x<=60&&y>=10&&y<=60; }
static inline int ok2(int x,int y){ return x>=0&&x<=61&&y>=10&&y<=61; }
int main(int argc,char**argv){ int KMAX=argc>1?atoi(argv[1]):14; Dd=malloc(4096*4096); memset(Dd,255,4096*4096);
  long nst=0,n0=0;
  for(int a=0;a<4096;a++) for(int b=0;b<4096;b++){ int ax=a&63,ay=a>>6,bx=b&63,by=b>>6; if(!ok3(ax,ay)||!ok2(bx,by)){ Dd[(long)a*4096+b]=254; continue; } nst++;
    S s; s.n=2; s.v[0]=3;s.x[0]=ax;s.y[0]=ay; s.v[1]=2;s.x[1]=bx;s.y[1]=by; if(won(&s)){ Dd[(long)a*4096+b]=0; n0++; } }
  printf("STATES %ld already_won(dist0) %ld\n",nst,n0); fflush(stdout);
  for(int k=1;k<=KMAX;k++){ long nk=0;
#pragma omp parallel for schedule(dynamic,16) reduction(+:nk)
    for(int a=0;a<4096;a++){ int ax=a&63,ay=a>>6; if(!ok3(ax,ay)) continue;
      for(int b=0;b<4096;b++){ long idx=(long)a*4096+b; if(Dd[idx]!=255) continue; int bx=b&63,by=b>>6; if(!ok2(bx,by)) continue;
        S s; s.n=2; s.v[0]=3;s.x[0]=ax;s.y[0]=ay; s.v[1]=2;s.x[1]=bx;s.y[1]=by; int found=0;
        for(int pass=0;pass<2&&!found;pass++){ int cx0= pass==0? ax-8:bx-8, cy0= pass==0? ay-8: by-8, w= pass==0? 20:19;
          for(int py=cy0;py<cy0+w&&!found;py++){ if(py<10||py>62) continue; for(int px=cx0;px<cx0+w;px++){ if(px<0||px>63) continue;
            if(pass==1 && hit(px,py,3,ax,ay)) continue; // already tried in pass 0
            S o; int wt; int r=rclick(&s,px,py,&o,&wt);
            if(r==R_WIN){ if(k==1){found=1;break;} else continue; }
            if(r!=R_OK || k==1) continue;
            int i3=o.v[0]==3?0:1; long j=(long)(o.y[i3]*64+o.x[i3])*4096 + (o.y[1-i3]*64+o.x[1-i3]);
            if(Dd[j]==k-1){found=1;break;} } } }
        if(found){ Dd[idx]=100+k; nk++; } } }
    // commit
#pragma omp parallel for
    for(long i=0;i<4096L*4096;i++) if(Dd[i]>100&&Dd[i]<254) Dd[i]-=100;
    printf("DIST %d states %ld\n",k,nk); fflush(stdout); if(!nk && k>2) break; }
  long unk=0; for(long i=0;i<4096L*4096;i++) if(Dd[i]==255) unk++; printf("UNREACHED_WITHIN_KMAX %ld\n",unk);
  FILE*f=fopen("r2exact.bin","wb"); fwrite(Dd,1,4096L*4096,f); fclose(f);
  // stratified sample: up to 300 per distance 1..KMAX (+ 200 unreached), deterministic stride
  unsigned long long rs=99; int per[256]={0};
  FILE*g=fopen("r2sample.txt","w");
  for(int rep=0;rep<40000000;rep++){ rs^=rs<<13; rs^=rs>>7; rs^=rs<<17; long i=rs%(4096L*4096); int d=Dd[i]; if(d==254||d==0) continue; int cap= d==255?200:300; if(per[d]>=cap) continue; per[d]++;
    int a=i/4096,b=i%4096; fprintf(g,"%d 2 3 %d %d 2 %d %d\n",d,a&63,a>>6,b&63,b>>6); }
  fclose(g); printf("SAMPLE"); for(int d=0;d<256;d++) if(per[d]) printf(" d%d:%d",d,per[d]); printf("\n"); return 0; }
