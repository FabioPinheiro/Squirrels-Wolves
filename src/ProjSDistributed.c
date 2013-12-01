#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int argc, char *argv[]) {
	double elapsed_time;
	MPI_Status status;
	MPI_Comm cart_comm;
	int id, p;
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &id);/*id dos Processos*/
	MPI_Comm_size (MPI_COMM_WORLD, &p); /*numero de processos*/
	int size[2], periods[2], coords[2];

	/*Distribution dynamically*/
	size[0]=size[1]=0;
	/* CheckBoard decomposition creation
	 * Use MPI_DIMS_CREATE(int nodes, int dims,int *size);  http://www.mpi-forum.org/docs/mpi-1.1/mpi-11-html/node134.html
	 *  MPI_CART_CREATE(MPI_Comm old_comm In - old communicator ,int dims In - grid dimensions,int *size In - # procs in each dim,
	 *  int *periodic  In - 1 if dim i wraps around , int reorder In - 1 if process ranks can be reordered;0 otherwise ,
	 *  MPI_Comm *cart_comm  Out - new communicator );
	 *  -> creates a comunicator  http://www.open-mpi.org/doc/v1.4/man3/MPI_Cart_create.3.php
	 * */


	/*p processors, 2 dimensions (2D), size=distribution of Processors 0 for dinamicaly*/
	MPI_Dims_create(p,2, size);


	/*Cria canal de comunicação*/
	/*Nota [Periodos] as Col e as Row podem ser periodicas, ou seja aceder ao -1 significa o mesmo
	 * 	que aceder a 1 (no array) AKA wrapped, uma especie do que se pode fazer com os ecrans dos telemoveis.*/
	periods[0]=periods[1]=0; /*Não ha periodos*/

	/*o 1 é dar premissoes para que reordene os processos para ser mais eficiente*/
	MPI_Cart_create (MPI_COMM_WORLD, 2, size, periodic, 1, &cart_comm);
	MPI_Cart_coords(cart_comm, id, 2, coords); /* Descobre as coordenadas do Processo*/


	/*MPI Cart rank() given coordinates of a process in Cartesian communicator,
	 *  returns process'rank*/

	/*MPI Cart coords()	given rank of a process in Cartesian communicator,
	 * returns process'coordinates*/


	/*Ending the Program*/
	elapsed_time += MPI_Wtime(); /*Calcula o tempo*/
	MPI_Finalize();
	return 0;
}
