#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "atividade11.h"

#define MAX 100
#define TAMANHO 1000

float *gerar_vetor(int n);

int atividade11exe() {

    MPI_Init(NULL, NULL);
    int num_procs;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    time_t seed;
    srand(time(NULL));

    if (proc_rank == 0) {
        float *vec;
        vec = gerar_vetor(TAMANHO);
        float total_sum = 0;
        float total_double_sum = 0;

        MPI_Bcast(vec, TAMANHO, MPI_FLOAT, 0, MPI_COMM_WORLD);
        MPI_Recv(&total_sum, 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&total_double_sum, 1, MPI_FLOAT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Sum: %.10f\n", total_sum);
        printf("Double sum: %.10f\n", total_double_sum);
    }
    else {
        float *vec;
        int i;
        float local_sum = 0;
        vec = (float *)malloc(sizeof(float) * TAMANHO);

        MPI_Bcast(vec, TAMANHO, MPI_FLOAT, 0, MPI_COMM_WORLD);

        if (proc_rank == 1) {
            for (i = 0; i < TAMANHO; i++) {
                local_sum += vec[i];
            }

            MPI_Send(&local_sum, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
        }

        if (proc_rank == 2) {
            for (i = 0; i < TAMANHO; i++) {
                local_sum += (2 * vec[i]);
            }

            MPI_Send(&local_sum, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
        }

        free(vec);
    }

    MPI_Finalize();
    return 0;
}

float *gerar_vetor(int n) {
    float *vec;
    int i;
    vec = (float *)malloc(sizeof(float) * n);
    for (i = 0; i < n; i++) {
        float num = (rand() / (float) RAND_MAX);
        vec[i] = num;
    }
    return vec;
}
