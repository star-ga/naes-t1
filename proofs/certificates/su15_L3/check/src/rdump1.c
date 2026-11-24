// print all distinct successful-click successors of the start (depth 1), and states along given routes (argv)
#include "rsim.h"
static int cmpi(const void*a,const void*b){ return *(const int*)a-*(const int*)b; }
static void canon(const S*s,char*buf){ int a[16]; for(int i=0;i<s->n;i++) a[i]=(s->v[i]<<16)|(s->x[i]<<8)|s->y[i]; qsort(a,s->n,sizeof(int),cmpi);
  int k=sprintf(buf,"%d",s->n); for(int i=0;i<s->n;i++) k+=sprintf(buf+k," %d %d %d",a[i]>>16,(a[i]>>8)&255,a[i]&255); }
int main(int argc,char**argv){ S s0; rstart(&s0);
  if(argc==1){ static char seen[4096][256]; int ns=0; for(int P=0;P<4096;P++){ S o; if(rclick(&s0,P&63,P>>6,&o,NULL)!=R_OK) continue; char b[256]; canon(&o,b); int d=0; for(int j=0;j<ns;j++) if(!strcmp(seen[j],b)){d=1;break;} if(!d){ strcpy(seen[ns++],b); printf("%s\n",b);} } fprintf(stderr,"distinct=%d\n",ns); return 0; }
  // route mode: argv[1] = "x,y x,y ..."
  S s=s0; char b[256]; canon(&s,b); printf("%s\n",b); char*r=strdup(argv[1]); for(char*t=strtok(r," ");t;t=strtok(NULL," ")){ int x,y; sscanf(t,"%d,%d",&x,&y); S o; int wt; int c=rclick(&s,x,y,&o,&wt); if(c==R_WIN){ fprintf(stderr,"WIN at %s tick %d\n",t,wt); break;} if(c!=R_OK){ fprintf(stderr,"click %s code %d\n",t,c);} s=o; canon(&s,b); printf("%s\n",b); } return 0; }
