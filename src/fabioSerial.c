#ifndef FABIO_INTERFACE_H
#define FABIO_INTERFACE_H

#define EPTY 0
#define WOLF 1
#define SQRL 2
#define ICE 3
#define TREE 4
#define SONT 6 /*squirrel on a tree*/
#define WES 7 /*a wold eating a squirrel*/

typedef struct strAnimal *sAnimal;
struct strAnimal {
	int type; /* Wolf, Squirrel*/
 	int child;
 	int update;
 	int diedata;
 	sAnimal next;
 };

typedef struct strWorld *sWorld;
struct strWorld {
	int type; /* ICE, TREE*/
	sAnimal pinha;
 };

sWorld myWorld

extern int worldsize, wolfBP, sqrlBP, wolfStarvP, genNum;

void printMatrixOutPut(){} //TODO
#endif FABIO_INTERFACE_H
/*************************************************************************/
#ifndef FABIO_FILE_IO_H
#define FABIO_FILE_IO_H
#endif FABIO_FILE_IO_H
/*************************************************************************/

/*************************************************************************/
#ifndef FABIO_WROLD_F_H
#define FABIO_WROLD_F_H
/*void calcCordsXY(int pos, int* x, int* y) {
	int aux1 = 0, aux2 = 0;
	aux1 = pos / worldsize;
	(*x) = aux1;
	aux2 = pos - aux1 * worldsize;
	(*y) = aux2;
}*/
void calcCordsPOS(int* pos, int x, int y) {
	*pos = y * worldsize + x
}
void setType(int x, int y, char chr){
	int type;
	if (x_cord > worldsize - 1 || y_cord > worldsize - 1 || x_cord < 0
			|| y_cord < 0) {
		printf("Invalid Input!\n");
		exit(2);
	}
	switch (chr) {
		case 'w':
			setNewBeast(x,y,WOLF)
			break;
		case 's':
			setNewBeast(x,y,SQRL)
			type = SQRL;
			break;
		case 'i':
			setLand(x,y,ICE)
			type = ICE;
			break;
		case 't':
			setLand(x,y,TREE)
			type = TREE;
			break;
	}
}
void setLand(int x, int y, int type);
void setNewBeast(int x, int y, int type);
#endif FABIO_WROLD_F_H

/*************************************************************************/
#ifndef FABIO_SERIAL_H
#define FABIO_SERIAL_H
int main(int argc, char const *argv[]) {
	/****************  DECLARATIONS  ********************/
	FILE * inputFile;
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
	if (fscanf(inputFile, "%d", &worldsize) != 1) {
		printf("Input error!\n");
		exit(-1);
	}

	myWorld = (sworld) calloc(worldsize * worldsize, sizeof(struct strWorld));

	/*
	 READ FILE
	 */

	while (1) {
		ret = fscanf(inputFile, "%d %d %c \n", &x, &y, &chr);
		if (ret != 3)
			break;
		setType(x, y, chr);
	}
	fclose(inputFile);


	start = omp_get_wtime();
	my_world1=  processGen(my_world1,my_world2);
	end = omp_get_wtime();
	 printMatrixOutFile(my_world1, "SerialOut.out");
/*	printMatrix(my_world);
	printf("\tAfter \n\n\n\n");*/
	printTimeOutFile(end-start);
	return 0;
}
#endif /* FABIO_SERIAL_H */

