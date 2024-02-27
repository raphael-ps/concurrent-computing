#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "atividade 5.h"


int encontrarMaiorCritical(int *vetor, int tamanho, double *tempo) {
    int i, maior = vetor[0];
    double inicio = omp_get_wtime();

    #pragma omp parallel for private(i) reduction(max:maior)
    for (i = 1; i < tamanho; i++) {
        if (vetor[i] > maior) {
            maior = vetor[i];
        }
    }
    double fim = omp_get_wtime();

    *tempo =  fim - inicio;
    return maior;
}

int encontrarMaiorSerial(int *vetor, int tamanho, double *tempo){
    int i, maior = vetor[0];
    double inicio = omp_get_wtime();


    for (i = 1; i < tamanho; i++) {
        if (vetor[i] > maior) {
            maior = vetor[i];
        }
    }

    double fim = omp_get_wtime();

    *tempo =  fim - inicio;
    return maior;
}
