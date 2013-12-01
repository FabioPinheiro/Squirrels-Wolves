#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int argc, char *argv[]) {
	double elapsed_time;
	MPI_Status status;
	int id, p;
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &id);/*id dos Processos*/
	MPI_Comm_size (MPI_COMM_WORLD, &p); /*numero de processos*/


	elapsed_time += MPI_Wtime();
	MPI_Finalize();
	return 0;
}
