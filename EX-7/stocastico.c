#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct{
    int ntra, tmax, scelta, b, imax;
    unsigned int seed;
} param;

param input(int argc, char *argv[]){
    param rb;
    if (argc > 5 || argc <4){
        fprintf(stderr,"==============================ERRORE==============================\n");
        fprintf(stderr,"Hai sbagliato a inserire qualche parametro\n");
        fprintf(stderr,"usage:%s 1)numero di traiettorie generate 2)tempo massimo di integrazione 3)modalita' 4)potenza\n\n",argv[0]);
        fprintf(stderr,"Modalita' 1: Stampa le varie traiettorie generate\n\n");
        fprintf(stderr,"Modalita' 2: Stampa media e varianza con relativi errori per i diversi istanti \n\n");  
        fprintf(stderr,"Modalita' 3: Crea istogrammi per diversi istanti \n\n"); 
        fprintf(stderr,"Il parametro 'potenza' serve per le modalità 2 e 3 e sarebbero le potenze da prendere come istanti,\n"); 
        fprintf(stderr,"Se si usa la modalità 1 può essere omesso, se non viene inserito verrà posto uguale a 2 \n\n"); 
        fprintf(stderr,"Il seed per la generazione di numeri random verrà generato automaticamente\n\n"); 
        fprintf(stderr,"STO USCENDO...\n");
        fprintf(stderr,"=================================================================\n");
        exit(EXIT_FAILURE);
    }

    rb.scelta = atoi(argv[3]);

    if(rb.scelta < 1 || rb.scelta > 3){
    fprintf(stderr,"==============================ERRORE==============================\n");
    fprintf(stderr,"C'e' stato un errore nella scelta della modalità\n");
    fprintf(stderr,"STO USCENDO...\n");
    fprintf(stderr,"=================================================================\n");
    exit(EXIT_FAILURE);
    }

    if(argv[4]==NULL) rb.b = 2;
    else rb.b = atoi(argv[4]);


    FILE *devran = fopen("/dev/random","r");
    fread(&rb.seed, 4, 1, devran);
    fclose(devran);


    rb.ntra = atoi(argv[1]);
    rb.tmax = atoi(argv[2]);

    return rb;
}


void statistica(long long int *m1, long long int *m2, long long int *m4, param s){
    int t, meastime;
    double med, var, smed, svar;

    s.imax = 1;
    meastime = s.b;
    while(meastime <= s.tmax){
        s.imax++;
        meastime *= s.b;
    }
    printf("#Numero traiettorie utilizzate-> %i \n#Tempo massimo-> %i\n",s.ntra, s.tmax);
    printf("#Legenda:\n #1)istante 2)media 3)errore sulla media 4)varianza 5)errore sulla varianza\n");
    meastime=1;
    for(t=0;t<s.tmax;t++){
        if(t==meastime){
        med=((double)(m1[t]))/s.ntra;
        var=((double)(m2[t]))/s.ntra;
        smed=sqrt(var/s.ntra);
        svar=sqrt((((double)(m4[t])/s.ntra)-t*t)/s.ntra);
        printf("%i %lf %lf %lf %lf\n",t , med , smed, var, svar);
        meastime*=s.b;
        }
    }
   //printf("%i %lf %lf %lf %lf\n",t , med , smed, var, svar);
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

void istogrammi(param s){
    long long int x;
    int i, t, n, j, meastime;
    int **h;

    srand48(s.seed);

    s.imax = 1;
    meastime = s.b;
    while(meastime <= s.tmax){
        s.imax++;
        meastime *= s.b;
    }

    h = (int**)calloc(s.imax,sizeof(int *));
    if(h==NULL){
    fprintf(stderr,"Errore nell'allocazione");
    exit(EXIT_FAILURE);
    }

    meastime=1;

    for(i=0;i<s.imax;i++){

        h[i] = (int*)calloc(meastime+1,sizeof(int));
        if(h[i]==NULL){
        fprintf(stderr,"Errore nell'allocazione");
        exit(EXIT_FAILURE);
        }

        meastime *= s.b;
    }

    for(n=0;n<s.ntra;n++){
        x=0;
        meastime=1;
        i=0;
        for(t=1;t<=s.tmax;t++){
            x+=-1+2*(int)(drand48()*2);
            if(t==meastime){
                h[i][(x+meastime)/2]++;
                meastime*=s.b;
                i++;
            }
        } 
    }

    meastime=2;

    printf("#Numero traiettorie utilizzate-> %i \n#Tempo massimo-> %i\n",s.ntra, s.tmax);

    for(i=1;i<s.imax;i++){ 
        for(j=0;j<=meastime;j++){
            printf("%i %i %lg\n",meastime, 2*j-(meastime), (double)(h[i][j])/s.ntra);
        }
        printf("\n\n");
        meastime*=s.b;
    
    }

    free(h);
}


int main(int argc, char *argv[]){
    param rb;

    rb=input(argc, argv);

    if(rb.scelta==1)traiettorie1(rb);

    if(rb.scelta==2)traiettorie2(rb);

    if(rb.scelta==3)istogrammi(rb);

    exit(EXIT_SUCCESS);

} 