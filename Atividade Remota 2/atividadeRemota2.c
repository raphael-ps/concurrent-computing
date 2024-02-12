#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include "atividadeRemota2.h"


int encontrarMaiorParalelo2(int *vetor, int tamanho, double *tempo) {
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

double multiplicarEscalarParalelo(int *vetorOriginal, int *vetorEscalares, int vetTam){
    double inicio = omp_get_wtime();

    #pragma omp parallel for
        for(int i = 0; i < vetTam; i++){
            vetorOriginal[i] = vetorOriginal[i] * vetorEscalares[i];
        }

    double fim = omp_get_wtime();

    return fim - inicio;
}

double multiplicarEscalarSerial(int *vetorOriginal, int *vetorEscalares, int vetTam){
    double inicio = omp_get_wtime();

    for(int i = 0; i < vetTam; i++){
        vetorOriginal[i] = vetorOriginal[i] * vetorEscalares[i];
    }

    double fim = omp_get_wtime();

    return fim - inicio;
}

int encontrarMaiorSerial2(int *vetor, int tamanho, double *tempo){
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

int execucaoParalela(int *vetO, int *vetE, int tamVet, double *tempo){
    double tempoMult = 0, tempoBusca = 0;

    tempoMult = multiplicarEscalarParalelo(vetO, vetE, tamVet);
    int maior = encontrarMaiorParalelo2(vetO, tamVet, &tempoBusca);

    *tempo = tempoMult + tempoBusca;

    return maior;
}

int execucaoSerial(int *vetO, int *vetE, int tamVet, double *tempo){
    double tempoMult = 0, tempoBusca = 0;

    tempoMult = multiplicarEscalarSerial(vetO, vetE, tamVet);

    int maior = encontrarMaiorSerial2(vetO, tamVet, &tempoBusca);

    *tempo = tempoMult + tempoBusca;

    return maior;
}
