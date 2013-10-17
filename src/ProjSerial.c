#define MAIN_FILE
#include "ProjSerial.h"
#include <stdio.h>
#include <stdlib.h>
#include "Mover.h"

#define DEBUG 0
#define EPTY 0
#define WOLF 1
#define SQRL 2
#define ICE 3
#define TREE 4
#define SONT 6
#define printInt(i) printf("%d\n",i)
#define calcPos(x, y, worldsize) y + x*worldsize

int worldsize = 0, wolfBP = 0, sqrlBP = 0, wolfStarvP = 0, genNum = 0;

void debug(char * str){
	if(DEBUG){
		printf(str);
	}
}

char printValues(int x){
	switch(x){
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
	}
	return ' ';
}

void printMatrix(sworld world){
	int i,j;
	for(i=0;i<worldsize;i++){
		for(j=0;j<worldsize ; j++){
			printf("%c ", printValues(world[i+j*worldsize].type));
		}
		printf("\n");
	}
}

int addSpecial(char string){
	switch (string){
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
	if(x_cord > worldsize-1 || y_cord > worldsize-1 || x_cord < 0 || y_cord < 0){
		printf("Invalid Input!\n");
		exit(2);
	}
	int type = addSpecial(chr);
	my_world[calcPos(x_cord, y_cord, worldsize)].type = type;
	//printf("Pos real %d a pos do define %d\n", y_cord+x_cord*worldsize, calcPos(x_cord, y_cord, worldsize));
	switch(type){
		case WOLF:
			my_world[calcPos(x_cord, y_cord, worldsize)].breeding_period = wolfBP;
			my_world[calcPos(x_cord, y_cord, worldsize)].starvation_period = wolfStarvP;
			break;
		case SQRL:
			my_world[calcPos(x_cord, y_cord, worldsize)].breeding_period = sqrlBP;
			break;
		default:
			break;
	}

}

void processEvens(sworld world){
	int i;
	debug("processEvens... \n");
	for(i = 0;i<worldsize*worldsize;i+=2){
		if(isAnimal(world[i].type)){
			goAnimal(world,i);
		}
	}
	debug("processEvens DONE!\n");
}
void processOds(sworld world){
	int i;
	debug("processOds... \n");
	for(i = 1;i<worldsize*worldsize;i+=2){
//		printf("i:%d =  %d\n",i, world[i].type);
		if(isAnimal(world[i].type)){
			goAnimal(world,i);
		}
	}
	debug("processOds DONE!\n");
}

void processGen(sworld world){
	int i;
	debug("processGen... \n");
	for(i = 0;i<genNum;i++){
		processEvens(world);
		processOds(world);
	}
}


int main(int argc, char const *argv[]){  
	wolfBP = atoi(argv[2]);
	sqrlBP = atoi(argv[3]);
	wolfStarvP = atoi(argv[4]);
	genNum = atoi(argv[5]);

	FILE * inputFile;
	inputFile = fopen(argv[1],"r");
	int teste = fscanf(inputFile,"%d", &worldsize);
	printf("Tamanho: %d\nwolfBP = %d, sqrlBP = %d, wolfStarvP = %d, genNum = %d\n", worldsize, wolfBP, sqrlBP, wolfStarvP, genNum);
	sworld my_world = (sworld) malloc( worldsize*worldsize*sizeof(sworld));


	/*
		READ FILE
	*/
	int ret=3, x,y;
	char chr;
	while(1){
		ret=fscanf(inputFile,"%d %d %s\n", &x, &y, &chr);
		if(ret != 3)
			break;
		printf("x: %d  y: %d\n", x, y);
		setType(my_world,x,y,chr);
	}
	printf("\n\nTHE WORLD:\n\n");
	printMatrix(my_world);
	printf("\tBefore \n\n\n\n");

	processGen(my_world);

	printMatrix(my_world);
	printf("\tAfter \n\n\n\n");

	printf("End File :D\n");
	return 0;
}
