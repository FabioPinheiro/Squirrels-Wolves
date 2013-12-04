#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "ProjSerial.h"
#include "Mover.h"
#include <string.h>
#define DIM 2

#define BLOCK_LOW(id,p,n) ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n) (BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id,p,n) (BLOCK_HIGH(id,p,n)-BLOCK_LOW(id,p,n)+1)
#define BLOCK_OWNER(index,p,n) (((p)*((index)+1)-1)/(n))


int wolfBP = 0, sqrlBP = 0, wolfStarvP = 0, genNum = 0;

struct world {
	int x;
	int y;
	int type; /* Wolf, Squirrel, etc. */
 	int breeding_period;
 	int starvation_period;
 };


typedef struct world *sworld;


void computedSize(int numP, int worldSize, int pId, int *result){
	*result = (int) worldSize/numP;
	if(worldSize%numP - pId > 0){
		*result += 1;
	}
}
void setType(sworld my_world, int x_cord, int y_cord, char chr){
	int type;
	if(x_cord > worldsize - 1 || y_cord > worldsize - 1 || x_cord < 0
			|| y_cord < 0){
		printf("Invalid Input!\n");
		exit(2);
	}
	type = addSpecial(chr);
	my_world[calcPos(x_cord, y_cord, worldsize)].type = type;
	switch(type){
	case WOLF:
		my_world[calcPos(x_cord, y_cord, worldsize)].breeding_period = wolfBP;
		my_world[calcPos(x_cord, y_cord, worldsize)].starvation_period =
				wolfStarvP;
		break;
	case SQRL:
		my_world[calcPos(x_cord, y_cord, worldsize)].breeding_period = sqrlBP;
		break;
	default:
		break;
	}

}

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
	int sizeToAlloc, computedSize;
	sworld personalWorld1;
	sworld personalWorld2;
	int personalWorldSize;
    int i, j, myrank;
    MPI_Status status;
    MPI_Datatype worldType;

    MPI_Datatype type[5] = { MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT};
    int blocklen[5] = { 1, 1, 1, 1, 1 };
    MPI_Aint disp[5];


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
	/*TODO change this*/
	MPI_Bcast(&worldsize, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int size[2] = { p, 1};
	printf("[BEFORE DIMS CREATE]\n");
	fflush(stdout);

	//MPI_Dims_create(p, 2, size);
	/*o 1 é dar premissoes para que reordene os processos para ser mais eficiente*/
	printf("[AFTER]  Processor %d size0 %d size1 %d \n", id, size[0], size[1]);
	fflush(stdout);
	MPI_Cart_create(MPI_COMM_WORLD, 2, size, periods, 1, &cart_comm);
//	MPI_Barrier(MPI_COMM_WORLD);

	/*    Testing   */
	MPI_Comm_rank(cart_comm, &rank);/*get id after dividing*/
	MPI_Cart_coords(cart_comm, rank/*we should not use the ID here*/, 2, coords); /* Descobre as coordenadas do Processo*/

	printf(
			"[LAST] Process ID: %d   Process coordinates %d, %d   Process Rank %d WORLDSIZE %d  \n",
			id, coords[0], coords[1], rank, worldsize);
	fflush(stdout);
	/*    Testing   */





	/*algoritmo
	 * Ler num Linhas,
	 * fazer divisao cartesiana
	 * Atribuir parte a processadores para trabalharem. (static load balancing) static num of tasks
	 * TODO programa Working ROW WISE (deadline 23h59 4/12/13)
	 * TODO ghosts lines (test with 1/2/3/4 ?) (deadline 13h00 5/12/13)
	 * TODO checkerboard (test then apply ghostsAchas que funciona lines) (deadline 23h59 5/12/13)
	 * TODO Add Parallel (apply to checkerboard+ghosts lines ) (deadline 23h59 5/12/13)
	 * */
	if(id != 0){
		/*receive tam*/
		MPI_Recv(&sizeToAlloc, 1, MPI_INT, 0, TAG, MPI COMM WORLD, &status); /*Buff, numPos, type, From, TAG, comm*/
		personalWorld1 = calloc(worldsize * sizeToAlloc, sizeof(struct world));
		personalWorld2 = calloc(worldsize * sizeToAlloc, sizeof(struct world));
	}
	else{
		for(i=0;i<p;i++){
			/*ComputeSize*/
			computedSize(p,worldsize, i, &computedSize);
			MPI_Send(&computedSize, 1, MPI_INT, i, TAG, MPI COMM WORLD); /*Buff, numPos, type, To, TAG, comm*/
		}
		/*ID 0 envia tamanho para alocar */
	}

	if(id != 0){
		/*Recebe de 0 o seu tamanho.*/
		MPI_Recv(&personalWorldSize, sizeToAlloc*worldsize, MPI_INT, 0, TAG, MPI COMM WORLD, &status);/*TODO recheck*/
		//sworldTreeIceCpy(my_world2, my_world1, worldsize);/*TODO create new funtion based on Parallel function*/
	}

	/*XXX Tem que estar depois dos mallocs*/
    disp[0] = &personalWorld1[0].x - &personalWorld1[0];
    disp[1] = &personalWorld1[0].y - &personalWorld1[0];
    disp[2] = &personalWorld1[0].type - &personalWorld1[0];
    disp[4] = &personalWorld1[0].breeding_period - &personalWorld1[0];
    disp[5] = &personalWorld1[0].starvation_period - &personalWorld1[0];
    MPI_Type_create_struct(5, blocklen, disp, type, &worldType);
    MPI_Type_commit(&worldType);


	if(id == 0){
		/*READ*/
		int xAux=0, yAux, charAux, auxBreak=1;
		computedSize(p,worldsize, 0, &computedSize);
		ret = fscanf(inputFile, "%d %d %c \n", &xAux, &yAux, &charAux);
		/*XXX Read Input 0 */setType(personalWorld1, xAux, yAux, charAux);
		while(xAux<computedSize){


			ret = fscanf(inputFile, "%d %d %c \n", &xAux, &yAux, &charAux);
			if (ret != 3){
				auxBreak=0; /*Assim nao entra no for pois ja nao ha mais nada para ler*/
				break;
			}
			/*XXX Read Input 0 */setType(personalWorld1, xAux, yAux, charAux);

		}
		if(auxBreak){
			for(i=1; i < p; i++){
				/*TODO Escreve no Buffer de i*/
				computedSize(p,worldsize, 0, &computedSize);
				while(xAux<computedSize){
					/*TODO Escreve no Buffer de i*/
					ret = fscanf(inputFile, "%d %d %c \n", &xAux, &yAux, &charAux);
					if (ret != 3){
						auxBreak=1; /*Assim sai do for pois ja nao ha mais nada para ler*/
						break;
					}
				}

				MPI_Send(&computedSize, 1, MPI_INT, i, TAG, MPI COMM WORLD);/*TODO CORRECT Envia para o ultimo gajo a receber aka envia para o "i"*/
			}
		}
	}



	/*		GAME TIME		*/
	MPI_Barrier(MPI_COMM_WORLD);
	game_time = -MPI_Wtime();
	/*Run game*/

	//my_world1 = processGen(my_world1, my_world2);
	/*TODO for each IT exchange Lines*/


	MPI_Barrier(MPI_COMM_WORLD);
	game_time += MPI_Wtime();
	/*		GAME TIME		*/

	/*		END GAME		*/

	/* Print Matrix while receiving from Processes */



	/*		END GAME		*/


	/*Ending the Program*/
	MPI_Barrier(MPI_COMM_WORLD);
	elapsed_time += MPI_Wtime(); /*Calcula o tempo*/
	MPI_Finalize();
	return 0;
}
