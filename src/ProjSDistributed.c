#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "ProjSerial.h"
#include "Mover.h"
#include <string.h>
#define DIM 2
int wolfBP = 0, sqrlBP = 0, wolfStarvP = 0, genNum = 0;


int main(int argc, char *argv[]) {

	int teste, worldsize = 0, i;
	FILE * inputFile;
	int ret = 3;
	char chr;
	double elapsed_time, game_time;
	MPI_Status status;
	MPI_Comm cart_comm;
	int id, p, rank; /*id geral, p numb PRocessors, rank checkerboard*/
	int coords[DIM], auxMap[DIM], divideX, divideY;




	/*Cria canais de comunicação*/
	/*Nota [Periodos] as Col e as Row podem ser periodicas, ou seja aceder ao -1 significa o mesmo que aceder a 1 (no array) AKA wrapped*/
	int periods[2] = { 0, 0 };
	/* Definitions:
	 * wolfBP = atoi(argv[2]);
	 * sqrlBP = atoi(argv[3]);
	 * wolfStarvP = atoi(argv[4]);
	 * genNum = atoi(argv[5]);
	 * inputFile = fopen(argv[1], "r");
	 * */



	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);/*id dos Processos*/
	MPI_Comm_size(MPI_COMM_WORLD, &p); /*numero de processos*/
	int size[2] = { p, 0 };
	/*			TIME		*/
	MPI_Barrier(MPI_COMM_WORLD);
	elapsed_time = -MPI_Wtime();
	/*			TIME		*/

	/*Read Arguments*/
	if (argc != 6) {
		if (!id)
			printf(
					"Command line: %s <inputFile, wolfBP, sqrlBP, wolfStarvP,IterationsNum >\n",
					argv[0]);
		MPI_Finalize();
		exit(1);
	}

	wolfBP = atoi(argv[2]);
	sqrlBP = atoi(argv[3]);
	wolfStarvP = atoi(argv[4]);
	genNum = atoi(argv[5]);

	/*Master Read The file*/
	if (id == 0) {
		inputFile = fopen(argv[1], "r");
		teste = fscanf(inputFile, "%d", &worldsize);
		if (teste != 1) {
			MPI_Finalize();
			printf("Input error!\n");
			exit(-1);
		}
		/*Define o tamanho da matrix para dividir*/
		printf("worldsize %d \n", worldsize);
		//size[0] = size[1] = worldsize;
	}
	/*p pr
	 *
	 * ocessors, 2 dimensions (2D), size=tamanho da matriz*/
	printf("Processors %d size0 %d size1 %d  \n", p, size[0], size[1]);
	fflush(stdout);
		MPI_Dims_create(p, 2, size);


		/*o 1 é dar premissoes para que reordene os processos para ser mais eficiente*/
		printf("Processors %d size0 %d size1 %d", p, size[0], size[1]);
		fflush(stdout);
		MPI_Cart_create(MPI_COMM_WORLD, 2, size, periods, 1, &cart_comm);

		/*    Testing   */
			MPI_Comm_rank(cart_comm, &rank);/*get id after dividing*/
			MPI_Cart_coords(cart_comm, rank/*we should not use the ID here*/, 2,
					coords); /* Descobre as coordenadas do Processo*/


			printf("Process ID: %d   Process coordinates %d, %d   Process Rank %d   \n",
					id, coords[0], coords[1], rank);
			fflush(stdout);

			/*    Testing   */


	/*algoritmo
	 * Ler num Linhas,
	 * fazer divisao cartesiana
	 * Atribuir parte a processadores para trabalharem. (static load balancing) static num of tasks
	 * TODO Send (Send matrix dimensions, Send the Data) (deadline 23h59 2/12/13)
	 * TODO create Row Wise (Use split?) (deadline 20h00 3/12/13)
	 * TODO ghosts lines (test with 1/2/3/4 ?) (deadline 23h59 3/12/13)
	 * TODO checkerboard (test then apply ghosts lines) (deadline 23h59 4/12/13)
	 * TODO Add Parallel (apply to checkerboard+ghosts lines ) (deadline 23h59 5/12/13)
	 * */




//	/*Mapping*/
//	if (id == 0) {
//		/*Arranja coordenadas da disposiçao do Mundo*/
//		MPI_Cart_coords(cart_comm, p, DIM, auxMap);
//		divideX = auxMap[0] + 1; /*TODO check This!*/
//		divideY = auxMap[1] + 1;
//		/*Check floor and ceiling*/
//		/*0 vai de 0 até N/x e 0 até N/Y*/
//		/*1 vao de N/x até 2N/X e N/Y até 2N/Y*/
//		/*P vai de PN/X N e de PN/Y ate N*/
//
//		/*Read & distribute the Matrix*/
//		while (ret != 3) {
//			for (;;) { /*TODO*/
//				/*guarda num buffer*/
//				if (/*Testa limites da coisa*/) {
//					rank++;
//					/*Envia o buffer*/
//				}
//			}
//		}
//	} else {
//		/*Le com a tag certa e afins*/
//
//	}


	/*		GAME TIME		*/
	game_time = -MPI_Wtime();
	/*Run game*/
	game_time += MPI_Wtime();
	/*		GAME TIME		*/

	/*Ending the Program*/
	elapsed_time += MPI_Wtime(); /*Calcula o tempo*/
	MPI_Finalize();
	return 0;
}
