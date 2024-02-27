#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define MAX 1000000000

int *gerar_vetor_inteiro(int n);
double execucao_serial(int num, int *vetor, int busca);
double execucao_paralela_wcritical(int num, int num_threads, int *vetor, int busca);
double execucao_paralela_dynamic_for(int num, int num_threads, int *vetor, int busca);
double execucao_paralela_static_for(int num, int num_threads, int *vetor, int busca);
void print_metrics(double tempo_serial, double tempo_2, int num_threads);

int main()
{
    srand(time(NULL));
    int num_threads = omp_get_num_procs() / 2;
    printf("Numero de threads: %d\n", num_threads);

    printf("Informe o tamanho do vetor:\n---> ");
    int vet_size;
    scanf("%d", &vet_size);

    printf("Informe o inteiro a ser buscado:\n---> ");
    int busca;
    scanf("%d", &busca);

    int *vetor = gerar_vetor_inteiro(vet_size);

    double tempo_serial = execucao_serial(vet_size, vetor, busca);
    double tempo_critical = execucao_paralela_wcritical(vet_size, num_threads, vetor, busca);
    double tempo_dynamic_for = execucao_paralela_dynamic_for(vet_size, num_threads, vetor, busca);
    double tempo_static_for = execucao_paralela_static_for(vet_size, num_threads, vetor, busca);

    printf("Tempo da execucao serial: %f\n\n", tempo_serial);

    printf("Execucao com critical:\n");
    print_metrics(tempo_serial, tempo_critical, num_threads);

    printf("Execucao dynamic for:\n");
    print_metrics(tempo_serial, tempo_dynamic_for, num_threads);

    printf("Execucao static for:\n");
    print_metrics(tempo_serial, tempo_static_for, num_threads);

    return 0;
}

double execucao_serial(int num, int *vetor, int busca){
    double inicio = omp_get_wtime();
    int cont = 0;

    for (int i = 0; i < num; i++){

        if (vetor[i] == busca){
            cont++;
        }
    }

    double fim = omp_get_wtime();

    printf("O numero apareceu no vetor %d vezes(execução serial)\n", cont);
    return fim - inicio;
}

double execucao_paralela_wcritical(int num, int num_threads, int *vetor, int busca){
    double inicio = omp_get_wtime();
    int cont = 0;
    #pragma omp parallel num_threads(num_threads)
    {
        int cont_local = 0;

        #pragma omp for
            for (int i = 0; i < num; i++){
                if (vetor[i] == busca){
                    cont_local++;
                }
            }

        #pragma omp critical
            cont += cont_local;
    }

    double fim = omp_get_wtime();

    printf("O numero apareceu no vetor %d vezes(execução paralela com critical)\n", cont);
    return fim - inicio;
}

double execucao_paralela_dynamic_for(int num, int num_threads, int *vetor, int busca){
    double inicio = omp_get_wtime();
    int cont = 0;
    #pragma omp parallel num_threads(num_threads)
    {
        #pragma omp for schedule(dynamic) reduction(+:cont)
            for (int i = 0; i < num; i++){
                if (vetor[i] == busca){
                    cont++;
                }
            }
    }

    double fim = omp_get_wtime();

    printf("O numero apareceu no vetor %d vezes(execução paralela for dinamico)\n", cont);
    return fim - inicio;
}

double execucao_paralela_static_for(int num, int num_threads, int *vetor, int busca){
    double inicio = omp_get_wtime();
    int cont = 0;
    #pragma omp parallel num_threads(num_threads)
    {
        #pragma omp for schedule(static) reduction(+:cont)
            for (int i = 0; i < num; i++){
                if (vetor[i] == busca){
                    cont++;
                }
            }
    }

    double fim = omp_get_wtime();

    printf("O numero apareceu no vetor %d vezes(execução paralela for statico)\n", cont);
    return fim - inicio;
}

void print_metrics(double tempo_serial, double tempo_2, int num_threads){
    printf("\tTempo = %f\n"
           "\tSpeedUp = %f\n"
           "\tEfficiency = %f\n\n", tempo_2, tempo_serial / tempo_2, (tempo_serial / tempo_2) / num_threads);
}

int *gerar_vetor_inteiro(int n) {
    int *vetor;
    int i;
    vetor = (int *)malloc(sizeof(int) * n);
    for (i=0;i<n;i++) {
        int num = (rand() % MAX);
        vetor[i] = num;
    }
    return vetor;
}
