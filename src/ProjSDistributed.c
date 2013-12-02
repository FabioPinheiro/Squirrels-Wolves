#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "ProjSerial.h"
#include "Mover.h"
#include <string.h>
#define DIM 2
int  wolfBP = 0, sqrlBP = 0, wolfStarvP = 0, genNum = 0;


void mapping(){
/* */
}

int main (int argc, char *argv[]) {

	int teste, worldsize=0;
	FILE * inputFile;
	int teste, i;
	int ret = 3, x, y;
	char chr;
	double elapsed_time, game_time;
	MPI_Status status;
	MPI_Comm cart_comm;
	int id, p, rank; /*id geral, p numb PRocessors, rank checkerboard*/
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &id);/*id dos Processos*/
	MPI_Comm_size (MPI_COMM_WORLD, &p); /*numero de processos*/
	int size[DIM], periods[DIM], coords[DIM], auxMap[DIM], divideX, divideY;

	/*algoritmo
	 * Ler num Linhas,
	 * fazer divisao cartesiana
	 * Atribuir parte a processadores para trabalharem. (static load balancing) static num of tasks
	 * TODO Map (deadline 23h59 2/12/13)
	 * TODO Send (Send matrix dimensions, Send the Data) (deadline 23h59 2/12/13)
	 * TODO create Row Wise (Use split?) (deadline 20h00 3/12/13)
	 * TODO ghosts lines (test with 1/2/3/4 ?) (deadline 23h59 3/12/13)
	 * TODO checkerboard (test then apply ghosts lines) (deadline 23h59 4/12/13)
	 * TODO Add Parallel (apply to checkerboard+ghosts lines ) (deadline 23h59 5/12/13)
	 * */

	/*Distribution dynamically*/
	size[0]=size[1]=0;
	/* CheckBoard decomposition creation
	 * Use MPI_DIMS_CREATE(int nodes, int ndims,int *dims);  http://www.mpi-forum.org/docs/mpi-1.1/mpi-11-html/node134.html
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
	MPI_Cart_create (MPI_COMM_WORLD, 2, size, periods, 1, &cart_comm);


	MPI_Comm_rank (cart_comm, &rank);/*get id after dividing*/
	MPI_Cart_coords(cart_comm, rank/*we should not use the ID here*/, 2, coords); /* Descobre as coordenadas do Processo*/
	printf("Process ID: %d   Process coordinates %d, %d   Process Rank %d   \n", id, coords[0], coords[1], rank);
	fflush(stdout);
	/*TODO Vale a pena fazer Split?*/

	/*MPI Cart rank() given coordinates of a process in Cartesian communicator,
	 *  returns process'rank*/

	/*MPI Cart coords()	given rank of a process in Cartesian communicator,
	 * returns process'coordinates*/

	/*
	 * wolfBP = atoi(argv[2]);
	 * sqrlBP = atoi(argv[3]);
	 * wolfStarvP = atoi(argv[4]);
	 * genNum = atoi(argv[5]);
	 * inputFile = fopen(argv[1], "r");
	 * */

	/*			TIME		*/
    MPI_Barrier (MPI_COMM_WORLD);
    elapsed_time = - MPI_Wtime();
    /*			TIME		*/


	/*Read Arguments*/
    if(argc != 6){
	if (!id) printf ("Command line: %s <inputFile, wolfBP, sqrlBP, wolfStarvP,IterationsNum >\n", argv[0]);
	MPI_Finalize();worldsize
	exit (1);
    }
    wolfBP = atoi(argv[2]);
    sqrlBP = atoi(argv[3]);
    wolfStarvP = atoi(argv[4]);
    genNum = atoi(argv[5]);



    /*Master Read The file*/
    if(id == 0){
		inputFile = fopen(argv[1], "r");
		teste = fscanf(inputFile, "%d", &worldsize);
		if(teste != 1){
			printf("Input error!\n");
			exit(-1);
		}
    }

	/*Mapping*/
    if(id == 0){
    	/*Arranja coordenadas da disposiçao do Mundo*/
    	MPI_Cart_coords(cart_comm, p, DIM,auxMap);
    	divideX = auxMap[0] + 1;/*TODO check This!*/
    	divideY = auxMap[1] + 1;
    	/*Check floor and ceiling*/
    	/*0 vai de 0 até N/x e 0 até N/Y*/
    	/*1 vao de N/x até 2N/X e N/Y até 2N/Y*/
    	/*P vai de PN/X N e de PN/Y ate N*/

    }
	/*Read & distribute the Matrix*/
    if(id == 0){
    	while(ret != 3){
    		for(;;){
    			/*guarda num buffer*/
    			if(/*Testa limites da coisa*/){
    				rank++;
    				/*Envia o buffer*/
    			}
    		}
    	}
    }
    else{
    	/*Le com a tag certa e afins*/

    }
    /*So o 0 Le
     * LE para o 0;
     * Le e guarda num buffer para o 1;
     * Ve quando passa a divisão;
     * envia par ao 1;
     * faz rank++
     * Repeat
     * */
    /*		GAME TIME		*/
    game_time = - MPI_Wtime();
    /*Run game*/
    game_time += MPI_Wtime();
    /*		GAME TIME		*/

	/*Ending the Program*/
	elapsed_time += MPI_Wtime(); /*Calcula o tempo*/
	MPI_Finalize();
	return 0;
}
