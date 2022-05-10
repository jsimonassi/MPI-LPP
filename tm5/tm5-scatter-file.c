#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv){
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    int array_size;
    int nSearch;
    FILE *file;


    if (size % 2 != 0){
        printf("O número de processos deve ser par!\n");
        return 0;
    }

    // Se for processo 0, deve alocar/receber o array e o número de elementos da busca
    if (rank == 0){

        file = fopen("tm5arq.txt", "r");

        int i = 0;
        int j = 0;
        fscanf (file, "%d", &array_size);
        fscanf (file, "%d", &nSearch);

        printf("Array size: %d - nSearch: %d \n", array_size, nSearch);

        int buffer[array_size];

        while (!feof (file)){
            fscanf (file, "%d", &i);
            buffer[j] = i;
            printf("Adicionando: %d \n", buffer[j]);
            j++;
        }
        fclose (file);
        

        int acc = array_size / (size);
        MPI_Bcast(&nSearch, 1, MPI_INT, 0, MPI_COMM_WORLD); //Envia número chave para busca
        MPI_Bcast(&acc, 1, MPI_INT, 0, MPI_COMM_WORLD); //Envia tamanho do array para o processo n
        int recvbuf[acc];
        MPI_Scatter(&buffer, acc, MPI_INT, &recvbuf, acc, MPI_INT, rank, MPI_COMM_WORLD);  //Separa e envia array para o processo n

        for (int i = 0; i < acc; ++i){
            if (recvbuf[i] == nSearch){
                printf("Encontrei %d no processo: %d \n", nSearch, rank);
            }
        }
    }

    else if (rank != 0){
        int acc;
        MPI_Bcast(&nSearch, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&acc, 1, MPI_INT, 0, MPI_COMM_WORLD);
        int recvbuf[acc];
        int buffer[acc];
        MPI_Scatter(&buffer, acc, MPI_INT, &recvbuf, acc, MPI_INT, 0, MPI_COMM_WORLD);  //Separa e envia array para o processo n

        for (int i = 0; i < acc; ++i){
            if (recvbuf[i] == nSearch){
                printf("Encontrei %d no processo: %d \n", nSearch, rank);
            }
        }
    }

    MPI_Finalize();
    return 0;
}