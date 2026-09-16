#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
  double x, v, t_max, W, dt,temp,Eo,E, dE, t=0;
  int n;
  int i; 
  fprintf(stderr,"Inserisci la posizione iniziale (in metri)\n");
  scanf("%lf",&x);
  fprintf(stderr,"Inserisci la velocita' i niziale (in metri al secondo)\n");
  scanf("%lf",&v);
  fprintf(stderr"Inserisci il tempo massimo di integrazione (in secondi)\n");
  scanf("%lf",&t_max);
  fprintf(stderr"Inserisci il passo di tempo (in secondi)\n");
  scanf("%lf",&dt);
  fprintf(stderr"Inserisci la pulsazione al quadrato\n");
  scanf("%lf",&W);

  Eo=(0,5*(v*v))+(0.5*W*(x*x));
  
  printf("%lf %lf %lf %lf %lf\n", x, v, t_max, dt, W);
  
  n = t_max/dt;

  printf("questo è il numero di passi totali: %i\n",n);

  fp=fopen("eulero.dat","w");
  fprintf(fp,"%lf %lf %lf %lf\n",t, x ,v ,dE);

  for(i=0;i<n;i++){
    temp=v;
    v=v-W*x*dt;
    x=x+temp*dt;
    E=(0,5*(v*v))+(0.5*W*(x*x));
    dE=(E-Eo)/Eo;
    t=i*dt;
    fprintf(fp,"%lf %lf %lf %lf\n",t ,x ,v,dE);
  }

  fclose(fp);  
}
