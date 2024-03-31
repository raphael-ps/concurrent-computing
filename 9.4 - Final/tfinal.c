#include <stdio.h>
#include <omp.h>
#include "tfinal.h"


void swap(int *num1, int *num2)
{
	int temp = *num1;
	*num1 =  *num2;
	*num2 = temp;
}

double bubbleSort_serial(int array[], int size) {
    double inicio = omp_get_wtime();

    for (int step = 0; step < size - 1; ++step) {
        for (int i = 0; i < size - step - 1; ++i) {
            if (array[i] > array[i + 1]) {
                int temp = array[i];
                array[i] = array[i + 1];
                array[i + 1] = temp;
            }
        }
    }
    double fim = omp_get_wtime();

    return fim - inicio;
}

void printArray(int array[], int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

double bubbleSort_mp(int array[], int size) {
    double inicio = omp_get_wtime();

    int first;
    for( int i = 0; i < size-1; i++ )
	{
		first = i % 2;
		#pragma omp parallel for
		for( int j = first; j < size-1; j += 1 )
		{
			if( array[ j ] > array[ j+1 ] )
			{
				swap( &array[ j ], &array[ j+1 ] );
			}
		}
	}
	double fim = omp_get_wtime();

    return fim - inicio;
}

