#include <stdlib.h>
#include <stdio.h>
#include <math.h>



void initconf(double p, int N, int M, int *label){
    int i, num, m, index;
    int *list;

    

    list=(int *)calloc(N,sizeof(int));
    if(list==NULL){
    fprintf(stderr,"Errore nell'allocazione");
    exit(EXIT_FAILURE);
    }

    for(i=0;i<N;i++){
        list[i]=i;
        label[i]=-1;
    }

    num=N;

    for(m=0;m<M;m++){

        index=(int)(drand48()*num);

        label[list[index]]=list[index];

        list[index]=list[--num];
    }

}

int etichetta(int L, int N, int *label){
    int i, inew, numchanges, changes=0;
    do{
        numchanges=0;
        for(i=0; i<N; i++){
            if(label[i]!=-1){

                inew=i+1;
                if(inew<N){
                    if(label[inew]!=-1 && label[inew]!=label[i]){

                        if(label[inew]<label[i]){
                            label[i]=label[inew];
                        }else{
                            label[inew]=label[i];
                        }

                        numchanges++;
                        changes++;
                    }
                }

                inew=i+L;
                if(inew<N){
                    if(label[inew]!=-1 && label[inew]!=label[i]){

                        if(label[inew]<label[i]){
                            label[i]=label[inew];
                        }else{
                            label[inew]=label[i];
                        }

                        numchanges++;
                        changes++;
                    }
                }
             
            }
        }
    }while(numchanges);

    return(changes);
}

void misura(double p, int N, int L, int M, int *label, int changes){
    int *size, i, j, nclu, perc, sperc;
    long int m2, snperc;
    perc=0;
    nclu=0;
    sperc=0;
    m2=0;

    size=(int *)calloc(N,sizeof(int));
    if(size==NULL){
    fprintf(stderr,"Errore nell'allocazione");
    exit(EXIT_FAILURE);
    }

    for(i=0;i<N;i++){
        if(label[i]!=-1){
            size[label[i]]++;
        }
    }

    for(i=0;i<N;i++){
        if(size[i]!=0){
            nclu++;
            m2+=size[i]*size[i];
        }
    }

    for(i=0;i<L;i++){
        if(label[i]!=-1){
            for(j=N-L;j<N;j++){
                if(label[i]==label[j]){
                    perc=1;
                    sperc=size[label[i]];
                    for(i=0;i<N;i++){
                        if(i!=label[i]){
                            snperc+=size[i];
                        }
                    }
                }
            }
        }
    }

    printf("%lg %i %i %i %li %i %i %li %i \n", p, L, M, perc, m2, nclu, sperc, snperc, changes);

}

int main(int argc, char *argv[]){
    int L, nconf, k, N, *label, M, changes;
    double dp, p;
    unsigned int seed;

    FILE *devran = fopen("/dev/random","r");
    fread(&seed, 4, 1, devran);
    fclose(devran);

    srand48(seed);

    if (argc !=4 ){
        fprintf(stderr,"==============================ERRORE==============================\n");
        fprintf(stderr,"Hai sbagliato a inserire qualche parametro\n");
        fprintf(stderr,"usage:%s 1)L 2)dp 3)nconf\n", argv[0]);
        fprintf(stderr,"STO USCENDO...\n");
        fprintf(stderr,"=================================================================\n");
        exit(EXIT_FAILURE);
    }

    L=atoi(argv[1]);
    dp=atof(argv[2]);
    nconf=atoi(argv[3]);

    N=L*L;

    printf("#Legenda: 1)p 2)L 3)M 4)Percola/non percola 5)m2 6)#cluster 9)Taglia cluster percolante 10)Somma taglie cluster non percolanti 11)t\n");

    for(p=dp; p<1; p+=dp){

        M=(int)(p*N);

        for(k=0; k<nconf; k++){

            label=(int *)calloc(N,sizeof(int));
            if(label==NULL){
            fprintf(stderr,"Errore nell'allocazione");
            exit(EXIT_FAILURE);
            }


            initconf(p, N, M, label);
            changes=etichetta(L, N, label);
            misura(p, N, L, M, label, changes);

        }
    }
}