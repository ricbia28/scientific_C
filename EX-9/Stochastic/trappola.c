#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]){
    long int x;
    int ntra, tmax, t, i, w;
    double r, *trap, *m2;
    unsigned int seed;

    if (argc != 3){
        fprintf(stderr,"==============================ERRORE==============================\n");
        fprintf(stderr,"Hai sbagliato a inserire qualche parametro\n");
        fprintf(stderr,"usage:%s 1)numero di traiettorie generate 2)tempo massimo\n",argv[0]);
        fprintf(stderr,"STO USCENDO...\n");
        fprintf(stderr,"=================================================================\n");
        exit(EXIT_FAILURE);
    }
 
    FILE *devran = fopen("/dev/random","r");
    fread(&seed, 4, 1, devran);
    fclose(devran);
 
 
    ntra = atoi(argv[1]);
    tmax = atoi(argv[2]);


    srand48(seed);

    m2=(double *)calloc(tmax+1,sizeof(double));
    if(m2==NULL){
    fprintf(stderr,"Errore nell'allocazione");
    exit(EXIT_FAILURE);
    }

    
    printf("#Legenda: 1)Istante 2)Posizione\n");

    for(i=1;i<=ntra;i++){

        trap=(double *)calloc(2*tmax+1,sizeof(double));
        if(trap==NULL){
        fprintf(stderr,"Errore nell'allocazione");
        exit(EXIT_FAILURE);
        }

        for(w=0;w<=2*tmax;w++){
            trap[w]=drand48();
        }
    

        x=0;

        for(t=0;t<tmax;t++){

            m2[t]+=x*x;

            printf("%i %li\n", t, x);

            r=drand48();

            if(r<trap[x+tmax]){

                x+=1;

            } else {

                x-=1;

            }

            

        } 

        m2[t]+=x*x;

        printf("%i %li\n\n", t, x);
    } 


    FILE *dist = fopen("dist.dat","w");
    for(t=1;t<tmax;t++){

        fprintf(dist,"%i %lg\n",t, (double)m2[t]/ntra);
            
    }
    fclose(dist);

    free(trap);
    free(m2);

}