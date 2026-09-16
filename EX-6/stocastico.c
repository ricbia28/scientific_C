#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct{
    int ntra, tmax, scelta;
    unsigned int seed;
} param;

param input(int argc, char *argv[]){
    param rb;
    if (argc > 5 || argc <4){
        fprintf(stderr,"==============================ERRORE==============================\n");
        fprintf(stderr,"Hai sbagliato a inserire qualche parametro\n");
        fprintf(stderr,"usage:%s 1)numero di traiettorie generate 2)tempo massimo di integrazione 3)modalita' 4)seed\n\n",argv[0]);
        fprintf(stderr,"Modalita' 1: Stampa le varie traiettorie generate\n\n");
        fprintf(stderr,"Modalita' 2: Stampa media e varianza con relativi errori per i diversi istanti \n\n");  
        fprintf(stderr,"Se il seed non verrà inserito, verrà generato automaticamente\n\n"); 
        fprintf(stderr,"STO USCENDO...\n");
        fprintf(stderr,"=================================================================\n");
        exit(EXIT_FAILURE);
    }

    rb.scelta = atoi(argv[3]);

    if(rb.scelta != 1 && rb.scelta != 2){
    fprintf(stderr,"==============================ERRORE==============================\n");
    fprintf(stderr,"C'e' stato un errore nella scelta dell'algoritmo\n");
    fprintf(stderr,"STO USCENDO...\n");
    fprintf(stderr,"=================================================================\n");
    exit(EXIT_FAILURE);
    }

    if(argv[4]==NULL) {
        FILE *devran = fopen("/dev/random","r");
        fread(&rb.seed, 4, 1, devran);
        fclose(devran);
    }
    else rb.seed = atoi(argv[4]);

    rb.ntra = atoi(argv[1]);
    rb.tmax = atoi(argv[2]);

    return rb;
}


void statistica(long long int *m1, long long int *m2, long long int *m4, param s){
    int t;
    double med, var, smed, svar;
    printf("Legenda:\n 1)istante 2)media 3)errore sulla media 4)varianza 5)errore sulla varianza");
    for(t=0;t<s.tmax;t++){
        printf("%i %lf %lf %lf %lf\n",t , med , smed, var, svar);
        med=((double)(m1[t]))/s.ntra;
        var=((double)(m2[t]))/s.ntra;
        smed=sqrt(var/s.ntra);
        svar=sqrt((((double)(m4[t])/s.ntra)-t*t)/s.ntra);

    }
    printf("%i %lf %lf %lf %lf\n",t , med , smed, var, svar);
}

void traiettorie1(param s){
    long int x;
    int t, i;
    srand48(s.seed);
    printf("#Legenda: 1)Istante 2)Posizione\n");
    for(i=1;i<=s.ntra;i++){
        x=0;
        for(t=0;t<s.tmax;t++){
            printf("%i %li\n",t ,x);
            x+=-1+2*(int)(drand48()*2);
        }
     printf("%i %li\n\n",t ,x);   
    }   
}

void traiettorie2(param s){
    long int x;
    int t, i;
    long long int *m1, *m2, *m4;

    srand48(s.seed);

    m1=(long long int *)calloc(s.tmax+1,sizeof(double));
    if(m1==NULL){
    fprintf(stderr,"Errore nell'allocazione");
    exit(EXIT_FAILURE);
    }

    m2=(long long int *)calloc(s.tmax+1,sizeof(double));
    if(m1==NULL){
    fprintf(stderr,"Errore nell'allocazione");
    exit(EXIT_FAILURE);
    }

    m4=(long long int *)calloc(s.tmax+1,sizeof(double));
    if(m4==NULL){
    fprintf(stderr,"Errore nell'allocazione");
    exit(EXIT_FAILURE);
    }

    printf("#Legenda: 1)Istante 2)Posizione\n");
    for(i=1;i<=s.ntra;i++){
        x=0;
        for(t=0;t<s.tmax;t++){
            m1[t]+=x;
            m2[t]+=x*x;
            m4[t]+=x*x*x*x;
            x+=-1+2*(int)(drand48()*2);
        } 
        m1[t]=x;
        m2[t]=x*x;
        m4[t]=x*x*x*x;
    }

    statistica(m1, m2, m4, s);

    free(m1);
    free(m2);
    free(m4);   
}

int main(int argc, char *argv[]){
    param rb;

    rb=input(argc, argv);

    if(rb.scelta==1)traiettorie1(rb);

    if(rb.scelta==2)traiettorie2(rb);

    exit(EXIT_SUCCESS);

} 