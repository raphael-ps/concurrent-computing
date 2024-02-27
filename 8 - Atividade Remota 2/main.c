#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "atividadeRemota2.h"

#define MAX 1000
double *gerar_vetor(int n);
int *gerar_vetor_inteiro(int n) ;
void print_metrics(double tempo_serial, double tempo_2, int num_threads);
void mostrar_vetor(double *v, int tamanho);
void copiar_vetor(int *vetor_origem, int tamanho, int **vetor_destino);

int main()
{
    srand(time(NULL));
    int num_threads = omp_get_num_procs() / 2;
    printf("Numero de threads: %d\n", num_threads);

    int vet_size = 500000000;

    int *vetor = gerar_vetor_inteiro(vet_size);

    int *vet_cpy;
    copiar_vetor(vetor, vet_size, &vet_cpy);

    int *escalares = gerar_vetor_inteiro(vet_size);

    double tempo_serial = 0, tempo_paralelo = 0;

    omp_set_num_threads(num_threads);

    int maior1 = execucaoSerial(vetor, escalares, vet_size, &tempo_serial);

    int maior2 = execucaoParalela(vet_cpy, escalares, vet_size, &tempo_paralelo);

    printf("Resultado (Serial) = %d\nResultado (Paralelo) = %d\n", maior1, maior2);

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

void copiar_vetor(int *vetor_origem, int tamanho, int **vetor_destino) {

  *vetor_destino = malloc(sizeof(int) * tamanho);

  if (*vetor_destino == NULL) {
    printf("Erro ao alocar memória para o vetor destino!\n");
    return;
  }

  for (int i = 0; i < tamanho; i++) {
    (*vetor_destino)[i] = vetor_origem[i];
  }
}
