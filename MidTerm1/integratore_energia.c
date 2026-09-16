#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct{
    double x,v,dt;
    int n, tmax, algo, ndt;
} param;

//ENERGIA
double en(double x, double v){
  return 0.5*v*v+2*log(x)*log(x)*exp(-x/2);
}

//ACCELERAZIONE
double ac(double x, double v, double t){
  return exp(-x/2)*log(x)*(log(x)-(4./x));
}

void eulero(param s){
  double x, v, temp;
  int i;
  x=s.x;
  v=s.v;
  for(i=0;i<s.n;i++){
    printf("%lg %.14lf %.14lf\n",i*s.dt ,x ,v);
    temp=x;
    x=x+v*s.dt;
    v=v+ac(temp,v,0)*s.dt;
  }
  printf("%lg %.14lf %.14lf\n", i*s.dt, x, v);
}


void cromer(param s){
  double x, v;
  int i;
  x=s.x;
  v=s.v;
  for(i=0;i<s.n;i++){
    printf("%lg %.14lf %.14lf\n",i*s.dt ,x ,v);
    v=v+ac(x,v,0)*s.dt;
    x=x+v*s.dt;
  }
  printf("%lg %.14lf %.14lf\n", i*s.dt, x, v);
}


void leapfrog(param s){
  double x, v, e0;
  int i;
  x=s.x;
  v=s.v+ac(s.x,s.v,0)*(s.dt/2);
  e0=en(s.x,s.v);
  for(i=0;i<s.n;i++){
    //printf("%lg %.14lf %.14lf %.14lf\n",i*s.dt ,x ,v-ac(x,v,0)*(s.dt/2),(e(x,v-ac(x,v,0)*(s.dt/2))-e0)/e0);
    x=x+v*s.dt;
    v=v+ac(x,v,0)*s.dt;
  }
  printf("%lg %.14lf %.14lf %.14lf %lg\n", i*s.dt, x, v-ac(x,v,0)*(s.dt/2),(en(x,v-ac(x,v,0)*(s.dt/2))-e0)/e0,s.dt);
}

void verletvelocita(param s){
  double x, v, a, temp;
  int i;
  x=s.x;
  v=s.v;
  a=ac(s.x,s.v,0);
  for(i=0;i<s.n;i++){
    printf("%lg %.14lf %.14lf\n",i*s.dt ,x ,v);
    x=x+v*s.dt+(0.5)*a*(s.dt*s.dt);
    temp=a;
    a=ac(x,v,0);
    v=v+((temp+a)/2)*s.dt;
  }
  printf("%lg %.14lf %.14lf\n", i*s.dt, x, v);
}

void verletposizioni(param s){
  double x, xnew, xold, v, a;
  int i;
  x=s.x;
  v=s.v;
  a=ac(s.x,s.v,0);
  xnew=x+v*s.dt+(0.5)*a*(s.dt*s.dt);
  for(i=0;i<s.n;i++){
    printf("%lg %.14lf %.14lf\n",i*s.dt ,x ,v);
    xold=x;
    x=xnew;
    a=ac(x,v,0); 
    xnew=2*x-xold+a*(s.dt*s.dt);
    v=(xnew-xold)/(2*s.dt);
  }
  printf("%lg %.14lf %.14lf\n", i*s.dt, x, v);
}


void rk2(param s){
  double x,xold, v;
  int i;
  x=s.x;
  v=s.v;
  for(i=0;i<s.n;i++){
    printf("%lg %.14lf %.14lf\n",i*s.dt ,x ,v);
    xold=x;
    //x=x+v*dt+0.5*a(x,0,0)*dt*dt;
    //v=v+0.5*a(xold,0,0)*dt+0.5*a(xold+v*dt,0,0)*dt;
    x=x+(v+ac(x,v,0)*s.dt*0.5)*s.dt;
    v=v+ac((xold+v*s.dt*0.5),(v+ac(xold,v,0)*s.dt*0.5),0)*s.dt;
      
  }
  printf("%lg %.14lf %.14lf\n", i*s.dt, x, v);
}


void rk4(param s){
  double x,dx1, dx2, dx3, dx4;
  double v,dv1, dv2, dv3, dv4;
  int i;
  x=s.x;
  v=s.v;
  for(i=0;i<s.n;i++){
    printf("%lg %.14lf %.14lf\n",i*s.dt ,x ,v);
    dx1=v*s.dt;
    dv1=ac(x,0,i*s.dt)*s.dt;
    dx2=(v+dv1/2)*s.dt;
    dv2=ac(x+dx1/2,0,i*s.dt+s.dt/2)*s.dt;
    dx3=(v+dv2/2)*s.dt;
    dv3=ac(x+dx2/2,0,i*s.dt+s.dt/2)*s.dt;
    dx4=(v+dv3)*s.dt;
    dv4=ac(x+dx3,0,i*s.dt+s.dt)*s.dt;
    x=x+(dx1+2*dx2+2*dx3+dx4)/6;
    v=v+(dv1+2*dv2+2*dv3+dv4)/6;
  }
  printf("%lg %.14lf %.14lf\n", i*s.dt, x, v);
}

int main(int argc, char *argv[]){
    param rb;
    int control;

    if(argc=!6){
        fprintf(stderr,"Hai sbagliato a inserire qualche dato\n");
        fprintf(stderr,"usage:%s velocita' dt ndt tmax algoritmo\n",argv[0]);
        fprintf(stderr,"sto uscendo...\n");
        exit(EXIT_FAILURE);
  }

    rb.x = 10;
    printf("#Posizione iniziale-->%lg\n",rb.x);
    rb.v = -(atof(argv[1]));
    printf("#Velocita' iniziale-->%lg\n",rb.v);
    rb.dt = atof(argv[2]);
    printf("#Passo di integrazione-->%g\n",rb.dt);
    rb.ndt = atoi(argv[3]);
    rb.tmax = atoi(argv[4]);
    printf("#Tempo massimo di integrazione-->%i\n",rb.tmax);
    rb.algo = atoi(argv[5]);

    
    rb.n=ceil(rb.tmax/rb.dt);
    printf("#Numero di passi-->%i\n",rb.n);

  if(rb.algo==1){
    printf("#E' stato utilizzato l'algoritmo di Eulero\n");
    printf("#Legenda\n");
    printf("#1:t 2:x 3:v 4:dE/Eo 5:E\n");
    eulero(rb);
    control=1;
  }
    
  if(rb.algo==2){
    printf("#E' stato utilizzato l'algoritmo di Eulero-Cromer\n");
    printf("#Legenda\n");
    printf("#1:t 2:x 3:v 4:dE/Eo 5:E\n");
    cromer(rb);
    control=1;
  }

  if(rb.algo==3){
    printf("#E' stato utilizzato l'algoritmo Leap Frog\n");
    printf("#Legenda\n");
    printf("#1:t 2:x 3:v 4:dE/Eo 5:dt\n");
    for(int i=0; i<rb.ndt; i++){
      leapfrog(rb);
      rb.dt/=2;
      rb.n*=2;
    }
    control=1;
  }

  if(rb.algo==4){
    printf("#E' stato utilizzato l'algoritmo Verlet-Velocita'\n");
    printf("#Legenda\n");
    printf("#1:t 2:x 3:v 4:dE/Eo 5:E\n");
    verletvelocita(rb);
    control=1;
  }

  if(rb.algo==5){
    printf("#E' stato utilizzato l'algoritmo Verlet-Posizioni\n");
    printf("#Legenda\n");
    printf("#1:t 2:x 3:v 4:dE/Eo 5:E\n");
    verletposizioni(rb);
    control=1;
  }

  if(rb.algo==6){
    printf("#E' stato utilizzato l'algoritmo Runge-Kutta del secondo ordine\n");
    printf("#Legenda\n");
    printf("#1:t 2:x 3:v 4:dE/Eo 5:E\n");
    rk2(rb);
    control=1;
  }

  if(rb.algo==7){
    printf("#E' stato utilizzato l'algoritmo Runge-Kutta del quarto ordine\n");
    printf("#Legenda\n");
    printf("#1:t 2:x 3:v 4:dE/Eo 5:E\n");
    rk4(rb);
    control=1;
  }
  
  
  if(control==0){
    fprintf(stderr,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    fprintf(stderr,"Attenzione\n");
    fprintf(stderr,"C'e' stato un errore nella scelta dell'algoritmo, sto uscento...\n");
    fprintf(stderr,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  }

  exit(EXIT_SUCCESS);


  
}
