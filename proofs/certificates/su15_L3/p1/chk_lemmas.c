// exhaustive lemma checks against the exact simulator simc.h
//  LA  : single piece, every v in 0..3, every valid top-left q, every click (cx 0..63, cy 0..63):
//        in range & cy in 10..62 -> status ok, not lossy, result = one piece (v,landc(v,P)); else noop with unchanged state.
//  LC  : for every click P and every value triple (v1,v2,v3): the three landed rects pairwise overlap (so a >=3-piece click is fail or lossy)
//  LB  : every pair (v1,q1),(v2,q2) both in range of P (shard by v1,v2): v1==v2 -> ok merge at the mean landing, never lossy;
//        v1!=v2 -> fail, or mid-flight win (only possible for {3,2}).
#include "simc.h"
static inline void landc(int v,int cx,int cy,int*lx,int*ly){ int x=cx-SZ[v]/2,y=cy-SZ[v]/2; clampp(v,&x,&y); *lx=x;*ly=y; }
static inline int validq(int v,int x,int y){return x>=0&&x<=WD-SZ[v]&&y>=YMIN&&y<=YMAX&&y<=WD-SZ[v];}
static void setup(void){ NZ=2; ZONES[0]=(Zone){5,46,9,9}; ZONES[1]=(Zone){19,46,9,9}; NG=2; GV[0]=3;GC[0]=1;GV[1]=2;GC[1]=1; }
int main(int argc,char**argv){ setup(); const char*mode=argv[1]; long bad=0,cnt=0;
  if(!strcmp(mode,"LA")){ long inr=0,noop=0;
    for(int v=0;v<4;v++)for(int x=0;x<=WD-SZ[v];x++)for(int y=YMIN;y<=YMAX&&y<=WD-SZ[v];y++){
      St s; s.n=1;s.v[0]=v;s.x[0]=x;s.y[0]=y;
      for(int cx=0;cx<64;cx++)for(int cy=0;cy<64;cy++){ St o;int ls; int r=click(&s,cx,cy,&o,&ls); cnt++;
        int ir=(cy>=YMIN&&cy<YMAX)&&inrange(cx,cy,v,x,y);
        if(!ir){ noop++; if(r!=0)bad++; continue; }
        inr++; int lx,ly; landc(v,cx,cy,&lx,&ly);
        if(r!=2||ls||o.n!=1||o.v[0]!=v||o.x[0]!=lx||o.y[0]!=ly){ bad++; if(bad<5)printf("LA FAIL v=%d q=(%d,%d) P=(%d,%d) r=%d\n",v,x,y,cx,cy,r);} } }
    printf("LA clicks=%ld in_range=%ld noop=%ld violations=%ld\n",cnt,inr,noop,bad); return bad!=0; }
  if(!strcmp(mode,"LC")){ for(int cy=YMIN;cy<YMAX;cy++)for(int cx=0;cx<64;cx++)for(int a=0;a<4;a++)for(int b=0;b<4;b++)for(int c=0;c<4;c++){ int lx[3],ly[3],v3[3]={a,b,c}; for(int i=0;i<3;i++)landc(v3[i],cx,cy,&lx[i],&ly[i]);
      for(int i=0;i<3;i++)for(int j=i+1;j<3;j++){ cnt++; if(!ovl(v3[i],lx[i],ly[i],v3[j],lx[j],ly[j])){bad++; if(bad<5)printf("LC FAIL P=(%d,%d) %d %d\n",cx,cy,v3[i],v3[j]);} } }
    printf("LC pair-overlap checks=%ld violations=%ld\n",cnt,bad); return bad!=0; }
  if(!strcmp(mode,"LB")){ int v1=atoi(argv[2]),v2=atoi(argv[3]); long nok=0,nfail=0,nwin=0;
    for(int cy=YMIN;cy<YMAX;cy++)for(int cx=0;cx<64;cx++){
      static int q1[1024][2],q2[1024][2]; int n1=0,n2=0;
      for(int x=0;x<=WD-SZ[v1];x++)for(int y=YMIN;y<=YMAX&&y<=WD-SZ[v1];y++) if(inrange(cx,cy,v1,x,y)){q1[n1][0]=x;q1[n1][1]=y;n1++;}
      for(int x=0;x<=WD-SZ[v2];x++)for(int y=YMIN;y<=YMAX&&y<=WD-SZ[v2];y++) if(inrange(cx,cy,v2,x,y)){q2[n2][0]=x;q2[n2][1]=y;n2++;}
      int lx1,ly1,lx2,ly2; landc(v1,cx,cy,&lx1,&ly1); landc(v2,cx,cy,&lx2,&ly2);
      for(int i=0;i<n1;i++)for(int j=0;j<n2;j++){ if(v1==v2&&j<i)continue; St s; s.n=2; s.v[0]=v1;s.x[0]=q1[i][0];s.y[0]=q1[i][1]; s.v[1]=v2;s.x[1]=q2[j][0];s.y[1]=q2[j][1]; canon(&s);
        St o;int ls; int r=click(&s,cx,cy,&o,&ls); cnt++;
        if(v1==v2){ if(i==j&&0){} // identical top-left of same value = two pieces on one cell: still a legal 2-piece state
          int nv=v1+1; int ax=lx1+SZ[v1]/2-SZ[nv]/2, ay=ly1+SZ[v1]/2-SZ[nv]/2; clampp(nv,&ax,&ay);
          if(r==2&&!ls&&o.n==1&&o.v[0]==nv&&o.x[0]==ax&&o.y[0]==ay)nok++; else {bad++; if(bad<5)printf("LB same FAIL v=%d P=(%d,%d) r=%d\n",v1,cx,cy,r);} }
        else { if(r==1)nfail++; else if(r==3&&((v1==3&&v2==2)||(v1==2&&v2==3)))nwin++; else {bad++; if(bad<5)printf("LB diff FAIL v1=%d v2=%d P=(%d,%d) r=%d\n",v1,v2,cx,cy,r);} } } }
    printf("LB v1=%d v2=%d pairs=%ld merge_ok=%ld fail=%ld midflight_win=%ld violations=%ld\n",v1,v2,cnt,nok,nfail,nwin,bad); return bad!=0; }
  return 2; }
