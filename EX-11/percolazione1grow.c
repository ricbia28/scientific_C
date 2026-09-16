#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int *label, *size, *list;

void initconf(double p, int N, int M){
    int i, m, num, index;

    for(i=0;i<N;i++){
        label[i]=-1;
        size[i]=0;
        list[i]=i;
    }

    num=N;
                
    for(m=0;m<M;m++){
        index=(int)(drand48()*num);

        label[list[index]]=list[index]; 
        list[index]=list[--num];

    }
}

int etichetta(int L, int N){
    int i, inew, numChanges, changes=0;
    do{
        numChanges=0;
        for(i=0; i<N; i++){

            inew=i-1;
            if(inew>=0 && label[i]!=-1 && label[inew]!=-1 && label[inew]!=label[i]){
                        
                if(label[inew]<label[i]){
                    label[i]=label[inew];
                }else{
                    label[inew]=label[i];
                }
                numChanges++;
                changes++;
            }

            inew=i-L;
            if(inew>=0 && label[i]!=-1 && label[inew]!=-1 && label[inew]!=label[i]){

                if(label[inew]<label[i]){
                    label[i]=label[inew];
                }else{
                    label[inew]=label[i];
                }

                numChanges++;
                changes++;
            }

            inew=i+1;
            if(inew<N && label[i]!=-1 && label[inew]!=-1 && label[inew]!=label[i]){

                if(label[inew]<label[i]){
                    label[i]=label[inew];
                }else{
                    label[inew]=label[i];
                }

                numChanges++;
                changes++;
            }
                        

            inew=i+L;
            if(inew<N && label[i]!=-1 &&label[inew]!=-1 && label[inew]!=label[i]){

                if(label[inew]<label[i]){
                    label[i]=label[inew];
                }else{
                    label[inew]=label[i];
                }

                numChanges++;
                changes++;
            }
                    
        }
    }while(numChanges);

    return changes;

}

void misura(double p, int N, int L, int M, int changes){
    int i, perc, sperc, nclust;
    unsigned long long int m2;

    for(i=0;i<N;i++){
        if(label[i]!=-1){
            size[label[i]]++;
        }
    }       
        

    perc=0;
    sperc=0;
            
    for(i=N-L;i<N;i++){
        if(label[i]!=-1 && label[i]<L){
            perc=1;
            sperc=size[label[i]];
        }
    }

    nclust=0;
    m2=0;
    for(i=0;i<N;i++){
        if(label[i]!=-1 && i==label[i]){
            nclust++;
            m2 += (unsigned long long int)size[i]*size[i];
        }
    }

    printf("%i %i %i %i %llu %i %i\n", L, M, perc, sperc, m2, nclust, changes);
            

}


int main(int argc, char *argv[]){
    int k, M, L, N, nconf, changes;
    double pmin, pmax, dp, p;
    unsigned int seed;

    FILE *devran = fopen("/dev/random","r");
    fread(&seed, 4, 1, devran);
    fclose(devran);

    if (argc !=6 ){
        fprintf(stderr,"==============================ERRORE==============================\n");
        fprintf(stderr,"Hai sbagliato a inserire qualche parametro\n");
        fprintf(stderr,"usage:%s 1)L 2)dp 3)pmin 4)pmax 5)nconf\n", argv[0]);
        fprintf(stderr,"STO USCENDO...\n");
        fprintf(stderr,"=================================================================\n");
        exit(EXIT_FAILURE);
    }

    L=atoi(argv[1]);
    dp=atof(argv[2]);
    pmin=atof(argv[3]);
    pmax=atof(argv[4]);
    nconf=atoi(argv[5]);

    srand48(seed);

    N=L*L;

    label=(int *)calloc(N,sizeof(int));
    if(label==NULL){
    fprintf(stderr,"Errore nell'allocazione");
    exit(EXIT_FAILURE);
    }

    size=(int *)calloc(N,sizeof(int));
    if(size==NULL){
    fprintf(stderr,"Errore nell'allocazione");
    exit(EXIT_FAILURE);
    }

    list=(int *)calloc(N,sizeof(int));
    if(list==NULL){
    fprintf(stderr,"Errore nell'allocazione");
    exit(EXIT_FAILURE);
    }

    printf("#Legenda: 1)L 2)M 3)Percola/non percola 4)taglia cluster percolante 5)m2 6)#cluster 7)t\n");

    changes=0;
    for(k=0;k<nconf;k++){
        for(p=pmin;p<=pmax;p+=dp){

            M=(int)(p*N);

            initconf(p, N, M);
            changes=etichetta(L, N);
            misura(p, N, L, M, changes);      
        }
    }

    free(list);
    free(label);
    free(size);
}