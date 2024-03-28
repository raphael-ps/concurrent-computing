#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int *gerar_vetor_binario(int n) {
    int *vetor;
    int i;
    vetor = (int *)malloc(sizeof(int) * n);
    for (i = 0; i < n; i++) {
        int num = rand() % 2;
        vetor[i] = num;
    }
    return vetor;
}

int main(int argc, char** argv) {
    int rank, size;
    int *vetor, *local_vetor;
    int n = 100; // Tamanho do vetor binário
    int local_count = 0, global_count = 0;
    int parity_bit = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Dividir o vetor binário entre os processos
    int local_n = n / size;
    vetor = gerar_vetor_binario(n);
    local_vetor = (int *)malloc(sizeof(int) * local_n);

    MPI_Scatter(vetor, local_n, MPI_INT, local_vetor, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // Contar o número de 1's localmente
    for (int i = 0; i < local_n; i++) {
        if (local_vetor[i] == 1) {
            local_count++;
        }
    }

    // Somar os contadores locais para obter o total global
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // O processo raiz decide o bit de paridade
    if (rank == 0) {
        if (global_count % 2 != 0) {
            parity_bit = 1;
        }
    }

    // Transmitir o bit de paridade para todos os processos
    MPI_Bcast(&parity_bit, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Adicionar o bit de paridade ao vetor binário local
    for (int i = 0; i < local_n; i++) {
        local_vetor[i] = parity_bit;
    }

    // Juntar os vetores locais para formar o vetor binário final
    MPI_Gather(local_vetor, local_n, MPI_INT, vetor, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // O processo raiz imprime o vetor binário final
    if (rank == 0) {
        printf("Vetor binário final:\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", vetor[i]);
        }
        printf("\n");
    }

    free(vetor);
    free(local_vetor);

    MPI_Finalize();

    return 0;
}