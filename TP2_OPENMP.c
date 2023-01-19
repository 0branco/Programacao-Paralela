#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

typedef struct {
    float x;
    float y;
    int c;
} ponto;


typedef struct 
{
    float x; // coordenada x
    float y; // coordenada y
} cluster;

ponto *p = NULL; 
cluster *clus = NULL; 



void inicializa(int N,int K,int T)
{
    int i;
    p = malloc(N*sizeof(ponto));
    clus = malloc(K*sizeof(cluster));
    srand(10);
    for (i = 0; i < N; i++) 
    {
        p[i].x = (float) rand() / RAND_MAX; 
        p[i].y = (float) rand() / RAND_MAX; 
        p[i].c = 0;
    }
    for (i = 0; i < K; i++) {
        clus[i].x = p[i].x; // x do centroide assume o valor do x do ponto
        clus[i].y = p[i].y; // y do centroide assume o valor do y do ponto
        p[i].c = i;
    }
}
int atribui_clus(int N,int K,int T)
{
    int i,j,r=0;
    float d1,d2,r1,r2;
    #pragma omp parallel num_threads(T)
    #pragma omp for reduction(+:r) private(d1,d2,r1,r2)
    for(i=0;i<N/2;i++){
        d1= ( (p[i].x-clus[p[i].c].x) * (p[i].x-clus[p[i].c].x) )+( (p[i].y-clus[ p[i].c ].y ) * (p[i].y-clus[ p[i].c ].y ) );
        r1= ( (p[N-i].x-clus[p[N-i].c].x) * (p[N-i].x-clus[p[N-i].c].x) )+( (p[N-i].y-clus[ p[N-i].c ].y ) * (p[N-i].y-clus[ p[N-i].c].y ) );
        for(j=0;j<K;j++)
	{
            d2 =( (p[i].x-clus[j].x) * (p[i].x-clus[j].x) )+( (p[i].y-clus[j].y ) * (p[i].y-clus[j].y ) );//faz distancia do ponto ao cluster de j
            r2 =( (p[N-i].x-clus[j].x) * ( p[N-i].x-clus[j].x) )+( (p[N-i].y-clus[j].y ) * (p[N-i].y-clus[j].y ) );
            if (d2 < d1)
	    {
                d1 = d2;
                p[i].c = j;
                r=1;
            }
            if(r2 < r1 )
	    {
                r1 = r2;
                p[N-i].c = j;
                r=1;
            }
        }
    }
    return r;
}
void verifica(int N,int K,int T) 
{
    int i;
    int size[K];
    float xp[K],yp[K];
    for (int i=0;i<K;i++) size[i]=0;
    for(i=0;i<K;i++)
    {
        xp[i]=0;
        yp[i]=0;
        size[i]=0;
    }
    #pragma omp parallel num_threads(T)
    #pragma omp for reduction(+:xp,yp,size)
    for(i=0;i<N;i++)
    {
        xp[p[i].c] +=p[i].x;
        yp[p[i].c] +=p[i].y;
        size[p[i].c]++;
    }
    for (i=0; i<K; i++) 
    {
        clus[i].x=xp[i]/size[i];
        clus[i].y=yp[i]/size[i];
    }

}

int main(int argc, char** argv)
{
    int N = atoi(argv[1]);    
    int K = atoi(argv[2]);    
    int T = atoi(argv[3]);
    inicializa(N,K,T);
    int iter=1;
    while(atribui_clus(N,K,T) && iter!=20)
    {
        verifica(N,K,T);
        iter++;
    }
    int tam[K];
    for(int i=0;i<K;i++) tam[i]=0;
    for(int i=0;i<N;i++) tam[p[i].c ]++;
    printf("N = %d, K = %d,  T = %d\n",N,K,T);
    for(int i=0;i<K;i++) 
    {
        printf("Center: (%.3f, %.3f) : Size: %d\n",clus[i].x,clus[i].y,tam[i]);
    }
    printf("Iterations: %d\n",iter);
    free(p);
    free(clus);
    return 0;

}