#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct{
  int L;
  double ro;
  int numSteps;
}dati;

typedef struct{
  int ix, iy, distx, disty;
}particle;

void saw(dati pp);
void inizializzazione(dati pp);
void compatto(dati pp);
void gasReticolare(int M, int N, dati pp, int site[], particle p[]);

int passo(void);

void printConf(particle *p, int M){
  FILE *conf=fopen("conf.dat","w");
  int i;

  for(i=0;i<M;i++){
    fprintf(conf, "%i %i\n", p[i].ix, p[i].iy);
  }
  fclose(conf);
}  

void errore(char *string){
  fprintf(stderr, "%s", string);
  fprintf(stderr, "  \n");
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){ 
  dati pp;
  int opt;
  unsigned int seed;
  FILE* devran=fopen("/dev/random","r");

  fread(&seed, 4, 1, devran);
  fclose(devran);

  srand48(seed);
  
  if(argc!=5){ 
    errore("ERROR, usage: taglia(L), densità(p), numSteps, opt \n"); 
  } 

  pp.L=atoi(argv[1]); 
  pp.ro=atof(argv[2]);
  pp.numSteps=atoi(argv[3]);
  opt=atoi(argv[4]);

  if(opt==1){
    saw(pp);   
  }
  if(opt==2){
    inizializzazione(pp);
  }
  if(opt==3){
    compatto(pp);
  }
}
    
inline int passo(void){
  int q;
  q=-1+2*((int)(drand48()*2));
  return q;
}

void saw(dati pp){
  int xory, index;
  int err, check;
  int t=0, i, j;
  int dir;
  int* x;
  int* y;
  int direzione[4];
  
  x=(int*)calloc(pp.numSteps, sizeof(int));
  y=(int*)calloc(pp.numSteps, sizeof(int));
  
  x[0]=0;
  y[0]=0;
  printf("%i %i %i \n", t, x[0], y[0]);
  
  while(t<pp.numSteps){
    for(j=0;j<4;j++){
      direzione[j]=j;
    }  
    t++;
    dir=4;
    i=0;
    err=0;
    while(i<4){
      check=0;
      index=(int)(drand48()*dir);
      xory=direzione[index];
      direzione[index]=direzione[--dir];

      if(xory==0){             
	x[t]=x[t-1]+1;       
	y[t]=y[t-1];
      }
      if(xory==1){             
	x[t]=x[t-1]-1;       
	y[t]=y[t-1];
      }
      if(xory==2){             
	x[t]=x[t-1];       
	y[t]=y[t-1]+1;
      }
      if(xory==3){             
	x[t]=x[t-1];       
	y[t]=y[t-1]-1;
      }

      for(j=0;j<t;j++){
	if(x[t]==x[j] && y[t]==y[j]){
	  check=1;	  
	}
      }
      if(check!=0){
	err++;
	x[t]=x[t-1];
	y[t]=y[t-1];
      }
      if(check==0){
	printf("%i %i %i\n", t, x[t], y[t]);
	i=5;
      }            
      if(err==4){
	fprintf(stderr, "la particella non si può più muovere \n");
	exit(EXIT_FAILURE);
      }
      i++;
    }
  }  
}

  
void inizializzazione(dati pp){
  int index, i, num, site;
  int N, M;
  int* list;
  int* occ;
  particle *p; 

  N=(pp.L)*(pp.L);
  M=(int)(N*(pp.ro));

  list=(int*)calloc(N, sizeof(int));
  occ=(int*)calloc(N, sizeof(int));
  p=(particle*)calloc(M, sizeof(particle));  
  
  for(i=0;i<N;i++){
    list[i]=i;
    occ[i]=-1;
  }
  
  num=N;
  for(i=0;i<M;i++){
    index=(int)(drand48()*num);
    site=list[index];
    list[index]=list[--num];
    occ[site]=i;
    p[i].ix=site%(pp.L);
    p[i].iy=(int)(site/(pp.L));
    p[i].distx=0;
    p[i].disty=0;
  }
  gasReticolare(M, N, pp, occ, p);
}

void compatto(dati pp){
  int invL, i;
  int N, M;
  int* occ;
  particle *p; 

  N=(pp.L)*(pp.L);
  M=(int)(N*(pp.ro));

  occ=(int*)calloc(N, sizeof(int));
  p=(particle*)calloc(M, sizeof(particle));  
  
  for(i=0;i<N;i++){
    occ[i]=-1;
  }
  
  invL=1.0/(pp.L);
  for(i=0;i<M;i++){
    occ[i]=i;
    p[i].ix=i%(pp.L);
    p[i].iy=(int)((pp.L-M*invL)/2.0+i*invL);
    p[i].distx=0;
    p[i].disty=0;
  }
  gasReticolare(M, N, pp, occ, p);
}

void gasReticolare(int M, int N, dati pp, int occ[], particle p[]){
  int i, t;
  int q;
  int measTime;
  int ixnew, iynew, xory;
  int dx=0, dy=0;
  int R2;
  long double D;
  FILE *gpl=popen("gnuplot -persist","r");

  fprintf(gpl, "set size square\n");
  fprintf(gpl, "unset key\n");
  fprintf(gpl, "unset zeroaxis\n");
  fprintf(gpl, "set xra [-0.5:%g]\n", pp.L-0.5);
  fprintf(gpl, "set yra [-0.5:%g]\n", pp.L-0.5);

  printConf(p, M);
  fprintf(gpl, "p 'conf.dat' w d\n");
  fflush(gpl);

  measTime=1;
  for(t=1;t<=pp.numSteps;t++){
    for(i=0;i<M;i++){
      xory=passo();
      q=passo();
      if(xory==1){             
	ixnew=(p[i].ix+pp.L+q)%(pp.L);       
	iynew=p[i].iy;
	if(occ[ixnew+(pp.L)*iynew]==-1){
	  occ[p[i].ix+(pp.L)*p[i].iy]=i;
	  occ[ixnew+(pp.L)*iynew]=occ[p[i].ix+(pp.L)*p[i].iy];
	  occ[p[i].ix+(pp.L)*p[i].iy]=-1;
	  p[i].distx+=q;
	  p[i].ix=ixnew;
	  p[i].iy=iynew;
	}
      }
      else if(xory==-1){
        ixnew=p[i].ix;
	iynew=(p[i].iy+pp.L+q)%(pp.L);
	if(occ[ixnew+(pp.L)*iynew]==-1){
	  occ[p[i].ix+pp.L*p[i].iy]=i;
	  occ[ixnew+pp.L*iynew]=occ[p[i].ix+pp.L*p[i].iy];
	  occ[p[i].ix+pp.L*p[i].iy]=-1;
	  p[i].disty+=q;
	  p[i].ix=ixnew;
	  p[i].iy=iynew;
	}
      }
      dx+=(p[i].distx)*(p[i].distx);
      dy+=(p[i].disty)*(p[i].disty);
      R2=dx+dy;
      D=((double)R2)/t;
      printf("%i %i %Lg \n", t, R2, D);
      //printf("%i %i %i  ", t, p[i].iy, p[i].ix);
      if(t==measTime){
	printConf(p, M);
	fprintf(gpl, "p 'conf.dat' w d\n");
	fflush(gpl);
	measTime+=100;
      }
    }       
  }
}
