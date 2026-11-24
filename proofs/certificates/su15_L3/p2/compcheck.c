// Over every engine-checked vector record: (1) click(full) == rest U click(hit subset); (2) sum of 2^v never increases; (3) hit-set statistics
#include "simp2.h"
int main(int argc,char**argv){ sim_init(); long tot=0,bad=0,sumbad=0,h1=0,h2=0,h3p=0,lossy_n=0,fail_n=0,okn=0; char buf[65536];
  for(int a=1;a<argc;a++){ FILE*f=fopen(argv[a],"r"); if(!f){perror(argv[a]);return 2;}
    while(fgets(buf,sizeof buf,f)){ char*p=buf; int n,off; if(sscanf(p,"%d%n",&n,&off)!=1)continue; p+=off; St s; s.n=n; for(int i=0;i<n;i++){int v,x,y;sscanf(p,"%d %d %d%n",&v,&x,&y,&off);p+=off;s.v[i]=v;s.x[i]=x;s.y[i]=y;}
      int cx,cy; sscanf(p,"%d %d",&cx,&cy); canon(&s); tot++;
      St o;int lossy; int r=click(&s,cx,cy,&o,&lossy);
      int idx[9],m=0; if(!(cy<=YMIN-1||cy>=YMAX)) for(int i=0;i<s.n;i++) if(inrange(cx,cy,s.v[i],s.x[i],s.y[i])) idx[m++]=i;
      if(m==1)h1++; else if(m==2)h2++; else if(m>=3)h3p++;
      if(r==2){ int ps=0,qs=0; for(int i=0;i<s.n;i++)ps+=1<<s.v[i]; for(int i=0;i<o.n;i++)qs+=1<<o.v[i]; if(qs>ps)sumbad++; if(lossy)lossy_n++; if(m>=2)okn++;
        if(m>=1){ St sub; sub.n=m; for(int k=0;k<m;k++){sub.v[k]=s.v[idx[k]];sub.x[k]=s.x[idx[k]];sub.y[k]=s.y[idx[k]];} canon(&sub); St so; int sl; int rs=click(&sub,cx,cy,&so,&sl);
          St comp; comp.n=0; char hit[9]={0}; for(int k=0;k<m;k++)hit[idx[k]]=1; for(int i=0;i<s.n;i++) if(!hit[i]){comp.v[comp.n]=s.v[i];comp.x[comp.n]=s.x[i];comp.y[comp.n]=s.y[i];comp.n++;}
          for(int i=0;i<so.n;i++){comp.v[comp.n]=so.v[i];comp.x[comp.n]=so.x[i];comp.y[comp.n]=so.y[i];comp.n++;} canon(&comp);
          if(rs!=2||sl!=lossy||cmpst(&comp,&o)!=0) bad++; } }
      else if(r==1){ fail_n++; St sub; sub.n=m; for(int k=0;k<m;k++){sub.v[k]=s.v[idx[k]];sub.x[k]=s.x[idx[k]];sub.y[k]=s.y[idx[k]];} canon(&sub); St so;int sl; int rs=click(&sub,cx,cy,&so,&sl); if(rs!=1 && rs!=3) bad++; }
    } fclose(f); }
  printf("COMPCHECK records=%ld composition_mismatches=%ld sum_increase=%ld hitset_size1=%ld size2=%ld size3plus=%ld ok_with_pairs_or_more=%ld lossy_ok=%ld fails=%ld\n",tot,bad,sumbad,h1,h2,h3p,okn,lossy_n,fail_n); return bad||sumbad; }
