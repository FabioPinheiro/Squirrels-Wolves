#define MAIN_FILE
#include "ProjSerial.h"
#include <stdio.h>
#include <stdlib.h>
#include "Mover.h"
#include <omp.h>

#define DEBUG 0

#define printInt(i) printf("%d\n",i)
#define calcPos(x, y, worldsize) y + x*worldsize

int worldsize = 0, wolfBP = 0, sqrlBP = 0, wolfStarvP = 0, genNum = 0;

void debug(char * str) {
	if (DEBUG) {
		printf("%s", str);
	}
}

char printValues(int x) {
	/*
	 * Recives an int that is the internal representation of the types and returns char that is the
	 * external representation of the same type
	 */
	char str[2];
	sprintf(str, "%d", x);
	/*printf("COISAS: type: %d str: %s  atoi(str): %d\n",x,str,atoi(str));*/
	switch (atoi(str)) {
	case WOLF:
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

void printMatrix(sworld world) { /*print para teste*/
	int i, j;
	for (i = 0; i < worldsize; i++) {
		for (j = 0; j < worldsize; j++) {
			printf("%c ", printValues(world[i + j * worldsize].type));
		}
		printf("\n");
	}
}

void printMatrixOutPut(sworld world) { /*output para Avaliacao*/
	int i, j;
	for (i = 0; i < worldsize; i++) {
		for (j = 0; j < worldsize; j++) {
			if (world[i + j * worldsize].type != EPTY)
				printf("%d %d %c\n", i, j,
						printValues(world[i + j * worldsize].type));
		}
	}
}
void printMatrixOutFile(sworld world, char* name) { /*output para Avaliacao*/
	FILE *out;
	int i, j;
	out = fopen(name, "w");
	for (i = 0; i < worldsize; i++) {
		for (j = 0; j < worldsize; j++) {
			if (world[i + j * worldsize].type > EPTY && world[i + j * worldsize].type <= SONT)
				fprintf(out,"%d %d %c\n", i, j,
						printValues(world[i + j * worldsize].type));
		}
	}
	fclose (out);
}

int addSpecial(char string) {
	switch (string) {
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

void setType(sworld my_world, int x_cord, int y_cord, char chr) {
	int type;
	if (x_cord > worldsize - 1 || y_cord > worldsize - 1 || x_cord < 0
			|| y_cord < 0) {
		printf("Invalid Input!\n");
		exit(2);
	}
	type = addSpecial(chr);
	my_world[calcPos(x_cord, y_cord, worldsize)].type = type;
	switch (type) {
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

void processReds(sworld world) {
	int i, l;
	debug("processEvens... \n");
	for (l = 0; l < worldsize; l += 2) {
		for (i = 0; i < worldsize; i += 2) {
			if (isAnimal(world[worldsize * l + i].type)) {
				/*printf("Reds 1 worldsize*l+i: %d   -  i:%d   -  l:%d\n",worldsize*l+i,i,l);*/
				goAnimal(world, worldsize * l + i,
						world[worldsize * l + i].type);
			}
		}
	}
	for (l = 1; l < worldsize; l += 2) {
		for (i = 1; i < worldsize; i += 2) {
			if (isAnimal(world[worldsize * l + i].type)) {
				/*				printf("Reds 2 worldsize*l+i: %d   -  i:%d   -  l:%d\n",worldsize*l+i,i,l);*/
				goAnimal(world, worldsize * l + i,
						world[worldsize * l + i].type);
			}
		}
	}
}

void processWhites(sworld world) {
	int i, l;
	/*debug("processOds... \n");*/
	for (l = 0; l < worldsize; l += 2) {
		for (i = 1; i < worldsize; i += 2) {
			if (isAnimal(world[worldsize * l + i].type)) {
				/*				printf("Whites 1 worldsize*l+i: %d   -  i:%d   -  l:%d\n",worldsize*l+i,i,l);*/
				goAnimal(world, worldsize * l + i,
						world[worldsize * l + i].type);
			}
		}
	}

	for (l = 1; l < worldsize; l += 2) {
		for (i = 0; i < worldsize; i += 2) {
			if (isAnimal(world[worldsize * l + i].type)) {
				/*			printf("Whites 2 worldsize*l+i: %d   -  i:%d   -  l:%d\n",worldsize*l+i,i,l);*/
				goAnimal(world, worldsize * l + i,
						world[worldsize * l + i].type);
			}
		}
	}
	/*debug("processOds DONE!\n");*/
}

void processGen(sworld world) {
	int i, j;
	/*debug("processGen... \n");*/
	for (i = 0; i < genNum; i++) {
		/*handle the breeding and starvation updates once each generation */
#pragma omp parallel for
		for (j = 0; j < worldsize * worldsize; j++) {
			if (isAnimal(world[j].type)) {
				world[j].breeding_period--;
				if (world[j].type == WOLF)
					world[j].starvation_period--;
			}
		}
/*#pragma omp sections
		{
#pragma omp section
			{*/
				processReds(world);
/*			}
#pragma omp section
			{*/
				processWhites(world);
/*			}
		}*/
		printf("\n\n Iteração nº %d\n\n", i + 1);
		printMatrix(world);
		printf("\n\n--------------------------------------\n\n\n");
	}
}

int main(int argc, char const *argv[]) {
	/****************  DECLARATIONS  ********************/
	FILE * inputFile;
	int teste;
	sworld my_world;
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
	if (teste != 1) {
		printf("Input error!\n");
		exit(-1);
	}
	printf(
			"Tamanho: %d\nwolfBP = %d, sqrlBP = %d, wolfStarvP = %d, genNum = %d\n",
			worldsize, wolfBP, sqrlBP, wolfStarvP, genNum);
	my_world = (sworld) malloc(worldsize * worldsize * sizeof(struct world));

	/*
	 READ FILE
	 */

	while (1) {
		ret = fscanf(inputFile, "%d %d %c \n", &x, &y, &chr);
		if (ret != 3)
			break;
		printf("x: %d  y: %d\n", x, y);
		setType(my_world, x, y, chr);
	}
	fclose (inputFile);
	printf("\n\nTHE WORLD:\n\n");
	printMatrix(my_world);
	printf("\tBefore \n\n\n\n");

	start = omp_get_wtime();
	processGen(my_world);
	end = omp_get_wtime();

	printMatrix(my_world);
	printf("\tAfter \n\n\n\n");
	printMatrixOutFile(my_world,argv[6]);
	printf("DEMOROU:       ->  %f  <-", end-start);
	printf("End File :D\n");
	return 0;
}
