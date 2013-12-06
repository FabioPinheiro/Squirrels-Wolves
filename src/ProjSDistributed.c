#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "headersMPI.h"
#include "MoverMPI.h"
#include <string.h>
#define DIM 2
#define TAG_STARTUP 9780
#define TAG_CHANGE 6548
#define TAG_END 5648
#define GHOST_NUM 2

int wolfBP = 0, sqrlBP = 0, wolfStarvP = 0, genNum = 0;

char printValues(int x){
	/*
	 * Recives an int that is the internal representation of the types and returns char that is the
	 * external representation of the same type
	 */
	char str[2];
	sprintf(str, "%d", x);
	switch(atoi(str)){
	case WOLF:
		return 'w';
	case WES:
		return 'w';
	case SQRL:
		return 's';
	case ICE:
		return 'i';
	case TREE:
		return 't';
	case SONT:
		return '$';
	case EPTY:
		return '-';
	default:
		return ' ';
	}
	return ' ';
}

void sworldTreeIceCpy(sworld worldCpyTo, sworld worldCpyFrom, int xSize, int ySize){
	int i;
	for(i = 0; i < xSize * ySize; i++){
		if(worldCpyFrom[i].type == TREE){
			worldCpyTo[i].type = TREE;
			worldCpyTo[i].x = worldCpyFrom[i].x;
			worldCpyTo[i].y = worldCpyFrom[i].y;
		}
		if(worldCpyFrom[i].type == ICE){
			worldCpyTo[i].type = ICE;
			worldCpyTo[i].x = worldCpyFrom[i].x;
			worldCpyTo[i].y = worldCpyFrom[i].y;
		}
	}
}

void printMatrixOutFile(sworld world, char* name, int xSize, int ySize, int realShiftSize){ /*output para Avaliacao*/
	FILE *out;
	int i, aux = (genNum == 1 || realShiftSize == 0? 0: 1);
	out = fopen(name, "a");
	//TODO
	for(i=0;i< xSize*ySize;i++){
		if(world[i].type > EPTY	&& world[i].type <= SONT)
			if(isAnimal(world[i].type)){
				//printf("world[i].x %d  world[i].y %d world[i].type %c\n", world[i].x, world[i].y, printValues(world[i].type));
				fprintf(out, "%d %d %c\n",world[i].x+((realShiftSize-GHOST_NUM)*aux) , world[i].y, printValues(world[i].type));
			}
			else{
				//printf("world[i].x %d  world[i].y %d world[i].type %c\n", world[i].x, world[i].y, printValues(world[i].type));
				fprintf(out, "%d %d %c\n",world[i].x+((realShiftSize-GHOST_NUM)*aux) , world[i].y, printValues(world[i].type));
			}
	}


	/*for(i = 0; i < worldsize; i++){
		for(j = 0; j < worldsize; j++){
			if(world[i + j * worldsize].type > EPTY
					&& world[i + j * worldsize].type <= SONT)
				fprintf(out, "%d %d %c\n", j, i,
						printValues(world[i + j * worldsize].type));
		}
	}*/
	fclose(out);
}

void computeSize(int numP, int worldSize, int pId, int *result){
	*result = (int) worldSize/numP;
	if(worldSize%numP - pId > 0){
		*result += 1;
	}
}

int addSpecial(char string){
	switch(string){
	case 'w':
		return WOLF;
	case 's':
		return SQRL;
	case 'i':
		return ICE;
	case 't':
		return TREE;
	}
	return EPTY;
}

void exchangeGhostLines(int rank, int p, MPI_Comm cart_comm,  MPI_Datatype worldType, sworld my_world2, int worldSize){
	//XXX worldtype hacked neste momento ano funca para checkerboard
	 MPI_Status status;
	 MPI_Request req;
	 int computedsize;
	 computeSize(p, worldSize, rank, &computedsize);
	if(rank==0){
		MPI_Irecv((my_world2+computedsize*worldSize), GHOST_NUM*worldSize, worldType, 1, TAG_CHANGE, cart_comm, &req);
		MPI_Send((my_world2+computedsize*worldSize-GHOST_NUM*worldSize), GHOST_NUM*worldSize, worldType, 1, TAG_CHANGE, cart_comm);
		MPI_Wait(&req, &status);

	}
	else{
		if(rank == p-1){
			MPI_Irecv(my_world2, GHOST_NUM*worldSize, worldType, rank-1, TAG_CHANGE, cart_comm, &req);
			MPI_Send(my_world2+GHOST_NUM*worldSize, GHOST_NUM*worldSize, worldType, rank-1, TAG_CHANGE, cart_comm);
			MPI_Wait(&req, &status);
		}
		else{
			/*Recebe cima*/
			MPI_Irecv(my_world2, GHOST_NUM*worldSize, worldType, rank-1, TAG_CHANGE, cart_comm, &req);
			MPI_Send(my_world2+GHOST_NUM*worldSize, GHOST_NUM*worldSize, worldType, rank-1, TAG_CHANGE, cart_comm);
			MPI_Wait(&req, &status);//xxx check
			/*Recebe baixo*/
			MPI_Irecv((my_world2+computedsize*worldSize), GHOST_NUM*worldSize, worldType, rank+1, TAG_CHANGE, cart_comm, &req);
			MPI_Send((my_world2+computedsize*worldSize-GHOST_NUM*worldSize), GHOST_NUM*worldSize, worldType, rank+1, TAG_CHANGE, cart_comm);
			MPI_Wait(&req, &status);//xxx check
		}
	}
}


void setType(sworld my_world, int x_cord, int y_cord, char chr, int xVirt, int yVirt, int worldsize){
	//XXX check if it is correct
	int type;
	if(x_cord > worldsize - 1 || y_cord > worldsize - 1 || x_cord < 0
			|| y_cord < 0){
		printf("Invalid Input!\n");
		exit(2);
	}

	type = addSpecial(chr);
	my_world[calcPos(xVirt, yVirt, worldsize)].x = xVirt;
	my_world[calcPos(xVirt, yVirt, worldsize)].y = y_cord;
	my_world[calcPos(xVirt, yVirt, worldsize)].type = type;
	switch(type){
	case WOLF:
		my_world[calcPos(xVirt, yVirt, worldsize)].breeding_period = wolfBP;
		my_world[calcPos(xVirt, yVirt, worldsize)].starvation_period =
				wolfStarvP;
		break;
	case SQRL:
		my_world[calcPos(xVirt, yVirt, worldsize)].breeding_period = sqrlBP;
		break;
	default:
		break;
	}

}



void processReds(sworld worldRead, sworld worldWrite, int xSize, int ySize){
	int l, index;
	//#pragma omp parallel for private(index)
	for(l = 0; l < xSize*ySize; l += 2 * ySize){
		for(index = l; index < l + ySize; index += 2){
			if(isAnimal(worldRead[index].type)){
				goAnimal(worldRead, worldWrite, index, worldRead[index].type, xSize ,ySize);
			}
		}
	  
		if(l + 2 * ySize <= xSize*ySize){ /*a matiz tem o tamanho de lado impar e esta o ultimo congunto*/
			for(index = 1 + l + ySize; index < l + 2 * ySize; index += 2){
				if(isAnimal(worldRead[index].type)){
					goAnimal(worldRead, worldWrite, index, worldRead[index].type, xSize ,ySize);
				}
			}
		}
	}
}

void processBlacks(sworld worldRead, sworld worldWrite, int xSize, int ySize){
	int l, index;
	//#pragma omp parallel for private(index)
	for(l = 0; l < xSize*ySize; l += 2 * ySize){
		for(index = 1 + l; index < l + ySize; index += 2){
			if(isAnimal(worldRead[index].type)){
				goAnimal(worldRead, worldWrite, index, worldRead[index].type, xSize ,ySize);
			}
		}

		if(l + 2 * ySize <= xSize*ySize){ /*a matiz tem o tamanho de lado impar e esta o ultimo congunto*/
			for(index = l + ySize; index < l + 2 * ySize; index += 2){
				if(isAnimal(worldRead[index].type)){
					goAnimal(worldRead, worldWrite, index, worldRead[index].type, xSize ,ySize);
				}
			}
		}
	}
}

sworld processGen(sworld my_world1, sworld my_world2, int xSize, int ySize, int rank, int p, MPI_Comm cart_comm,  MPI_Datatype worldType){
	
	int i, j, aux;
	sworld my_worldAUX;
	printf("size before %d\n", xSize);


	for(i = 0; i < genNum; i++){
		my_worldAUX = my_world1;
		my_world1 = my_world2;
		my_world2 = my_worldAUX;
		//#pragma omp parallel for
		for(j = 0; j < xSize * ySize; j++){
			if(isAnimal(my_world1[j].type)){
				my_world1[j].breeding_period--;
				if(my_world1[j].type == WOLF){
					my_world1[j].starvation_period--;
					if(my_world1[j].starvation_period == 0){
						my_world1[j].type = EPTY;
						my_world1[j].breeding_period = 0;
						my_world1[j].starvation_period = 0;
					}
				}
				if(my_world1[j].type == WES){
					my_world1[j].type = WOLF;
					my_world1[j].starvation_period = wolfStarvP - 1;
				}
			}
		}
		processReds(my_world1, my_world2, xSize, ySize);
		processBlacks(my_world1, my_world2, xSize, ySize);
		exchangeGhostLines(rank, p, cart_comm, worldType, my_world2, ySize); //faz uma vez a mais... but is that really a big overhead?
		exchangeGhostLines(rank, p, cart_comm, worldType, my_world1, ySize);
	}
	return my_world2;
}


int main(int argc, char *argv[]) {

	int teste, worldsize = 0;
	FILE * inputFile;
	int ret = 3;
	char chr;
	double elapsed_time, game_time;
	MPI_Comm cart_comm;
	int id, p, rank; /*id geral, p numb PRocessors, rank checkerboard*/
	int coords[DIM], auxMap[DIM], divideX, divideY;
	/*Cria canais de comunicação*/
	/*Nota [Periodos] as Col e as Row podem ser periodicas, ou seja aceder ao -1 significa o mesmo que aceder a 1 (no array) AKA wrapped*/
	int periods[2] = { 0, 0 };
	int sizeToAlloc;
	sworld personalWorld1;
	sworld personalWorld2;
	int personalWorldSize;
    int i, j, myrank;
    MPI_Status status;
    MPI_Datatype worldType;

    MPI_Datatype type[5] = { MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT};
    int blocklen[5] = { 1, 1, 1, 1, 1 };
    MPI_Aint disp[5];

    struct world aux;

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


	/* XXX - COMPILA e nao tem Conflitos do GITi!! */
	MPI_Aint str_add, x_add, y_add, type_add, bp_add, sp_add;

	MPI_Get_address(&aux, &str_add);
	MPI_Get_address(&aux.x, &x_add);
	MPI_Get_address(&aux.y, &y_add);
	MPI_Get_address(&aux.type, &type_add);
	MPI_Get_address(&aux.breeding_period, &bp_add);
	MPI_Get_address(&aux.starvation_period, &sp_add);

    disp[0] = x_add - str_add;
    disp[1] = y_add - str_add;
    disp[2] = type_add - str_add;
    disp[3] = bp_add - str_add;
    disp[4] = sp_add - str_add;
    MPI_Type_create_struct(5, blocklen, disp, type, &worldType);
    MPI_Type_commit(&worldType);

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
	/*TODO Rever se compensa fazer isto*/
	MPI_Bcast(&worldsize, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int size[2] = { p, 1};
	//int size[2] = {0, 0};
	//MPI_Dims_create(p, 2, size);
	//printf("[BEFORE DIMS CREATE]\n");
	fflush(stdout);

	//MPI_Dims_create(p, 2, size);
	/*o 1 é dar premissoes para que reordene os processos para ser mais eficiente*/
	//printf("[AFTER]  Processor %d size0 %d size1 %d \n", id, size[0], size[1]);
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
	 * TODO checkerboard (test then apply ghostsAchas que funciona lines) (deadline 12h00 6/12/13)
	 * TODO Add Parallel (apply to checkerboard+ghosts lines ) (deadline 12h00 6/12/13)
	 * */


	/*				READ INPUT FILE AND DISTRIBUT THEN*/
	if(id == 0){
		MPI_Request *req;
		req = (MPI_Request *) malloc(p* sizeof(MPI_Request));
		sworld bufferSend;
		int computedSize, auxBreak=1, acumulatedSize=0;

		/*LE para ele (0) e guarda*/
		int xAux=0, yAux, charAux, lastX;
		computeSize(p,worldsize, 0, &computedSize);
		personalWorldSize = computedSize+GHOST_NUM;
		personalWorld1 = calloc(worldsize * (computedSize + GHOST_NUM), sizeof(struct world)); //add ghost lines
		personalWorld2 = calloc(worldsize * (computedSize + GHOST_NUM), sizeof(struct world)); //add ghost lines

		acumulatedSize += computedSize;
		ret = fscanf(inputFile, "%d %d %c \n", &xAux, &yAux, &charAux);
		lastX = xAux;
		while(xAux < computedSize){
			setType(personalWorld1, xAux, yAux, charAux, xAux, yAux, worldsize); //neste caso o real e o virtual sao os mesmos
			ret = fscanf(inputFile, "%d %d %c \n", &xAux, &yAux, &charAux);

			if(lastX > xAux){
				printf("Input nao está ordenado!");
				MPI_Finalize();
				exit(-1);
			}
			lastX = xAux;
			if (ret != 3){
				/*Chegou ao fim do ficheiro*/
				auxBreak=0;
				break;
			}
		}
		sworldTreeIceCpy(personalWorld2, personalWorld1, personalWorldSize, worldsize);
		//TODO tratar de quando nao começa em 0 nem em 1(apesar de ser bue weird em termos de load balancing...)
		for(i=1; i<p; i++){
			/*Calc size&createBuffer*/
			computeSize(p,worldsize, i, &computedSize);
			acumulatedSize += computedSize;
			//int somaGhost = (id == p-1? 2 : 4);/*XXX Tentativa de enviar GhostLines*/
			int sizeToSend = computedSize*worldsize;
			if(i == p-1){
				sizeToSend = computedSize*worldsize;
				bufferSend = calloc(sizeToSend,sizeof(struct world));//add ghost lines
			}
			else{
				bufferSend = calloc(sizeToSend,sizeof(struct world));//add 2 * ghost lines
			}

			if(auxBreak){
	//			ret = fscanf(inputFile, "%d %d %c \n", &xAux, &yAux, &charAux);

				while(xAux< acumulatedSize){
					setType(bufferSend, xAux, yAux, charAux,xAux-acumulatedSize+computedSize+GHOST_NUM, yAux, worldsize );
					ret = fscanf(inputFile, "%d %d %c \n", &xAux, &yAux, &charAux);
					if (ret != 3){
						/*Chegou ao fim do ficheiro*/
						auxBreak=0;
						break;
					}
				}
				/*envia a Linha*/
				//TODO rebenta aqui
				MPI_Isend( bufferSend, sizeToSend , worldType ,i , TAG_STARTUP ,MPI_COMM_WORLD ,&req[i]);
				//TODO CHECK this -> Do not forget to complete the request!
				MPI_Wait(&req[i], MPI_STATUS_IGNORE);
			}
			else{
				/*envia a Linha(s)*/
				MPI_Isend(bufferSend , sizeToSend , worldType ,i , TAG_STARTUP ,MPI_COMM_WORLD ,&req[i]);
				//TODO CHECK this -> Do not forget to complete the request!
				MPI_Wait(&req[i], MPI_STATUS_IGNORE);
			}
		}
	}
	else{
		/*XXX Checked*/
		MPI_Status status;
		int auxN;
		 // Wait for a message from rank 0 with tag 0
		MPI_Probe(0, TAG_STARTUP, MPI_COMM_WORLD, &status);
		// Find out the number of elements in the message -> size goes to "n"
		MPI_Get_count(&status, worldType, &auxN);
		personalWorldSize = auxN/worldsize; /*aka computedsize xD*/
		// Allocate memory
		//xxx se for no meio tem de alocar 2*GHOST_NUM
		if(rank != p-1){
			personalWorld1 = calloc(auxN+GHOST_NUM*worldsize, sizeof(struct world));
			personalWorld2 = calloc(auxN+GHOST_NUM*worldsize, sizeof(struct world));
		}
		else{
			personalWorld1 = calloc(auxN+2*GHOST_NUM*worldsize, sizeof(struct world));
			personalWorld2 = calloc(auxN+2*GHOST_NUM*worldsize, sizeof(struct world));
		}
		// Receive the message. ignore the status
	//	printf("DEBUG1 !\n");
		MPI_Recv(personalWorld1+GHOST_NUM*worldsize, auxN, worldType, 0, TAG_STARTUP, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		sworldTreeIceCpy(personalWorld2, personalWorld1, personalWorldSize, worldsize);

	}

	/*		GAME TIME		*/
	MPI_Barrier(MPI_COMM_WORLD);
	game_time = -MPI_Wtime();

	/*       RUN game       */
	exchangeGhostLines(rank,p, cart_comm, worldType, personalWorld1, worldsize);
	exchangeGhostLines(rank,p, cart_comm, worldType, personalWorld2, worldsize);
//	printf("DEBUG-Maloc !\n");
	personalWorld1 = processGen(personalWorld2, personalWorld1, personalWorldSize , worldsize, rank, p, cart_comm, worldType); //XXX hack change worldsize in checkerboard

	/*       RUN game       */

	MPI_Barrier(MPI_COMM_WORLD);
	game_time += MPI_Wtime();
	/*		GAME TIME		*/
	printf("Game Time: %lf", game_time);
	//receiving stuff to print
	if(id == 0){
		int receive, computedSize, realShiftSize=0;
		//print personalWorld1;
		computeSize(p,worldsize, 0, &computedSize);
		//printf("computedSize: %d\n", computedSize);
		 printMatrixOutFile(personalWorld1, "Distributed.out", computedSize,worldsize, realShiftSize);

		for(i=1; i<p;i++){
			realShiftSize += computedSize;
			computeSize(p,worldsize, i, &computedSize);

			//no need to clean world because we are receiving, unless dims are different...
			//use computedSize to work it out when printing
			receive = computedSize*worldsize;
			//printf("Computed size %d \n", computedSize);
		//	printf("rank: %d  size: %d  receiveSize: %d\n", rank, computedSize, receive);
			//printf("DEBUG2 !\n");
			MPI_Recv(personalWorld1, receive, worldType, i, TAG_END, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//printf("DEBUG3 !\n");
			printMatrixOutFile(personalWorld1, "Distributed.out", computedSize ,worldsize, realShiftSize);
		}
	}
	else{
		int send, computedSize;
		computeSize(p,worldsize, rank, &computedSize);
		send = computedSize*worldsize;
		//printf("rank: %d  size: %d  sendSize: %d\n", rank, computedSize, send);
		//printf("BEFORE 1 SEND\n");
		MPI_Send((personalWorld1+GHOST_NUM*worldsize), send, worldType, 0, TAG_END, MPI_COMM_WORLD); //XXX THEM HACKS

	}

	/* Print Matrix while receiving from Processes */



	/*		END GAME		*/


	/*Ending the Program*/
	MPI_Barrier(MPI_COMM_WORLD);
	elapsed_time += MPI_Wtime(); /*Calcula o tempo*/
	MPI_Finalize();
	return 0;
}
