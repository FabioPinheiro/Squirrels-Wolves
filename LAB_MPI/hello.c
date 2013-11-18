
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int argc, char *argv[]) {

    MPI_Status status;
    int id, p, i, rounds;
    double secs;
	char hostname[1024];
	hostname[1023] = '\0';
	gethostname(hostname, 1023);

    
    MPI_Init (&argc, &argv);

    MPI_Comm_rank (MPI_COMM_WORLD, &id);
    MPI_Comm_size (MPI_COMM_WORLD, &p);
	
	
    MPI_Barrier (MPI_COMM_WORLD);
    secs = - MPI_Wtime();
	
	printf("Process %d sends greetings from machine %s\n", id, hostname);
	
    MPI_Barrier (MPI_COMM_WORLD);
    secs += MPI_Wtime();
return 0;
}
