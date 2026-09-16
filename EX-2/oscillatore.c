#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void eulero(double x, double v,double W, double Eo,double dt, int n ){
  double temp, E;
  printf("#E' stato utilizzato l'algoritmo di Eulero\n");
  printf("#Legenda\n");
  printf("#1:t 2:x 3:v 4:dE\n");
  printf("0 %lf %lf 0\n",x ,v);
  for(int i=1;i<=n;i++){
    temp=v;
    v=v-W*x*dt;
    x=x+temp*dt;
    E=(0.5*(v*v))+(0.5*W*(x*x));
    printf("%lf %lf %lf %lf\n",i*dt ,x ,v,(E-Eo)/Eo);
  }

}



void cromer(double x, double v,double W, double Eo,double dt, int n ){
  double E;
  printf("#E' stato utilizzato l'algoritmo di Eulero-Cromer\n");
  printf("#Legenda\n");
  printf("#1:t 2:x 3:v 4:dE\n");
  printf("0 %lf %lf 0\n",x ,v);
  for(int i=1;i<=n;i++){
    v=v-W*x*dt;
    x=x+v*dt;
    E=(0.5*(v*v))+(0.5*W*(x*x));
    printf("%lf %lf %lf %lf\n",i*dt ,x ,v,(E-Eo)/Eo);
  }

}

void leapfrog(double x, double v,double W, double Eo,double dt, int n ){
  double temp, E;
  printf("#E' stato utilizzato l'algoritmo Leap Frog\n");
  printf("#Legenda\n");
  printf("#1:t 2:x 3:v 4:dE\n");
  v=v-W*x*(dt/2);
  printf("0 %lf %lf 0\n",x ,v);
  for(int i=1;i<=n;i++){
    x=x+v*dt;
    v=v-W*x*dt;
    E=(0.5*(v*v))+(0.5*W*(x*x));
    printf("%lf %lf %lf %lf\n",i*dt ,x ,v+W*x*(dt/2),(E-Eo)/Eo);
  }

}

void message(){
  fprintf(stderr,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  fprintf(stderr,"Questo programma simula un oscillatore armonico utilizzando\n");
  fprintf(stderr,"i dati dati in input da tastiera e scrive i risultati ottenuti\n");
  fprintf(stderr,"dall'integrazione, utilizzando un algoritmo, sul file output.dat\n");
  fprintf(stderr,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n\n");
}


int scelta(){
  int s;
  fprintf(stderr,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  fprintf(stderr,"Possono essere utilizzati 3 diversi tipi di algoritmo:\n");
  fprintf(stderr,"Per utilizzare l'algoritmo di Eulero premere 1\n");
  fprintf(stderr,"Per utilizzare l'algoritmo di Eulero-Cromer premere 2\n");
  fprintf(stderr,"Per utilizzare l'algoritmo Leap Frog premere 3\n");
  fprintf(stderr,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  fprintf(stderr,"La sua scelta-->");
  scanf("%i",&s);
  return s;
}

int main(){
  double x,v,W,Eo,dt,tmax;
  int n, s, control=0;

  message();
  
  fprintf(stderr,"Inserire posizione iniziale--> ");
  scanf("%lf",&x);
  printf("#Posizione iniziale=%lf\n",x);
  
  fprintf(stderr,"Inserire Velocita' iniziale--> ");
  scanf("%lf",&v);
  printf("#Velocita' iniziale=%lf\n",v);
  
  fprintf(stderr,"Inserire la pulsazione al quadrato--> ");
  scanf("%lf",&W);
  printf("#Pulsazione al quadrato=%lf\n",W);
  
  fprintf(stderr,"Inserire il tempo massimo di integrazione--> ");
  scanf("%lf",&tmax);
  printf("#Tempo massimo di integrazione=%lf\n",tmax);
  
  fprintf(stderr,"Inserire il passo di tempo--> ");
  scanf("%lf",&dt); 
  printf("#Passo di tempo=%lf\n",dt);

  
  n=(int)(tmax/dt);
  if(n*dt<tmax)n++;
  Eo=(0,5*(v*v))+(0.5*W*(x*x));
  
  


  

  while(control==0){
    
    s=scelta();

    if(s==1){
      eulero(x,v,W,Eo,dt,n);
      control=1;
    }
    
    if(s==2){
      cromer(x,v,W,Eo,dt,n);
      control=1;
    }

    if(s==3){
      leapfrog(x,v,W,Eo,dt,n);
      control=1;
    }
    
    if(control==0){
      fprintf(stderr,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      fprintf(stderr,"Attenzione\n");
      fprintf(stderr,"C'e' stato un errore nella scelta dell'algoritmo\n");
      fprintf(stderr,"Riavvio della scelta\n");
      fprintf(stderr,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    }
    
  }

  return 0;
}
