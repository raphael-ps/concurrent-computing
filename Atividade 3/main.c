#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double execucao_serial(int num){
    double inicio = omp_get_wtime();
    unsigned long int soma = 0;


    for (unsigned int i = 1; i <= num; i++){

        if (num % i == 0){
            soma += i;
        }
    }

    double fim = omp_get_wtime();

    printf("Soma dos divisores desse numero(execução serial): %ld\n", soma);
    return fim - inicio;
}

double execucao_paralela_wcritical(int num, int num_threads){
    double inicio = omp_get_wtime();
    unsigned long int soma = 0;
    #pragma omp parallel num_threads(num_threads)
    {
        unsigned long int soma_local = 0;

        #pragma omp for
            for (unsigned int i = 1; i <= num; i++){
                if (num % i == 0){
                    soma_local += i;
                }
            }

        #pragma omp critical
            soma += soma_local;
    }

    double fim = omp_get_wtime();

    printf("Soma dos divisores desse numero(execução paralela com critical): %ld\n", soma);
    return fim - inicio;
}

double execucao_paralela_dynamic_for(int num, int num_threads){
    double inicio = omp_get_wtime();
    unsigned long int soma = 0;
    #pragma omp parallel num_threads(num_threads)
    {
        #pragma omp for schedule(dynamic) reduction(+:soma)
            for (unsigned int i = 1; i <= num; i++){
                if (num % i == 0){
                    soma += i;
                }
            }
    }

    double fim = omp_get_wtime();

    printf("Soma dos divisores desse numero(execução paralela for dinamico): %ld\n", soma);
    return fim - inicio;
}

double execucao_paralela_static_for(int num, int num_threads){
    double inicio = omp_get_wtime();
    unsigned long int soma = 0;
    #pragma omp parallel num_threads(num_threads)
    {
        #pragma omp for schedule(static) reduction(+:soma)
            for (unsigned int i = 1; i <= num; i++){
                if (num % i == 0){
                    soma += i;
                }
            }
    }

    double fim = omp_get_wtime();

    printf("Soma dos divisores desse numero(execução paralela for statico): %ld\n", soma);
    return fim - inicio;
}

void print_metrics(double tempo_serial, double tempo_2, int num_threads){
    printf("\tSpeedUp = %f\n"
           "\tEfficiency = %f\n\n", tempo_serial / tempo_2, (tempo_serial / tempo_2) / num_threads);
}

int main()
{
    int num = 1000000001;
    int num_threads = 4;

    double tempo_serial = execucao_serial(num);
    double tempo_critical = execucao_paralela_wcritical(num, num_threads);
    double tempo_dynamic_for = execucao_paralela_dynamic_for(num, num_threads);
    double tempo_static_for = execucao_paralela_static_for(num, num_threads);

    printf("Execucao com critical:\n");
    print_metrics(tempo_serial, tempo_critical, num_threads);

    printf("Execucao dynamic for:\n");
    print_metrics(tempo_serial, tempo_dynamic_for, num_threads);

    printf("Execucao static for:\n");
    print_metrics(tempo_serial, tempo_static_for, num_threads);
    return 0;
}
