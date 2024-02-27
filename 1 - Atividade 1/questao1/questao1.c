#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "questao1.h"
#include "../utils/vectorGenerator.h"


double execucao_busca_paralela(int *vetor, int vetSize, int qtd_threads, int busca){

    int vezes_encontrado = 0;
    double tempo_paralelo_inicio = omp_get_wtime();
    #pragma omp parallel num_threads(qtd_threads)
    {
        #pragma omp for reduction (+ : vezes_encontrado)
            for(int i = 0; i < vetSize; i++){
                if(vetor[i] == busca){
                    vezes_encontrado++;
                }
            }
    }

    double tempo_paralelo_fim = omp_get_wtime();
    double tempo_paralelo = tempo_paralelo_fim - tempo_paralelo_inicio;

    printf("Tempo de execucao paralela: %f segundos\n", tempo_paralelo);
    printf("O numero %d foi encontrado %d vezes\n", busca, vezes_encontrado);

    return tempo_paralelo;
}

double execucao_busca_serial(int *vetor, int vetSize, int busca){

    int vezes_encontrado = 0;

    double tempo_serial_inicio = omp_get_wtime();
    for(int i = 0; i < vetSize; i++){
        if(vetor[i] == busca){
            vezes_encontrado++;
        }
    }
    double tempo_serial_fim = omp_get_wtime();

    double tempo_serial = tempo_serial_fim - tempo_serial_inicio;
    printf("Tempo de execucao serial: %f segundos\n", tempo_serial);
    printf("O numero %d foi encontrado %d vezes\n", busca, vezes_encontrado);

    return tempo_serial;
}


int questaoOcorrenciasMain(int vetSize, int qtd_threads){

    printf("Informe o inteiro a ser buscado:\n---> ");
    int busca;
    scanf("%d", &busca);

    int *vetor = gerar_vetor_inteiro(vetSize);

    printf("\n");
    double tempo_serial = execucao_busca_serial(vetor, vetSize, busca);
    printf("\n");
    double tempo_paralelo = execucao_busca_paralela(vetor, vetSize, qtd_threads, busca);
    printf("\n");

    double speedUp = tempo_serial / tempo_paralelo;
    double eficiencia = speedUp / qtd_threads;

    printf("SpeedUp = %f\nEficiencia = %f", speedUp, eficiencia);

    return 0;
}
