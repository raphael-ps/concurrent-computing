#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "tfinal.h"
#include <stdio.h>
#define MAX 100000

#define SIZE 500000

int *gerar_vetor_inteiro(int n) ;
void gravarVetorEmArquivo(const char *nomeArquivo, int vetor[], int tamanho);
int* lerVetorDeArquivo(const char *nomeArquivo, int tamanho);


int main()
{
    omp_set_num_threads(2);

    int *vetor = lerVetorDeArquivo("vetorSize500000.txt", SIZE);
    double tempo = bubbleSort_mp(vetor, SIZE);
    printf("Tempo = %f\n", tempo);

    return 0;
}
/*

*/
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

void gravarVetorEmArquivo(const char *nomeArquivo, int vetor[], int tamanho) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    for (int i = 0; i < tamanho; i++) {
        fprintf(arquivo, "%d\n", vetor[i]);
    }

    fclose(arquivo);

    printf("Vetor gravado no arquivo.\n");
}

int* lerVetorDeArquivo(const char *nomeArquivo, int tamanho) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para leitura, gerando um novo vetor.\n");
        int *vetor = gerar_vetor_inteiro(SIZE);
        gravarVetorEmArquivo(nomeArquivo, vetor, SIZE);

        return vetor;
    }

    // Descobrindo o tamanho do vetor no arquivo
    int count = 0;
    int valor;
    while (fscanf(arquivo, "%d", &valor) == 1) {
        count++;
    }
    rewind(arquivo);

    // Alocando memória para o vetor
    int *vetor = (int*) malloc(count * sizeof(int));
    if (vetor == NULL) {
        printf("Erro ao alocar memória para o vetor.\n");
        fclose(arquivo);
        return NULL;
    }

    // Lendo os valores do arquivo para o vetor
    for (int i = 0; i < count; i++) {
        fscanf(arquivo, "%d", &vetor[i]);
    }

    fclose(arquivo);
    printf("tamanho original %d == tamanho arquivo %d\n", tamanho, count);
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
