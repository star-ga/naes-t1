// P2/P3: the exact win semantics used by the final-click rules F0/F1/F2 of p1dp.h, checked against simc.h click() exhaustively.
//  FP : state {v3 at a, v2 at b}, every click P (cx 0..63, cy 0..63), every a in range, every b in range (both pieces hit):
//       click()==win  <=>  exists tick t in 1..3 with both pieces (stepped t times toward P with clamp) centred in a zone.   (rule F1)
//  FS : state {mover, static}, exactly one piece in range: for the mover = v3 (static v2) and mover = v2 (static v3): every P, every mover position in range,
//       static positions = every in-zone position plus 150 random out-of-range positions:
//       click()==win  <=>  static piece centred in a zone AND mover centred in a zone at some tick 1..4 (tick 4 = landing).    (rules F2 and F0)
#include "p1dp.h"
#include <omp.h>
static void ticks(int v,int x,int y,int cx,int cy,int*z){ for(int t=0;t<3;t++){ stepf(v,&x,&y,cx,cy); z[t]=inzone(v,x,y);} }
int main(int argc,char**argv){ l3_setup(); const char*mode=argv[1]; long chk=0,bad=0,wins=0;
  if(!strcmp(mode,"FP")){
    #pragma omp parallel for schedule(dynamic,1) reduction(+:chk,bad,wins)
    for(int pc=0;pc<64*64;pc++){ int cx=pc/64,cy=pc%64; if(cy<YMIN||cy>=YMAX)continue;
      for(int ka=0;ka<offc[3];ka++){ int ax=cx+offx[3][ka],ay=cy+offy[3][ka]; if(!valid(3,ax,ay))continue; int za[3]; ticks(3,ax,ay,cx,cy,za);
        for(int kb=0;kb<offc[2];kb++){ int bx=cx+offx[2][kb],by=cy+offy[2][kb]; if(!valid(2,bx,by))continue; int zb[3]; ticks(2,bx,by,cx,cy,zb);
          int pred=(za[0]&&zb[0])||(za[1]&&zb[1])||(za[2]&&zb[2]);
          St s; s.n=2; s.v[0]=2;s.x[0]=bx;s.y[0]=by; s.v[1]=3;s.x[1]=ax;s.y[1]=ay; canon(&s); St o;int ls; int r=click(&s,cx,cy,&o,&ls); chk++; int win=(r==3); wins+=win;
          if(win!=pred||(!win&&r!=1)){ bad++; } } } }
    printf("FP checks=%ld wins=%ld violations=%ld\n",chk,wins,bad); return bad!=0; }
  if(!strcmp(mode,"FS")){ int mv=atoi(argv[2]); int sv=mv==3?2:3; srand(7);
    #pragma omp parallel for schedule(dynamic,1) reduction(+:chk,bad,wins)
    for(int pc=0;pc<64*64;pc++){ int cx=pc/64,cy=pc%64; if(cy<YMIN||cy>=YMAX)continue; unsigned sd=pc*7919u+1;
      static __thread int dummy; (void)dummy;
      for(int km=0;km<offc[mv];km++){ int mx=cx+offx[mv][km],my=cy+offy[mv][km]; if(!valid(mv,mx,my))continue; int zt[3]; ticks(mv,mx,my,cx,cy,zt);
        int lx,ly; landc(mv,cx,cy,&lx,&ly); int zend=inzone(mv,lx,ly);
        for(int sx=0;sx<=WD-SZ[sv];sx++)for(int sy=YMIN;sy<=YMAX&&sy<=WD-SZ[sv];sy++){ int iz=inzone(sv,sx,sy); if(!iz){ sd=sd*1103515245u+12345u; if((sd>>16)%40!=0)continue; }
          if(inrange(cx,cy,sv,sx,sy))continue; // exactly one piece in range
          St s; s.n=2; s.v[0]=mv;s.x[0]=mx;s.y[0]=my; s.v[1]=sv;s.x[1]=sx;s.y[1]=sy; canon(&s); St o;int ls; int r=click(&s,cx,cy,&o,&ls); chk++;
          int pred=iz&&(zt[0]||zt[1]||zt[2]||zend); int win=(r==3); wins+=win; if(win!=pred)bad++; } } }
    printf("FS mover=%d static=%d checks=%ld wins=%ld violations=%ld\n",mv,sv,chk,wins,bad); return bad!=0; }
  return 2; }
