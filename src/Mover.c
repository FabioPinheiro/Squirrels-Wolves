#include "Mover.h"
#include "ProjSerial.h"

void cleanPos(sworld world, int x, int y){
	world[calcPos(x,y,worldsize)].type = EPTY;
	world[calcPos(x,y,worldsize)].breeding_period = 0;
	world[calcPos(x,y,worldsize)].starvation_period = 0;
}



int getPositionType(sworld world, int x, int y){
	return world[calcPos(x,y,worldsize)].type;
}

sworld getPositionStructure(sworld world, int x, int y){
	return &world[calcPos(x,y,worldsize)];
}

void setPosition(sworld world, int x, int y, int type, int breedingPeriod, int starvationPeriod){
	world[calcPos(x,y,worldsize)].type = type;
	world[calcPos(x,y,worldsize)].breeding_period = breedingPeriod;
	world[calcPos(x,y,worldsize)].starvation_period = starvationPeriod;
}

void setPositionType(sworld world, int x, int y, int type){
	world[calcPos(x,y,worldsize)].type = type;
}
void setPositionBreeding(sworld world, int x, int y, int breedingPeriod){
	world[calcPos(x,y,worldsize)].breeding_period = breedingPeriod;
}

void setPositionStarvation(sworld world, int x, int y, int starvationPeriod){
	world[calcPos(x,y,worldsize)].starvation_period = starvationPeriod;
}
int isAnimal(int type){
	return (type == WOLF || type == SQRL);
}
int isAble(sworld world, int x_from, int y_from){

	return 1; //TODO: Tem que se ver se é able ou nao!!
}
void move(sworld world, int x_from, int y_from, int x_to, int y_to){
	sworld aux = getPositionStructure(world,x_from,y_from);
	if(isAnimal(aux->type) && isAble(aux, x_to,y_to)){
		setPosition(world,x_to,y_to, aux->type, aux->breeding_period,aux->starvation_period);
		cleanPos(world,x_from,y_from);
	}
}
