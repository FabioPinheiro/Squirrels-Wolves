#include "Mover.h"

void cleanPos(sworld world, int x, int y) {
	if(world[calcPos(x,y,worldsize)].type!=SONT)
		world[calcPos(x,y,worldsize)].type = EPTY;
	else
		world[calcPos(x,y,worldsize)].type = TREE;

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
	ret = (type == WOLF || type == WES || type == SQRL || type == SONT);

	return ret;
}
int isAble(sworld world, int x_pos, int y_pos, int type) {
	/*0 não se pode mexer
	1 pode-se mexer
	 */
	/*TODO ver se está dentro do quadrado*/
	int positionType;
	
	if(x_pos < 0 || y_pos < 0 || x_pos >= worldsize || y_pos >= worldsize){
		return 0; /*fora do quadrado*/
	}
	positionType = world[calcPos(x_pos, y_pos, worldsize)].type;

	

	/* check lobos */
	if(type == WOLF ||type == WES ){
		if(positionType == EPTY || positionType == SQRL){ /*empty or squirrel or wolf*/
		return 1;
		}
	}
	else{/*check esquilos SQRT e SONT*/
		if(positionType != ICE && positionType != WOLF && positionType != WES){ /*if not ice or wolf */
		return 1;
		}
	}
	return 0; 
}
void move(sworld world_from, int x_from, int y_from, sworld world_to, int x_to, int y_to){
	/*TODO be careful with   SONT*/
	/*
	If a squirrel/wolf completes a breeding period if he moves it breeds
	Handle colissions
	*/
	sworld aux = getPositionStructure(world_from, x_from, y_from);
	sworld finalPos = getPositionStructure(world_to, x_to, y_to);
	int star, breed;

	if(aux->type != WOLF || aux->starvation_period != 0){ /*still healthy we don't like weak wolfs*/
		if(isAnimal(finalPos->type)) {
			star = aux->starvation_period - finalPos->starvation_period;
			breed = aux->breeding_period - finalPos->breeding_period;
			/*COLISIONS*/
			/*WOLF vs WOLF */
			if(finalPos->type == WOLF){
				if(aux->type == WOLF ){
					if(star != 0){
						if(star > 0)
							setPosition(world_to, x_to, y_to, aux->type, aux->breeding_period,aux->starvation_period);
					}
					else{
						if(breed > 0)
							setPosition(world_to, x_to, y_to, aux->type, aux->breeding_period,aux->starvation_period);
					}
				}
				if(aux->type == WES  ){
					if(star != 0){
						if(star > 0)
							setPosition(world_to, x_to, y_to, WOLF, aux->breeding_period,wolfStarvP);
					}
					else{
						if(breed > 0)
							setPosition(world_to, x_to, y_to, WOLF, aux->breeding_period,wolfStarvP);
					}
				}
				
				else{
					/* We have a suicidal SQRL muahahahahah!!!
						SQRL vs WOLF wolf eating a squirrel
					*/
					if(aux->type == WES)
						finalPos->breeding_period = wolfStarvP;
					finalPos->type = WES;

				}
			}
			else{
				/*WOLF vs SQRL*/
				if(aux->type == WOLF){
					setPosition(world_to, x_to, y_to, WES, aux->breeding_period,aux->starvation_period);
					cleanPos(world_from, x_to, y_to);
					/*Apagar esquilo da nova posiçao na matrix antiga (pos +1)*/
				}
				if(aux->type == WES){
					setPosition(world_to, x_to, y_to, WES, aux->breeding_period,wolfStarvP);
					cleanPos(world_from, x_to, y_to);
					/*Apagar esquilo da nova posiçao na matrix antiga (pos +1)*/
				}
				/*SQRL vs SQRL*/
				else{
					if (breed > 0)
					{
						setPosition(world_to, x_to, y_to, aux->type, aux->breeding_period,aux->starvation_period);
					}
				}
			}
			/*setPosition(world, x_to, y_to, aux->type, aux->breeding_period,aux->starvation_period);*/
		}

		else{
			if(aux->type == WES)
					setPosition(world_to, x_to, y_to, WOLF, aux->breeding_period,aux->wolfStravP);
			else{
			setPosition(world_to, x_to, y_to, aux->type, aux->breeding_period,aux->starvation_period);
			}
			

		}


		/*Let's see if we have any suprises*/
		if(aux->breeding_period == 0){
		/*we have a litle baby!*/
			/*
				wolfBP, sqrlBP, wolfStarvP
			*/
			/*finalPos->breeding_period = wolfBP;*/
			
			if(aux->type == WOLF ||aux->type == WES){
				aux->breeding_period = wolfBP;
				aux->starvation_period = wolfStarvP;
				finalPos->breeding_period = wolfBP;
/*mete o breeding period do wolf ao inicial no novo mundo*/
				if(aux->type == WES)
					setPosition(world_to, x_to, y_to, WOLF, finalPos->breeding_period,finalPos->wolfStravP);
				setPosition(world_to, x_from, y_from, WOLF, aux->breeding_period,aux->starvation_period);  /*new baby here*/
			
			}
			else{
				aux->breeding_period = sqrlBP;
				finalPos->breeding_period = sqrlBP; /*mete o breeding period do squirrel ao inicial no novo mundo*/
				setPosition(world_to, x_from, y_from, aux->type, aux->breeding_period,aux->starvation_period); /*new baby here*/
			}
			
		}
		else{/*mexeu-se  limpa-se mundos*/
			cleanPos(world_from, x_from, y_from);
			cleanPos(world_to, x_from, y_from);
		}
	}
	else{
		/*is dead :( Limpar nos 2 mundos
*/
		cleanPos(world_from, x_from, y_from);
		cleanPos(world_to, x_from, y_from);
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
		int i, nPos;
		int numbPossible=0;
		int theChoosenOne;
		int vec[POSSIBLE_POS]={-1,-1,-1,-1};
		int ret = 0;
		/*Pos Left 0*/
		/*x y-1*/
		if(isAble(world, x,y-1, type)){
			numbPossible++;
			vec[0]=calcPos(x,y-1,worldsize);
		}
		/*Pos Up 0*/
		/*(x-1)*worldsize y*/
		if(isAble(world, (x+1), y, type)){
			numbPossible++;
			vec[1]=calcPos((x+1) ,y,worldsize);
		}
		/*Pos Rigth 0*/
		/*x y+1*/

		if(isAble(world, x,y+1, type)){
			numbPossible++;
			vec[2]=calcPos(x,y+1,worldsize);
		}
		/*Pos Down 0*/
		/*(x+1)*worldsize y*/
		if(isAble(world, (x-1),y, type)){

			numbPossible++;

			vec[3]=calcPos((x-1),y,worldsize);
		}

		/* Matrix do rafael 
	UP
		if(isAble(world, x-1,y, type)){
			numbPossible++;
			vec[0]=calcPos(x-1,y,worldsize);
		}
		Rigth

		if(isAble(world, x, y+1, type)){
			numbPossible++;
			vec[1]=calcPos(x ,y+1,worldsize);
		}
Down
	
		if(isAble(world, x+1,y, type)){
			numbPossible++;
			vec[2]=calcPos(x+1,y,worldsize);
		}
	Left
	
		if(isAble(world, x,y-1, type)){
			numbPossible++;
			vec[3]=calcPos(x,y-1,worldsize);
		}

		*/

		/*calculating C MOD numbPossible pag 2 enum*/
		if(numbPossible == 0)
			return -1;
		nPos=calcPos(x,y,worldsize);
		theChoosenOne = nPos%numbPossible;
		/*printf("\n\ntheChoosenOne: %d calcPos:%d  numbPossible: %d calcPos(x,y,worldsize)MODnumbPossible: %d \n\n",theChoosenOne,calcPos(x,y,worldsize),numbPossible, calcPos(x,y,worldsize)%numbPossible);
		printf("[%d , %d , %d , %d]\n\n",vec[0],vec[1],vec[2],vec[3]);*/
		for(i=0; i < POSSIBLE_POS; i++){
			if(vec[i] != -1){
				if(theChoosenOne == 0){
					ret=vec[i];
					return ret;
				}
				else{
					theChoosenOne--;
				}
			}
		}



	return ret;
}
void goAnimal(sworld world_init, sworld world_final, int pos, int type){
	int y_init, x_init, y_final, x_final, posFinal;
	calcCords(pos, &x_init, &y_init);

	posFinal =/* (2 < rand()%5)?pos+1:pos+worldsize;*/calcMovePos(world_init, x_init, y_init,type);
	/*posFinal = posFinal<(worldsize*worldsize)? posFinal: posFinal%(worldsize*worldsize);*/
	if(posFinal<0)
		return;

/*	printf("posição: %d final: %d\n",pos, posFinal);*/
	calcCords(posFinal, &x_final, &y_final);
	move(world_init, x_init, y_init, world_final, x_final, y_final);
}
