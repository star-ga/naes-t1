// for each input state line, print all distinct successful children (excluding the state itself), canonical form
#include "rsim.h"
static int cmpi(const void*a,const void*b){ return *(const int*)a-*(const int*)b; }
static void canon(const S*s,char*buf){ int a[16]; for(int i=0;i<s->n;i++) a[i]=(s->v[i]<<16)|(s->x[i]<<8)|s->y[i]; qsort(a,s->n,sizeof(int),cmpi);
  int k=sprintf(buf,"%d",s->n); for(int i=0;i<s->n;i++) k+=sprintf(buf+k," %d %d %d",a[i]>>16,(a[i]>>8)&255,a[i]&255); }
static int cmps(const void*a,const void*b){ return strcmp(*(char*const*)a,*(char*const*)b); }
int main(){ char line[4096]; static char store[4096][160]; static char*ptr[4096]; long tot=0,wins=0;
  while(fgets(line,sizeof line,stdin)){ char*p=line; S s; s.n=strtol(p,&p,10); if(s.n<=0) continue; for(int i=0;i<s.n;i++){ s.v[i]=strtol(p,&p,10); s.x[i]=strtol(p,&p,10); s.y[i]=strtol(p,&p,10); }
    char self[160]; canon(&s,self); int n=0;
    for(int P=0;P<4096;P++){ S o; int wt; int r=rclick(&s,P&63,P>>6,&o,&wt); if(r==R_WIN){ wins++; printf("WINCHILD %s ; %d %d\n",self,P&63,P>>6); continue; } if(r!=R_OK) continue; int m=0; for(int i=0;i<o.n;i++) m+=1<<o.v[i]; if(m!=12) continue; canon(&o,store[n]); if(!strcmp(store[n],self)) continue; ptr[n]=store[n]; n++; }
    qsort(ptr,n,sizeof(char*),cmps); for(int i=0;i<n;i++){ if(i&&!strcmp(ptr[i],ptr[i-1])) continue; printf("%s\n",ptr[i]); tot++; } }
  fprintf(stderr,"KIDS total=%ld winchildren=%ld\n",tot,wins); return 0; }
