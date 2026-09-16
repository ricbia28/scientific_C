#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
  double x, v, t_max, w, dt, t=0;
  int n;
  int i;
  FILE *fp;
  printf("Eulero\n");
  printf("Inserisci la posizione iniziale (in metri)\n");
  scanf("%lf",&x);
  printf("Inserisci la velocita' i niziale (in metri al secondo)\n");
  scanf("%lf",&v);
  printf("Inserisci il tempo massimo di integrazione (in secondi)\n");
  scanf("%lf",&t_max);
  printf("Inserisci il passo di tempo (in secondi)\n");
  scanf("%lf",&dt);
  printf("Inserisci la pulsazione\n");
  scanf("%lf",&w);
  printf("%lf %lf %lf %lf  %lf\n", x, v, t_max, dt, w);
  
  n = t_max/dt;

  printf("questo è il numero di passi totali: %i\n",n);

  fp=fopen("eulero.dat","w");
  fprintf(fp,"%lf %lf %lf\n",t, x ,v);

  for(i=0;i<n;i++){
    x=x+v*dt;
    v=v-(w*w)*x*dt;
    t=t+dt;
    fprintf(fp,"%lf %lf %lf\n",t ,x ,v);
  }

  fclose(fp);  
}
