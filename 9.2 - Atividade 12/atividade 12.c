#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX 100

float *gerar_vetor(int n);
float calcular_media(float *vetor, int n);
float calcular_desvio_padrao(float *vetor, int n, float media);

int main() {

    // Iniciando MPI
    MPI_Init(NULL, NULL);
    int nprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    
    time_t t;
    srand(time(NULL) * rank);
    
    int n = MAX / nprocs; // Tamanho do subvetor em cada processo
    float *vetor;
    
    if (rank == 0) {
        vetor = gerar_vetor(MAX);
        // Imprime o vetor gerado
        printf("Vetor gerado:\n");
        for (int i = 0; i < MAX; i++) {
            printf("%.2f ", vetor[i]);
        }
        printf("\n\n");
    }
    
    // Scatter do vetor para todos os processos
    float *subvetor = (float *)malloc(sizeof(float) * n);
    MPI_Scatter(vetor, n, MPI_FLOAT, subvetor, n, MPI_FLOAT, 0, MPI_COMM_WORLD);
    
    // Cálculo da média local em cada processo
    float media_local = calcular_media(subvetor, n);
    
    // Cálculo da média global utilizando MPI_Allreduce
    float media_global;
    MPI_Allreduce(&media_local, &media_global, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
    media_global /= nprocs;
    
    // Cálculo do desvio padrão local em cada processo
    float desvio_padrao_local = calcular_desvio_padrao(subvetor, n, media_global);
    
    // Cálculo do desvio padrão global utilizando MPI_Reduce
    float desvio_padrao_global;
    MPI_Reduce(&desvio_padrao_local, &desvio_padrao_global, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        // Imprime o desvio padrão global
        desvio_padrao_global = sqrt(desvio_padrao_global / MAX);
        printf("Desvio padrão: %.2f\n", desvio_padrao_global);
    }
    
    MPI_Finalize();
    return 0;
}

float *gerar_vetor(int n) {
    float *vetor;
    int i;
    vetor = (float *)malloc(sizeof(float) * n);
    for (i = 0; i < n; i++) {
        float num = (rand() / (float)RAND_MAX);
        vetor[i] = num;
    }
    return vetor;
}

float calcular_media(float *vetor, int n) {
    float soma = 0.0;
    for (int i = 0; i < n; i++) {
        soma += vetor[i];
    }
    return soma / n;
}

float calcular_desvio_padrao(float *vetor, int n, float media) {
    float soma = 0.0;
    for (int i = 0; i < n; i++) {
        soma += pow(vetor[i] - media, 2);
    }
    return soma;
}