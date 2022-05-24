#include <stdio.h>
#include <mpi.h>
#include <math.h>


int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // The initial values, u_i = i^2; v_i = log(i+1)
  float u_i = rank*rank;
  float v_i = log(rank+1.0);

  // Each process computes its intermediate value
  float uv = u_i*v_i;
  float tot;

// Reducing on process 0 :
   MPI_Reduce(&uv, &tot, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

// Validating the result

   if (rank == 0) {        
       printf ("The reduced value is %f on rank 0\n", result );

       // Checking the result
      float validation = 0.0f;
      for (int i=0; i < size; ++i)
        validation += i*i * log(i+1.0f);

       printf ( "Validation gives the value : %f\n ", validation );
  }else
        printf("%f\n", uv);
  
  MPI_Finalize();
  
  return 0;
}