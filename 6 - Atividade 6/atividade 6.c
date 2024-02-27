#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "atividade 6.h"


int encontrarIndiceMaiorParalelo(int *vetor, int tamanho, double *tempo) {
    int indice = 0;

    double inicio = omp_get_wtime();

    #pragma omp parallel
    {
        int indice_maior_local = 0;

        for(int i = 1; i < tamanho; i++) {
            if(vetor[indice_maior_local] < vetor[i]) {
                indice_maior_local = i;
            }
        }

        #pragma omp critical
        {
            if (vetor[indice_maior_local] >= vetor[indice]){
                indice = indice_maior_local;
            }
        }
    }


    double fim = omp_get_wtime();

    *tempo =  fim - inicio;
    return indice;
}

int encontrarIndiceMaiorSerial(int *vetor, int tamanho, double *tempo){
    int i, indiceMaior = 0;
    double inicio = omp_get_wtime();


    for (i = 1; i < tamanho; i++) {
        if (vetor[i] > vetor[indiceMaior]) {
            indiceMaior = i;
        }
    }

    double fim = omp_get_wtime();

    *tempo =  fim - inicio;
    return indiceMaior;
}
