#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int *label, *size;

int clusterOf(int site){
    while(label[site]!=site){
        site=label[site];
    }

    return site;
}

void mergeClusters(int i, int j){
    if(i<j){
        label[j]=i;
        size[i]+=size[j];
    } else{
        label[i]=j;
        size[j]+=size[i];
    }
}

int main(int argc, char *argv[]){
    int i, j, M, L, N, *list, num, mmin, mmax, dm;
    int is, nconf, perc, sperc, nclust, numChanges;
    unsigned long long int m2;
    double pmin, pmax, dp;
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

    mmin=(int)(pmin*N+0.5);
    mmax=(int)(pmax*N+0.5);
    dm=(int)(dp*N+0.5);

    printf("#Legenda: 1)L 2)M 3)Percola/non percola 4)taglia cluster percolante 5)m2 6)#cluster 7)t\n");

    for(is=0;is<nconf;is++){
        for(i=0;i<N;i++){
            label[i]=-1;
            size[i]=0;
            list[i]=i;
        }
    

        num=N;
        numChanges=0;

        for(M=mmin;M<=mmax;M+=dm){
            while(num>N-M){
                j=(int)(drand48()*num);
                i=list[j];
                list[j]=list[--num];
                label[i]=i;
                size[i]=1;

                if((i-L)>=0 && label[i-L]!=-1 && clusterOf(i) != clusterOf(i-L)){
                    mergeClusters(clusterOf(i), clusterOf(i-L));
                    numChanges++;
                }

                if((i-1)>=0 && label[i-1]!=-1 && clusterOf(i) != clusterOf(i-1)){
                    mergeClusters(clusterOf(i), clusterOf(i-1));
                    numChanges++;
                }

                if((i+L)<N && label[i+L]!=-1 && clusterOf(i) != clusterOf(i+L)){
                    mergeClusters(clusterOf(i), clusterOf(i+L));
                    numChanges++;
                }

                if((i+1)<N && label[i+1]!=-1 && clusterOf(i) != clusterOf(i+1)){
                    mergeClusters(clusterOf(i), clusterOf(i+1));
                    numChanges++;
                }
            }

            perc=0;
            sperc=0;
            for(i=N-L;i<N;i++){
                if(label[i]!=-1 && clusterOf(i)<L){
                    perc=1;
                    sperc=size[clusterOf(i)];
                }
            }

            nclust=0;
            m2=0;
            for(i=0;i<N;i++){
                if(label[i]!=-1 && i==clusterOf(i)){
                    nclust++;
                    m2 += (unsigned long long int)size[i]*size[i];
                }
            }

            printf("%i %i %i %i %llu %i %i\n", L, M, perc, sperc, m2, nclust,numChanges);
            numChanges=0;

        }
    }

    free(list);
    free(label);
    free(size);
}
