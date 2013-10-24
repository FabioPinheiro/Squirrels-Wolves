#include "Mover.h"

void cleanPos(sworld world, int x, int y) {
	world[calcPos(x,y,worldsize)].type = EPTY;
	world[calcPos(x,y,worldsize)].breeding_period = 0;
	world[calcPos(x,y,worldsize)].starvation_period = 0;
}

int getPositionType(sworld world, int x, int y) {
	return world[calcPos(x,y,worldsize)].type;
}
int getPositionBreedingPeriod(sworld world, int x, int y) {
	return world[calcPos(x,y,worldsize)].breeding_period;
}
int getPositionStarvationPeriod(sworld world, int x, int y) {
	return world[calcPos(x,y,worldsize)].starvation_period;
}

sworld getPositionStructure(sworld world, int x, int y) {
	return &world[calcPos(x,y,worldsize)];
}

void setPosition(sworld world, int x, int y, int type, int breedingPeriod,
		int starvationPeriod) {
	world[calcPos(x,y,worldsize)].type = type;
	world[calcPos(x,y,worldsize)].breeding_period = breedingPeriod;
	world[calcPos(x,y,worldsize)].starvation_period = starvationPeriod;
}

void setPositionType(sworld world, int x, int y, int type) {
	world[calcPos(x,y,worldsize)].type = type;
}
void setPositionBreeding(sworld world, int x, int y, int breedingPeriod) {
	world[calcPos(x,y,worldsize)].breeding_period = breedingPeriod;
}

void setPositionStarvation(sworld world, int x, int y, int starvationPeriod) {
	world[calcPos(x,y,worldsize)].starvation_period = starvationPeriod;
}
int isAnimal(int type) {
	int ret = 0;
	ret = (type == WOLF || type == SQRL || type == SONT);

	return ret;
}
int isAble(sworld world, int x_from, int y_from, int type) {
	/*0 não se pode mexer
	1 pode-se mexer
	 */
	/*TODO ver se está dentro do quadrado*/

	int positionType = world[calcPos(x_from, y_from, worldsize)].type;
	if(x_from < 0 || y_from < 0 || x_from > worldsize*worldsize || y_from > worldsize*worldsize){
		return 0; /*fora do quadrado*/
	}

	/* check lobos */
	if(type == WOLF){
		if(positionType == EPTY || positionType == SQRL || positionType == WOLF ){ /*empty or squirrel or wolf*/
		return 1;
		}
	}
	else{/*check esquilos SQRT e SONT*/
		if(positionType != ICE && positionType != WOLF ){ /*if not ice or wolf */
		return 1;
		}
	}
	


	return 0; 
}
void move(sworld world, int x_from, int y_from, int x_to, int y_to) {
	/*TODO be careful with   SONT*/
	/*
	If a squirrel/wolf completes a breeding period if he moves it breeds
	Handle colissions
	*/
	sworld aux = getPositionStructure(world, x_from, y_from);
	sworld finalPos = getPositionStructure(world, x_to, y_to);
	int star, breed;

	if(aux->type != WOLF || aux->starvation_period != 0){ /*still healthy we don't like weak wolfs*/
		if(isAnimal(finalPos->type)) {
			star = aux->starvation_period - finalPos->starvation_period;
			breed = aux->breeding_period - finalPos->breeding_period;
			/*COLISIONS*/
			/*WOLF vs WOLF */
			if(finalPos->type == WOLF){
				if(aux->type == WOLF){
					
					if(star != 0){
						if(star > 0)
							setPosition(world, x_to, y_to, aux->type, aux->breeding_period,aux->starvation_period);
					}
					else{
						if(breed > 0)
							setPosition(world, x_to, y_to, aux->type, aux->breeding_period,aux->starvation_period);
					}
				}
				else{
					/* We have a suicidal SQRL muahahahahah!!!
						SQRL vs WOLF
					*/
					finalPos->starvation_period = wolfStarvP;
						
				}
			}
			else{
				/*WOLF vs SQRL*/
				if(aux->type == WOLF){
					setPosition(world, x_to, y_to, aux->type, aux->breeding_period,aux->starvation_period);
					finalPos->starvation_period = wolfStarvP;
				}
				/*SQRL vs SQRL*/
				else{
					if (breed > 0)
					{
						setPosition(world, x_to, y_to, aux->type, aux->breeding_period,aux->starvation_period);
					}
				}
			}
			/*setPosition(world, x_to, y_to, aux->type, aux->breeding_period,aux->starvation_period);*/
		}
		else{
			setPosition(world, x_to, y_to, aux->type, aux->breeding_period,aux->starvation_period);
		}


		/*Let's see if we have any suprises*/
		if(aux->breeding_period == 0){
		/*we have a litle baby!*/
			/*
				wolfBP, sqrlBP, wolfStarvP
			*/
			if(aux->type == WOLF){
				aux->breeding_period = wolfBP;
				aux->starvation_period = wolfStarvP;
			}
			else{
				aux->breeding_period = sqrlBP;
			}
			
		}
		else{
			cleanPos(world, x_from, y_from);
		}
	}
	else{
		/*is dead :(*/
		cleanPos(world, x_from, y_from);
	}
}

void calcCords(int pos, int* x, int* y) {
	int aux1 = 0, aux2 = 0;
	aux1 = pos / worldsize;
	(*x) = aux1;
	aux2 = pos - aux1 * worldsize;
	(*y) = aux2;
}

int calcMovePos(sworld world, int x, int y, int type){ /*calc the Next pos*/
	/* trees and ice don't move
	• Number the possible choices starting from 0, clockwise starting from the 12:00 
		position (i.e. up, right, down, left). Note that only cells that are unoccupied (for 
		moves) or occupied by squirrels (for wolves to eat), should be numbered. Call the 
		number of possible cells p. 
	• Compute the grid cell number of the cell being evaluated. If the cell is at position 
		(i,j) in the world grid with (0,0) the grid origin, and the grid is of size MxN, the grid 
		cell number is C = i x N + j .
	• The cell to select is then determined by C mod p. For example, if there are 3 possible 
		cells to choose from, say up, down and left, then if C mod p is 0 the selected cell is 
		up from the current cell, if it is 1 then select down, and if it is 2 then select left.
	*/

		/*TODO numerar posições*/
		int i;
		int numbPossible=0;
		int theChoosenOne;
		int vec[POSSIBLE_POS]={0,0,0,0};
		int ret = 0;
		/*Pos Up 0*/
		/*(x-1)*worldsize y*/
		if(isAble(world, (x-1), y, type)){
			numbPossible++;
			vec[0]=calcPos((x-1) ,y,worldsize);
		}
		/*Pos Rigth 0*/
		/*x y+1*/
		if(isAble(world, x,y+1, type)){
			numbPossible++;
			vec[1]=calcPos(x,y+1,worldsize);
		}
		/*Pos Down 0*/
		/*(x+1)*worldsize y*/
		if(isAble(world, (x+1),y, type)){
			numbPossible++;
			vec[2]=calcPos((x+1),y,worldsize);
		}
		/*Pos Left 0*/
		/*x y-1*/
		if(isAble(world, x,y-1, type)){
			numbPossible++;
			vec[3]=calcPos(x,y-1,worldsize);
		}
		/*calculating C MOD numbPossible pag 2 enum*/
		theChoosenOne = calcPos(x,y,worldsize)%numbPossible;

		for(i=0; i < POSSIBLE_POS; i++){
			if(vec[i] != 0){
				if(theChoosenOne == 0){
					ret=vec[i];
				}
				else{
					theChoosenOne--;
				}
			}
		}



	return ret;
}

void goAnimal(sworld world, int pos, int type) {
	int y_init, x_init, y_final, x_final, posFinal;
	calcCords(pos, &x_init, &y_init);
	posFinal =/* (2 < rand()%5)?pos+1:pos+worldsize;*/calcMovePos(world,x_init, y_init,type);
	/*posFinal = posFinal<(worldsize*worldsize)? posFinal: posFinal%(worldsize*worldsize);*/
	printf("posição: %d final: %d\n",pos, posFinal);
	calcCords(posFinal, &x_final, &y_final);
	move(world, x_init, y_init, x_final, y_final);
}
