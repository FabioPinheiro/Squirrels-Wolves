/*
 * MoverMPI.h
 *
 */

#ifndef MOVER_H_
#define MOVER_H_
#include "headersMPI.h"
#include <stdio.h>
#include <stdlib.h>
#define POSSIBLE_POS 4
/*#define printInt(i) printf("%d\n",i)*/
#define calcPos(x, y, worldsize) y + x*worldsize

void cleanPos(sworld world, int x, int y, int worldsize);
int getPositionType(sworld world, int x, int y, int worldsize);
sworld getPositionStructure(sworld world, int x, int y, int worldsize);
void setPosition(sworld world, int x, int y, int type, int breedingPeriod,
		int starvationPeriod, int worldsize);
void setPositionType(sworld world, int x, int y, int type, int worldsize);
void setPositionBreeding(sworld world, int x, int y, int breedingPeriod, int worldsize);
void setPositionStarvation(sworld world, int x, int y, int starvationPeriod, int worldsize);
int isAnimal(int type);
int isAble(sworld world, int x_from, int y_from, int type, int xSize, int ySize ); 	/*0 não se pode mexer / 1 pode-se mexer*/
void move(sworld world_from, int x_from, int y_from, sworld world_to, int x_to, int y_to, int worldsize);
void goAnimal(sworld world_init, sworld world_final, int pos, int type, int xSize, int ySize); /*TODO hacked here*/
#endif /* MOVER_H_ */
