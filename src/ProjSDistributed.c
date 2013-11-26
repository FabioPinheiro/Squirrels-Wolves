#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int argc, char *argv[]) {
	 MPI_Status status;
	 int id, p;

	 MPI_Init (&argc, &argv);
	 MPI_Comm_rank (MPI_COMM_WORLD, &id); /*id dos Processos*/
	 MPI_Comm_size (MPI_COMM_WORLD, &p); /*numero de processos*/


	return 0;
}
