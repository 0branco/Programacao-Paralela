#include <stdio.h>
#include <stdlib.h>

#define N 10000000 // N-> Número de pontos
#define K 4 // K-> Número de cters

typedef struct 
{
    float x; // coordenad x
    float y; // coordenada y do ponto
    int c; // clus a que pertence ponto: [0, 1, 2, 3]
} ponto;

 ponto *p = NULL; 
 ponto *clus = NULL; 

void inicializa()
{
    int i;
    p = malloc(N*sizeof(ponto));
    clus = malloc(K*sizeof(ponto));

    srand(10);
    for (i = 0; i < N/2; i++) 
    {
        p[i].x = (float) rand() / RAND_MAX; // coordenada x random
        p[i].y = (float) rand() / RAND_MAX; // coordenada y random
        p[i].c = 0;
        p[N-i].x=(float) rand() / RAND_MAX;
        p[N-i].y=(float) rand() / RAND_MAX;
        p[N-i].c = 0;
    }
    for (i = 0; i < K; i++) 
    {
        clus[i].x = p[i].x; 
        clus[i].y = p[i].y; 
        p[i].c = i;
    }
}

int atribui_clus()
{
    int i,j,var=0;
    float dist, dist2, r1, r2;
    for(i=0;i<N/2;i++)
    {
        ponto cons=p[i];
        ponto cons2=p[N-i];
        dist= ((cons.y-clus[cons.c ].y)*(cons.y-clus[ cons.c ].y)+(cons.x-clus[cons.c].x)*(cons.x-clus[cons.c].x));
        dist2= ((cons2.y-clus[cons2.c ].y)*(cons2.y-clus[ cons2.c ].y)+(cons2.x-clus[cons2.c].x)*(cons2.x-clus[cons2.c].x));
        for(j=0;j<K;j++)
        {
            r1=((cons.y-clus[j].y)*(cons.y-clus[j].y)+(cons.x-clus[j].x)*(cons.x-clus[j].x));
            r2=((cons2.y-clus[j].y)*(cons2.y-clus[j].y)+(cons2.x-clus[j].x)*(cons2.x-clus[j].x));
            if(r1<dist && j!=p[i].c)
            {
                dist = r1;
                p[i].c=j;
                var =1;
            }
            if(r2<dist2 && j!=p[N-i].c)
            {
                dist2 = r2;
                p[N-i].c=j;
                var=1;
            }
        }
    }
    return var;
}

void recalcula_clus()
{
    int i;
    int size[K];
    ponto centros[K]; 
    for(i=0; i<K; i++) 
    {
        centros[i].x = 0; 
        centros[i].y = 0;
        size [i] = 0; 
    }
    for(i=0; i<N/2; i++) 
    {
        centros[p[i].c].x += p[i].x;
        centros[p[N-i].c].x+=p[N-i].x;
        centros[p[i].c].y += p[i].y;
        centros[p[N-i].c].y +=p[N-i].y;
        size[p[i].c]++;
        size[p[N-i].c]++;
    }
    for (i=0; i<K; i++) 
    {
        centros[i].x = centros[i].x / size[i];
        centros[i].y = centros[i].y / size[i];
        clus[i] = centros[i];
    }
}

int iteracoes()
{
    int cnt=1;
    while(atribui_clus()!=0)
    {
        cnt++;
        recalcula_clus();
    }
    return cnt;
}

int main() 
{
    int size[K];
    int i;
    inicializa();
    int n=iteracoes();
    for(i=0;i<K;i++) size[i]=0;
    for(i=0;i<N/2;i++) 
    {
        size[p[i].c]++;
        size[p[N-i].c]++;
    }
    printf("N = %d, K = %d\n",N,K);
    for(i=0;i<K;i++) printf("Center: (%f, %f) : Size: %d\n",clus[i].x,clus[i].y,size[i]);
    printf("Iterations: %d\n",n);
    free(p);
    free(clus);
    return 0;
}
