#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "../utils/vectorGenerator.h"
#include "questao2.h"

double execucao_serial(double *vetor, int tam_vet){

    double somatorio = 0, media = 0;
    double tempo_serial_inicio = omp_get_wtime();

    for (int i = 0; i < tam_vet; i++){
        somatorio += vetor[i];
    }
    media = somatorio / tam_vet;
    double tempo_serial_fim = omp_get_wtime();
    double tempo_serial = tempo_serial_fim - tempo_serial_inicio;

    printf("Somatório: %f\n", somatorio);
    printf("A media aritmetica do vetor eh: %.2f\n", media);
    printf("Media calculada em: %f segundos\n", tempo_serial);

    return tempo_serial;
}

double execucao_paralela(double *vetor, int tam_vet, int qtd_threads){

    double somatorio2 = 0;
    double tempo_paralelo_inicio = omp_get_wtime();
    #pragma omp parallel num_threads(qtd_threads)
    {
        #pragma omp for reduction(+ : somatorio2)
            for (int i = 0; i < tam_vet; i++){
                somatorio2 += vetor[i];
            }
    }
    double media2 = somatorio2 / tam_vet;
    double tempo_paralelo_fim = omp_get_wtime();
    double tempo_paralelo = tempo_paralelo_fim - tempo_paralelo_inicio;

    printf("Somatório: %f\n", somatorio2);
    printf("A media aritmetica do vetor eh: %.2f\n", media2);
    printf("Media calculada em: %f segundos\n", tempo_paralelo);

    return tempo_paralelo;
}

int questaoMediaMain(int vetSize, int qtd_threads){

    double *vetor = gerar_vetor(vetSize);

    /**
    printf("[");
    for (int c = 0; c < vetSize; c++){
        printf("%.2f ", vetor[c]);
    }
    printf("]\n");
    **/

    double tempo_serial = execucao_serial(vetor, vetSize);
    printf("--------------------------\n\n");

    double tempo_paralelo = execucao_paralela(vetor, vetSize, qtd_threads);
    printf("--------------------------\n\n");

    double speedUp = tempo_serial / tempo_paralelo;
    double eficiencia = speedUp / qtd_threads;

    printf("SpeedUp = %f\n", speedUp);
    printf("Eficiencia = %f\n", eficiencia);

    return 0;
}
