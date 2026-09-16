#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct{
    int ix, iy, distx, disty;
} particle;

void printConf(particle *part, int M){
    FILE *conf = fopen("conf.dat","w");
    int i;

    for(i=0;i<M;i++){

        fprintf(conf,"%i %i\n", part[i].ix, part[i].iy);
            
    }

    fclose(conf);
}

void initConfig(int L, int *occ, int N, particle *part, int M){
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

    for(m=0;m<M;m++){

        index=(int)(drand48()*num);

        occ[list[index]]=m;

        list[index]=list[--num];
    }

    for(j=0;j<N;j++){
        if(occ[j] != -1){

            part[occ[j]].ix=j%L;
            part[occ[j]].iy=(int)(j/L);
            part[occ[j]].distx=0;
            part[occ[j]].disty=0;
            
        }
    }
            
}

void oneStep(int L, int *occ, particle *part, int M, double *dist){
    double r;
    int ix, iy, ixnew, iynew, distx, disty, distxnew, distynew, m;

    for(m=0;m<M;m++){

        r=drand48();

        ix=part[m].ix;
        iy=part[m].iy;
        distx=part[m].distx;
        disty=part[m].disty;

        if(r<0.25){

            ixnew=(ix+1)%L;
            iynew=iy;
            distxnew=distx+1;
            distynew=disty;

        }else if(r<0.50){

            ixnew=(ix-1+L)%L;
            iynew=iy;
            distxnew=distx-1;
            distynew=disty;

        }else if(r<0.75){

            ixnew=ix;
            iynew=(iy+1)%L;
            distxnew=distx;
            distynew=disty+1;

        }else {

            ixnew=ix;
            iynew=(iy-1+L)%L;
            distxnew=distx;
            distynew=disty-1;

        }

        if( occ[ixnew+iynew*L] == -1){

            occ[ixnew+L*iynew]=m;
            occ[ix+L*iy]=-1;

            part[m].ix=ixnew;
            part[m].iy=iynew;
            part[m].distx=distxnew;
            part[m].disty=distynew;
        
        } 
 
    }

}

int main(int argc, char *argv[]){
    int L, N, *occ, M, m, tmax, t;
    double rho, *dist;
    unsigned int seed;
    particle *part;

    FILE *devran = fopen("/dev/random","r");
    fread(&seed, 4, 1, devran);
    fclose(devran);

    if (argc !=4 ){
        fprintf(stderr,"==============================ERRORE==============================\n");
        fprintf(stderr,"Hai sbagliato a inserire qualche parametro\n");
        fprintf(stderr,"usage:%s 1)L 2)rho 3)tmax\n", argv[0]);
        fprintf(stderr,"STO USCENDO...\n");
        fprintf(stderr,"=================================================================\n");
        exit(EXIT_FAILURE);
    }
    
    L=atoi(argv[1]);
    rho=atof(argv[2]);
    tmax=atoi(argv[3]);

    srand48(seed);

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

    dist=(double *)calloc(tmax,sizeof(double));
    if(dist==NULL){
    fprintf(stderr,"Errore nell'allocazione");
    exit(EXIT_FAILURE);
    }

    initConfig(L, occ, N, part, M);

    dist[0]=0;

    printConf(part, M);

    for(t=1;t<=tmax;t++){

        oneStep(L, occ, part, M, dist);

        for(m=0;m<M;m++){
        dist[t] += ((part[m].distx)*(part[m].distx)+(part[m].disty)*(part[m].disty));
        }

        printf("%i %lf\n", t, (double)dist[t]/M);

    }

    printConf(part, M);

    
    free(occ);
    free(part);
    free(dist);

    return (EXIT_SUCCESS);
}