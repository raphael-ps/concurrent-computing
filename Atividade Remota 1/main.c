#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "atividadeRemota1.h"

#define MAX 10
double *gerar_vetor(int n);
int *gerar_vetor_inteiro(int n) ;
void print_metrics(double tempo_serial, double tempo_2, int num_threads);
void mostrar_vetor(double *v, int tamanho);


int main()
{
    srand(time(NULL));
    int num_threads = omp_get_num_procs() / 2;
    printf("Numero de threads: %d\n", num_threads);

    int vet_size = 200000000;

    double *vetor = gerar_vetor(vet_size);

    double tempo_serial = 0, tempo_paralelo = 0;

    double desvioSerial = desvioPadraoSerial(vetor, vet_size, &tempo_serial);

    omp_set_num_threads(num_threads);
    double desvioParalelo = desvioPadraoParalelo(vetor, vet_size, &tempo_paralelo);

    printf("Resultado (Serial) = %f\nResultado (Paralelo) = %f\n", desvioSerial, desvioParalelo);

    printf("Execucao Serial:\n");
    print_metrics(tempo_serial, tempo_serial, 1);

    printf("Execucao Paralelo:\n");
    print_metrics(tempo_serial, tempo_paralelo, num_threads);
    return 0;
}

double *gerar_vetor(int n) {
    double *vetor;
    int i;
    vetor = (double *)malloc(sizeof(double) * n);
    for (i=0;i<n;i++) {
        double num = (rand() / (float)RAND_MAX) * (rand() % MAX + 1);
        vetor[i] = num;
    }
    return vetor;
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

void mostrar_vetor_inteiro(int *v,int tamanho) {
    int i;
    printf("[");
    for (i=0;i<tamanho;i++) {
        printf("%d%s", v[i], i == tamanho-1 ? "]\n" : ", ");
    }
    printf("\n");
}

void mostrar_vetor(double *v, int tamanho) {
    int i;
    printf("[");
    for (i=0;i<tamanho;i++) {
        printf("%f%s", v[i], i == tamanho-1 ? "]\n" : ", ");
    }
    printf("\n");
}

void print_metrics(double tempo_serial, double tempo_2, int num_threads){
    printf("\tTempo = %f\n"
           "\tSpeedUp = %f\n"
           "\tEfficiency = %f\n\n", tempo_2, tempo_serial / tempo_2, (tempo_serial / tempo_2) / num_threads);
}
