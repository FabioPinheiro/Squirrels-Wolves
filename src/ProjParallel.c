#define MAIN_FILE
#include "ProjSerial.h"
#include <stdio.h>
#include <stdlib.h>
#include "Mover.h"
#include <omp.h>
#include <string.h>

#define DEBUG 0

#define printInt(i) printf("%d\n",i)
#define calcPos(x, y, worldsize) y + x*worldsize

int worldsize = 0, wolfBP = 0, sqrlBP = 0, wolfStarvP = 0, genNum = 0;
int threads = 0;
void debug(char * str){
	if(DEBUG){
		printf("%s", str);
	}
}

void sworldTreeIceCpy(sworld worldCpyTo, sworld worldCpyFrom, int worldsize){
	int i;
	for(i = 0; i < worldsize * worldsize; i++){
		if(worldCpyFrom[i].type == TREE)
			worldCpyTo[i].type = TREE;
		if(worldCpyFrom[i].type == ICE)
			worldCpyTo[i].type = ICE;
	}
}

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

void printMatrix(sworld world){ /*print para teste*/
	int i, j;
	for(i = 0; i < worldsize; i++){
		for(j = 0; j < worldsize; j++){
			printf("%c ", printValues(world[i + j * worldsize].type));
		}
		printf("\n");
	}
}

void printMatrixOutPut(sworld world){ /*output para Avaliacao*/
	int i, j;
	for(i = 0; i < worldsize; i++){
		for(j = 0; j < worldsize; j++){
			if(world[i + j * worldsize].type != EPTY)
				printf("%d %d %c\n", i, j,
						printValues(world[i + j * worldsize].type));
		}
	}
}
void printMatrixOutFile(sworld world, char* name){ /*output para Avaliacao*/
	FILE *out;
	int i, j;
	out = fopen(name, "w");
	for(i = 0; i < worldsize; i++){
		for(j = 0; j < worldsize; j++){
			if(world[i + j * worldsize].type > EPTY
					&& world[i + j * worldsize].type <= SONT)
				fprintf(out, "%d %d %c\n", j, i,
						printValues(world[i + j * worldsize].type));
		}
	}
	fclose(out);
}
void printTimeOutFile(double time){
	FILE *out;
	out = fopen("timeOut.out", "a");
	fprintf(out, "P %f\n", time);
	fclose(out);
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

void processReds(sworld worldRead, sworld worldWrite){
	int l, index;
	#pragma omp parallel for private(index)
	for(l = 0; l < worldsize*worldsize; l += 2 * worldsize){
		for(index = l; index < l + worldsize; index += 2){
			if(isAnimal(worldRead[index].type)){
				goAnimal(worldRead, worldWrite, index, worldRead[index].type);
			}
		}
	  
		if(l + 2 * worldsize <= worldsize*worldsize){ /*a matiz tem o tamanho de lado impar e esta o ultimo congunto*/
			for(index = 1 + l + worldsize; index < l + 2 * worldsize; index += 2){
				if(isAnimal(worldRead[index].type)){
					goAnimal(worldRead, worldWrite, index, worldRead[index].type);
				}
			}
		}
	}
}

void processBlacks(sworld worldRead, sworld worldWrite){
	int l, index;
	#pragma omp parallel for private(index)
	for(l = 0; l < worldsize*worldsize; l += 2 * worldsize){
		for(index = 1 + l; index < l + worldsize; index += 2){
			if(isAnimal(worldRead[index].type)){
				goAnimal(worldRead, worldWrite, index, worldRead[index].type);
			}
		}

		if(l + 2 * worldsize <= worldsize * worldsize){ /*a matiz tem o tamanho de lado impar e esta o ultimo congunto*/
			for(index = l + worldsize; index < l + 2 * worldsize; index += 2){
				if(isAnimal(worldRead[index].type)){
					goAnimal(worldRead, worldWrite, index, worldRead[index].type);
				}
			}
		}
	}
}

sworld processGen(sworld my_world1, sworld my_world2){
	int i, j;
	sworld my_worldAUX;
	for(i = 0; i < genNum; i++){
		my_worldAUX = my_world1;
		my_world1 = my_world2;
		my_world2 = my_worldAUX;
		#pragma omp parallel for
		for(j = 0; j < worldsize * worldsize; j++){
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
		processReds(my_world1, my_world2);
		processBlacks(my_world1, my_world2);
	}
	return my_world2;
}

int main(int argc, char const *argv[]){
	/****************  DECLARATIONS  ********************/
	FILE * inputFile;
	int teste;
	sworld my_world1, my_world2;
	/*	READ FILE VARS */
	int ret = 3, x, y;
	char chr;
	double start, end;

	/*******************  CODE  *************************/
	wolfBP = atoi(argv[2]);
	sqrlBP = atoi(argv[3]);
	wolfStarvP = atoi(argv[4]);
	genNum = atoi(argv[5]);

	inputFile = fopen(argv[1], "r");
	teste = fscanf(inputFile, "%d", &worldsize);
	if(teste != 1){
		printf("Input error!\n");
		exit(-1);
	}
	/*printf(
	 "Tamanho: %d\nwolfBP = %d, sqrlBP = %d, wolfStarvP = %d, genNum = %d\n",
	 worldsize, wolfBP, sqrlBP, wolfStarvP, genNum);*/
	my_world1 =(sworld) calloc(worldsize * worldsize, sizeof(struct world));
	my_world2 =(sworld) calloc(worldsize * worldsize, sizeof(struct world));
	
	/*
	 READ FILE
	 */

	while(1){
		ret = fscanf(inputFile, "%d %d %c \n", &x, &y, &chr);
		if(ret != 3)
			break;
		/*printf("x: %d  y: %d chr: %c\n", x, y, chr);*/
		setType(my_world1, x, y, chr);
	}
	fclose(inputFile);
	/*printf("\n\nTHE WORLD:\n\n");
	 printMatrix(my_world1);
	 printf("\tBefore \n\n\n\n");*/
	sworldTreeIceCpy(my_world2, my_world1, worldsize);
	start = omp_get_wtime();
	my_world1 = processGen(my_world1, my_world2);
	end = omp_get_wtime();

	/*printMatrix(my_world);
	 printf("\tAfter \n\n\n\n");*/
	 printMatrixOutFile(my_world1, "ParallelOut.out");
	printTimeOutFile(end - start);
	/*printf("Parallel DEMOROU:       ->  %f  <-\n", end - start);*/
	printf("End File THREADS: %d:D\n", threads);
	return 0;
}
