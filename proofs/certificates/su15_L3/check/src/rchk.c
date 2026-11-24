// Adversarial check: compare engine outcomes (reng.py output) with rsim.h. Prints mismatches and a summary.
#include "rsim.h"
#include <ctype.h>
static int cmpi(const void*a,const void*b){ return *(const int*)a-*(const int*)b; }
int main(int argc,char**argv){ long rec=0,mm=0,wins=0,noop=0,fail=0,ok=0,ewin=0; int wt_hist[5]={0};
 for(int f=1;f<argc;f++){ FILE*fp=fopen(argv[f],"r"); char line[4096];
  while(fgets(line,sizeof line,fp)){ char*semi=strchr(line,';'); char*arrow=strstr(line,"=>"); if(!semi||!arrow) continue;
   S s; char*p=line; s.n=strtol(p,&p,10); for(int i=0;i<s.n;i++){ s.v[i]=strtol(p,&p,10); s.x[i]=strtol(p,&p,10); s.y[i]=strtol(p,&p,10); }
   p=semi+1; int px=strtol(p,&p,10), py=strtol(p,&p,10); p=arrow+2; while(*p==' ') p++;
   S o; int wt=0; int r=rclick(&s,px,py,&o,&wt); rec++;
   char exp[2048]; if(r==R_WIN){ strcpy(exp,"WIN"); wins++; wt_hist[wt]++; }
   else { if(r==R_NOOP) noop++; else if(r==R_FAIL) fail++; else ok++;
     int a[16]; for(int i=0;i<o.n;i++) a[i]=(o.v[i]<<16)|(o.x[i]<<8)|o.y[i]; qsort(a,o.n,sizeof(int),cmpi);
     int k=sprintf(exp,"%d",o.n); for(int i=0;i<o.n;i++) k+=sprintf(exp+k," %d %d %d",a[i]>>16,(a[i]>>8)&255,a[i]&255); }
   char got[2048]; strcpy(got,p); int L=strlen(got); while(L&&isspace((unsigned char)got[L-1])) got[--L]=0;
   if(!strcmp(got,"WIN")) ewin++;
   if(strcmp(got,exp)){ mm++; if(mm<=20) printf("MISMATCH %s | sim: %s\n",line,exp); } }
  fclose(fp); }
 printf("RCHK records=%ld mismatches=%ld sim_noop=%ld sim_fail=%ld sim_ok=%ld sim_win=%ld engine_win=%ld win_tick_hist t1=%d t2=%d t3=%d t4=%d\n",rec,mm,noop,fail,ok,wins,ewin,wt_hist[1],wt_hist[2],wt_hist[3],wt_hist[4]);
 return mm?1:0; }
