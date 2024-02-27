#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include "atividadeRemota1.h"

double desvioPadraoSerial(double *vetor, int vetTam, double *tempoSerial){
    double tInicial = omp_get_wtime();

    long double somatorio = 0;

    for (int i = 0; i < vetTam; i++){
        somatorio += vetor[i];
    }
    double media = somatorio / vetTam;
    long double variancia = 0;
    for (int i = 0; i < vetTam; i++){
        variancia += pow(vetor[i] - media, 2);
    }

    double desvioPadrao = sqrt(variancia/vetTam);

    double tFinal = omp_get_wtime();
    *tempoSerial = tFinal - tInicial;

    return desvioPadrao;
}

double desvioPadraoParalelo(double *vetor, int vetTam, double *tempoParalelo){
    double tInicial = omp_get_wtime();

    long double somatorio = 0.0;
    #pragma omp parallel for reduction(+:somatorio)
        for (int i = 0; i < vetTam; i++){
            somatorio += vetor[i];
        }
    double media = somatorio / vetTam;

    long double variancia = 0.0;
    #pragma omp parallel for reduction(+:variancia)
        for (int i = 0; i< vetTam; i++){
            variancia += pow(vetor[i] - media, 2);
        }

    double desvioPadrao = sqrt(variancia / vetTam);

    double tFinal = omp_get_wtime();
    *tempoParalelo = tFinal - tInicial;

    return desvioPadrao;
}
