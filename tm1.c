#include <stdio.h>
#include "mpi.h"

int main(argc, argv)
int argc;
char **argv;
{
    int rank, value = 100, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Processo: %d | Total de processos: %d\n", rank, size);

    //Se o número de processos for impar, abortar execução
    if(size % 2 == 0){
        int rankToSend = 0;
        if(rank < size/2){
            rankToSend = size/2 + rank; 
        }else{
            rankToSend = rank - size/2;
        }
        printf("Processo: %d | Enviando para: %d\n", rank, rankToSend);
        MPI_Send(&value, 1, MPI_INT, rankToSend, status.MPI_TAG, MPI_COMM_WORLD);
        MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("Processo: %d | Enviou seu ID para YY: %d\n\n", rank, rankToSend);
    }
}