#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

#define qtd_passos 300000000

int main() {

    double passo;
    double x, pi, soma = 0.0;

    double inicio = omp_get_wtime();
    passo = 1.0/qtd_passos;

    for (int i = 0; i < qtd_passos; i++){
        x = (i + 0.5) * passo;
        soma+= 4.0 / (1.0 + x*x);
    }
    pi = passo * soma;
    double fim = omp_get_wtime();
    double tempo_serial = fim-inicio;
    printf("PI(Serial) = %f\n", pi);
    printf("Execucao sequencial(s): %f\n", tempo_serial);

    double x2 = 0, pi2 = 0, soma2 = 0;;
    double passo2;
    double inicio2 = omp_get_wtime();
    passo2 = 1.0/ qtd_passos;

    int qtd_threads = 5;
    #pragma omp parallel private(x2) num_threads(qtd_threads)
    {
        #pragma omp for reduction (+:soma2)
        for (int i = 0; i < qtd_passos; i++){
            x2 = (i + 0.5) * passo2;
            soma2 += 4.0 / (1.0 + x2 * x2);
        }
    }
    pi2 = passo2 * soma2;
    double fim2 = omp_get_wtime();
    double tempo_paralelo = fim2 - inicio2;
    double speedup = tempo_serial / tempo_paralelo;

    printf("PI(Paralelo) = %f\n", pi2);
    printf("Execucao paralela(s): %f\n", tempo_paralelo);
    printf("\n\nSpeedup: %.4f\n", speedup);
	printf("Eficiencia: %.4f\n", speedup / qtd_threads);
    return 0;
}
