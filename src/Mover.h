/*
 * Mover.h
 *
 */

#ifndef MOVER_H_
#define MOVER_H_
#include "ProjSerial.h"
#include <stdio.h>
#include <stdlib.h>
#define POSSIBLE_POS 4
/*#define printInt(i) printf("%d\n",i)*/
#define calcPos(x, y, worldsize) y + x*worldsize

void cleanPos(sworld world, int x, int y);
int getPositionType(sworld world, int x, int y);
sworld getPositionStructure(sworld world, int x, int y);
void setPosition(sworld world, int x, int y, int type, int breedingPeriod,
		int starvationPeriod);
void setPositionType(sworld world, int x, int y, int type);
void setPositionBreeding(sworld world, int x, int y, int breedingPeriod);
void setPositionStarvation(sworld world, int x, int y, int starvationPeriod);
int isAnimal(int type);
int isAble(sworld world, int x_from, int y_from, int type ); 	/*0 não se pode mexer / 1 pode-se mexer*/
void move(sworld world_from, int x_from, int y_from, sworld world_to, int x_to, int y_to);
void goAnimal(sworld world_init, sworld world_final, int pos, int type);
#endif /* MOVER_H_ */
