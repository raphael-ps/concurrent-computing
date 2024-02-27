#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils/vectorGenerator.h"
#include "questao1/questao1.h"
#include "questao2/questao2.h"


int main()
{
    srand(time(NULL));

    printf("Questao 1 - Contar ocorrencias de um numero n em um vetor de inteiros. "
           "Fazer de forma sequencial e de forma paralela, e calcular speedUp e eficiencia.\n\n");
    questaoOcorrenciasMain(1000000000, 4);

    printf("\n\nQuestao 2 - Calcular media dos valores em um vetor de floats."
           " Fazer de forma sequencial e paralela, no fim calcular speedUp e eficiencia\n\n");
    questaoMediaMain(1000000000, 4);

    return 0;
}
