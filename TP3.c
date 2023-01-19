#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <stdbool.h>

typedef struct ponto
{
    float x;
    float y;
    int c;

} ponto;

ponto *p = NULL;
ponto *clus = NULL;
ponto *clus_ant = NULL;


int N,K,T;


void inicializa( )
{
    int i;
    srand(10);
    p = malloc(N*sizeof(struct ponto));
    clus = malloc(K*sizeof(struct ponto));
    clus_ant = malloc(K*sizeof(struct ponto));
    for (i = 0; i < N; i++)
    {
        p[i].x = (float)rand() / RAND_MAX;
        p[i].y = (float)rand() / RAND_MAX;
        p[i].c = 0;
    }
    
    for (i = 0; i < K; i++)
    {
        clus[i].x = p[i].x;
        clus[i].y = p[i].y;
        clus[i].c=i;
        
    }
}
void store()
{
    int i=0;
    for (i = 0; i < K; i++)
    {
        clus_ant[i].x = clus[i].x;
        clus_ant[i].y = clus[i].y;
        clus[i].x =0; 
	clus[i].y = 0;
        clus[i].c = 0;
    }
}

void assign()
{
    float min_distance = 1.1;
    int i, j;
    float distance;
    #pragma omp parallel for reduction(min: min_distance)
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < K; j++)
        {
            distance = (clus_ant[j].x - p[i].x) * (clus_ant[j].x - p[i].x) + (clus_ant[j].y - p[i].y) * (clus_ant[j].y - p[i].y);
            if (distance < min_distance)
            {
                min_distance = distance;
                p[i].c = j;
            }
        }
        min_distance = 1.1;
    }
}



bool centros()
{
    bool flag=true;
    int i, acum[K];
    float cx[K], cy[K];
    for(i=0; i<K; i++) 
    {
        cx[i] = 0;
        cy[i] = 0;
        acum[i] = 0;
    }
    
    for(i=0; i<N; i+=2) 
    {
        cx[p[i].c] += p[i].x;
        cx[p[i+1].c]+= p[i+1].x;
        cy[p[i].c] += p[i].y;
        cy[p[i+1].c] +=p[i+1].y;
        acum[p[i].c]++;
        acum[p[i+1].c]++;
    }
    for (i=0; i<K; i++) 
    {
        cx[i] = cx[i] / acum[i];
        cy[i] = cy[i] / acum[i];
        clus[i].x = cx[i];
        clus[i].y = cy[i];
        clus[i].c=acum[i];
        if (clus[i].x != clus_ant[i].x || clus[i].y != clus_ant[i].y) flag=false;
    }
    return flag;
    
}

void imprime()
{
    printf("\nN = %d, K = %d", N, K);
    for (int i = 0; i < K; i++)
    {
        printf("\nCenter: (%.3f, %.3f) : Size: %d", clus[i].x, clus[i].y, clus[i].c);
    }
}

int main(int argc, char **argv)
{
    int i;
    int iteracoes = 0;
    bool flag=false;
    N = atoi(argv[1]);
    K = atoi(argv[2]);
    T = atoi(argv[3]);
    
    omp_set_num_threads(T);
    inicializa();
    
    for (i = 0; i < K; i++) clus_ant[i].x = clus_ant[i].y = 1;
    
    do
    {
        store();
        assign();
        flag = centros();
        iteracoes ++;
    } while (iteracoes < 21 && flag == false);

    imprime();

    printf("\nIterations: %d\n", (iteracoes - 1));
    return 0;
}