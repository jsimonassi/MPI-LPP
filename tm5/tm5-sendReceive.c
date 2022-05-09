#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv){
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    int nSearch;
    int array_size = 100;

    if (size % 2 != 0){
        printf("O número de processos deve ser par!\n");
        return 0;
    }

    // Se for processo 0, deve alocar/receber o array e o número de elementos da busca
    if (rank == 0){
        printf("Qual número você quer encontrar no array? \n");
        scanf("%d", &nSearch);

        int buffer[array_size];

        // Inicializando buffer com números aleatórios entre 0 e 50
        for (int i = 0; i < array_size; ++i){
            buffer[i] = (rand() % 51);
        }

        int acc = array_size / (size);
        int lastSend = acc;

        // Começando em 1 para não enviar ao mestre
        for (int i = 1; i < size; i++){

            //Movendo um ponteiro para as divisões do array
            int *arrayCursor = &buffer[lastSend];

            MPI_Send(&nSearch, 1, MPI_INT, i, status.MPI_TAG, MPI_COMM_WORLD);
            MPI_Send(&acc, 1, MPI_INT, i, status.MPI_TAG, MPI_COMM_WORLD);
            MPI_Send(&arrayCursor, acc, MPI_INT, i, status.MPI_TAG, MPI_COMM_WORLD);

            lastSend += acc;
        }

        //Processo 0 também trabalha
        for (int i = 0; i < acc; ++i){
            if (buffer[i] == nSearch){
                printf("Encontrei %d no processo: %d \n", nSearch, rank);
            }
        }
    }

    else if (rank != 0){
        int size;
        MPI_Recv(&nSearch, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&size, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        int array[size];
        MPI_Recv(&array, size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        for (int i = 0; i < size; ++i){
            if (array[i] == nSearch){
                printf("Encontrei %d no processo: %d \n", nSearch, rank);
            }
        }
    }

    MPI_Finalize();
    return 0;
}