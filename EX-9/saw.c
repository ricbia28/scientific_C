#include <stdlib.h>
#include <stdio.h>
#include <math.h>
 
typedef struct{
    int ntra, tmax;
    unsigned int seed;
} param;
 
param input(int argc, char *argv[]){
    param rb;
    if (argc != 3){
        fprintf(stderr,"==============================ERRORE==============================\n");
        fprintf(stderr,"Hai sbagliato a inserire qualche parametro\n");
        fprintf(stderr,"usage:%s 1)numero di traiettorie generate 2)tempo massimo\n",argv[0]);
        fprintf(stderr,"STO USCENDO...\n");
        fprintf(stderr,"=================================================================\n");
        exit(EXIT_FAILURE);
    }
 
    FILE *devran = fopen("/dev/random","r");
    fread(&rb.seed, 4, 1, devran);
    fclose(devran);
 
 
    rb.ntra = atoi(argv[1]);
    rb.tmax = atoi(argv[2]);
 
    return rb;
}
 
 
 
void saw(param s){
    int t, i, j;
    double r, *dist;
    long int *X, *Y;
    srand48(s.seed);
 
    
 
    printf("#Legenda: 1)Istante 2)x 3)y\n");
    for(i=0;i<s.ntra;i++){
        X=(long int *)calloc(s.tmax+1, sizeof(long int));
        if(X==NULL){
        fprintf(stderr,"Errore nell'allocazione");
        exit(EXIT_FAILURE);
        }
 
        Y=(long int *)calloc(s.tmax+1, sizeof(long int));
        if(Y==NULL){
        fprintf(stderr,"Errore nell'allocazione");
        exit(EXIT_FAILURE);
        }

        X[0]=0;
        Y[0]=0;
 
        for(t=1;t<=s.tmax;t++){
 
            r=drand48();
 
            if(r < 0.25){
                X[t]=X[t-1]+1;
                Y[t]=Y[t-1];
            } else if (r < 0.50){
                X[t]=X[t-1]-1;
                Y[t]=Y[t-1];
            } else if (r < 0.75){
                X[t]=X[t-1];
                Y[t]=Y[t-1]+1;
            } else{
                X[t]=X[t-1];
                Y[t]=Y[t-1]-1;
            }

            for(j=t-1;j>=0;j--){

                if (X[t] == X[j] && Y[t] == Y[j]) {

                    t--;
                    
                    X[t]=X[t-1];
                    Y[t]=Y[t-1];
                    
                    j=-1;
                }

            }

        }
        for(t=0;t<=s.tmax;t++){
            printf("%i %li %li \n", t, X[t], Y[t]);
        }
        printf("\n\n");
    }
 
    free(Y);
    free(X);
}
 
 
 
int main(int argc, char *argv[]){
    param rb;
 
    rb=input(argc, argv);
 
    saw(rb);
 
    exit(EXIT_SUCCESS);
 
} 