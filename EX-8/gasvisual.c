#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

typedef struct{
    int ix, iy;
} particle;

void printConf(particle *part, int M, int t){
    FILE *conf = fopen("conf.dat","w");
    int i;

    for(i=0;i<M;i++){

        fprintf(conf,"%i %i\n", part[i].ix, part[i].iy);
            
    }

    fclose(conf);
}

void initConfig(int L, int *occ, int N, particle *part, int M, int scelta){
    int i, m, j, num, index;
    int *list;

    list=(int *)calloc(N,sizeof(int));
    if(list==NULL){
    fprintf(stderr,"Errore nell'allocazione");
    exit(EXIT_FAILURE);
    }

    for(i=0;i<N;i++){
        list[i]=i;
        occ[i]=-1;
    }

    num=N;

    if(scelta==1){

        for(m=0;m<M;m++){

            index=(int)(drand48()*num);

            occ[list[index]]=m;

            list[index]=list[--num];
        }
    }

    if(scelta==2){
        for(m=0;m<M;m++){
            occ[(N/2)-(M/2)+m]=m;
        }
    }

    for(j=0;j<N;j++){
        if(occ[j] != -1){

            part[occ[j]].ix=j%L;
            part[occ[j]].iy=(int)(j/L);
            
        }
    }
            
}

void oneStep(int L, int *occ, particle *part, int M){
    double r;
    int ix, iy, ixnew, iynew, m;

    for(m=0;m<M;m++){

        r=drand48();

        ix=part[m].ix;
        iy=part[m].iy;

        if(r<0.25){

            ixnew=(ix+1)%L;
            iynew=iy;

        }else if(r<0.50){

            ixnew=(ix-1+L)%L;
            iynew=iy;

        }else if(r<0.75){

            iynew=(iy+1)%L;
            ixnew=ix;

        }else {

            iynew=(iy-1+L)%L;
            ixnew=ix;

        }

        if( occ[ixnew+iynew*L] == -1){

            occ[ixnew+L*iynew]=m;
            occ[ix+L*iy]=-1;

            part[m].ix=ixnew;
            part[m].iy=iynew;
        
        }
    }

}

int main(int argc, char *argv[]){
    int L, N, *occ, M, tmax, t, meastime, scelta;
    double rho;
    unsigned int seed;
    particle *part;

    FILE *devran = fopen("/dev/random","r");
    fread(&seed, 4, 1, devran);
    fclose(devran);

    FILE *gnu = popen("gnuplot -persist","w");

    if (argc !=5 ){
        fprintf(stderr,"==============================ERRORE==============================\n");
        fprintf(stderr,"Hai sbagliato a inserire qualche parametro\n");
        fprintf(stderr,"usage:%s 1)L 2)rho  3)tmax 4)scelta\n", argv[0]);
        fprintf(stderr,"STO USCENDO...\n");
        fprintf(stderr,"=================================================================\n");
        exit(EXIT_FAILURE);
    }
    
    L=atoi(argv[1]);
    rho=atof(argv[2]);
    tmax=atoi(argv[3]);
    scelta=atoi(argv[4]);


    srand48(seed);

    fprintf(gnu,"set size square\n");
    fprintf(gnu,"unset key\n");
    fprintf(gnu,"unset zeroaxis\n");
    fprintf(gnu,"set xra [-0.5:%g]\n", L-0.5);
    fprintf(gnu,"set yra [-0.5:%g]\n", L-0.5);
    fprintf(gnu,"set datafile separator ' '\n");


    N=L*L;
    M=(int)(rho*N);

    occ=(int *)calloc(N,sizeof(int));
    if(occ==NULL){
    fprintf(stderr,"Errore nell'allocazione");
    exit(EXIT_FAILURE);
    }

    part=(particle *)calloc(M,sizeof(particle));
    if(part==NULL){
    fprintf(stderr,"Errore nell'allocazione");
    exit(EXIT_FAILURE);
    }

    initConfig(L, occ, N, part, M, scelta);

    t=0;

    printConf(part, M, t);

    fprintf(gnu,"p 'conf.dat' w d\n");
    fflush(gnu);

    meastime=1;

    for(t=1;t<=tmax;t++){

        oneStep(L, occ, part, M);

        if(t==meastime){
            
            printConf(part, M, t);
            usleep(10000);
            fprintf(gnu,"p 'conf.dat' w d\n");
            fflush(gnu);

            meastime +=100;
        }
    }

    
    free(occ);
    free(part);

    return (EXIT_SUCCESS);
}