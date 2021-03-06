#include "Mover.h"

void cleanPos(sworld world, int x, int y) {
	if (world[calcPos(x,y,worldsize)].type != SONT)
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
	/*XXX ver se está dentro do quadrado*/
	int positionType;

	if (x_pos < 0 || y_pos < 0 || x_pos >= worldsize || y_pos >= worldsize) {
		return 0; /*fora do quadrado*/
	}
	positionType = world[calcPos(x_pos, y_pos, worldsize)].type;

	/* check lobos */
	if (type == WOLF || type == WES) {
		if (positionType == SQRL)
			return 2;
		if (positionType == EPTY) { /*empty or squirrel or wolf*/
			return 1;
		}
	} else {/*check esquilos SQRT e SONT*/
		if (positionType != ICE && positionType != WOLF && positionType != WES) { /*if not ice or wolf */
			return 1;
		}
	}
	return 0;
}

void cleanWorldPos(sworld world_from, sworld world_to, int x_from, int y_from){
	cleanPos(world_from, x_from, y_from);
	cleanPos(world_to, x_from, y_from);
}

void checkBabies(sworld world_from, sworld world_to, sworld animalAux, int x_from, int y_from) {
	/*AnimalAux ja está na nova posiçao*/
	/*se sim cria o bebe*/
	if (animalAux->breeding_period == 0) {
		sworld newAnimal = getPositionStructure(world_from, x_from, y_from);
		/*ver se é SQRL WOLF e WES */
		if (animalAux->type == WOLF || animalAux->type == WES) {
			newAnimal->breeding_period = wolfBP;
			newAnimal->starvation_period = wolfStarvP;
			if (animalAux->type == WES) {
				animalAux->breeding_period = wolfBP;
				animalAux->starvation_period = wolfStarvP;
			} else {
				animalAux->breeding_period = wolfBP;
			}
		} else {
			newAnimal->breeding_period = sqrlBP;
			animalAux->breeding_period = sqrlBP;
		}
	}
}

void moveWOLFs(sworld world_from, sworld world_to, sworld animalAuxFrom,
		sworld animalAuxTo, int x_from, int y_from, int x_to, int y_to) {
	if (animalAuxTo->type == WOLF || WES) {
		int type;
		if(animalAuxTo->type == WES){
			type = WES;
		}
		else{
			type = WOLF;
		}
		/*Se for WES é type pq está lá um esquilo e o que chega se sobreviver acaba de comer o esquilo!!*/
		/*Starvation Maior*/
		if(animalAuxFrom->starvation_period > animalAuxTo->starvation_period){
			setPosition(world_to, x_to, y_to, type, animalAuxFrom->breeding_period,animalAuxFrom->starvation_period);
		}
		else{
			if(animalAuxFrom->starvation_period < animalAuxTo->starvation_period){
				setPosition(world_to, x_to, y_to, type, animalAuxTo->breeding_period,animalAuxTo->starvation_period);
			}
			else{
				if(animalAuxFrom->starvation_period == animalAuxTo->starvation_period){
				/*Breeding Maior*/
					if(animalAuxFrom->breeding_period > animalAuxTo->breeding_period){
						setPosition(world_to, x_to, y_to, type, animalAuxFrom->breeding_period,animalAuxFrom->starvation_period);
					}
					else{
						if(animalAuxFrom->breeding_period <= animalAuxTo->breeding_period){
							setPosition(world_to, x_to, y_to, type, animalAuxTo->breeding_period,animalAuxTo->starvation_period);
						}
						else{
							printf("BUG stupid BUG");
						}

					}

				}
			}
		}

	} else { /*WOLF VS SQRL*/
		if (animalAuxTo->type == SQRL) {
			if (animalAuxFrom->type == WES) {
				setPosition(world_to, x_to, y_to, WES, animalAuxFrom->breeding_period,
						wolfStarvP);
			} else {
				setPosition(world_to, x_to, y_to, WES, animalAuxFrom->breeding_period,
						animalAuxFrom->starvation_period);
			}
			/*cleanPos(world_from, x_to, y_to);*/ /*Come o esquilo e limpa a posição*/
		} else {
			printf("[BUG] the gosths apear in moveWolf world to :s");
		}
	}
	checkBabies(world_from,world_to,animalAuxFrom, x_from, y_from);
	cleanWorldPos(world_from, world_to, x_from,y_from);

}
void moveSQRLs(sworld world_from, sworld world_to, sworld animalAuxFrom,
		sworld animalAuxTo, int x_from, int y_from, int x_to, int y_to) {
	if (animalAuxTo->type == WOLF || WES) { /*Animal to is in world_to*/
		/* Le Suicide Squirrel*/
		if (animalAuxTo->type == WES) {
			setPosition(world_to, x_to, y_to, WES, animalAuxTo->breeding_period,
					wolfStarvP);
		} else {
			setPosition(world_to, x_to, y_to, WES, animalAuxTo->breeding_period,
					animalAuxTo->starvation_period);
		}
	/*	cleanPos(world_from, x_to, y_to); *//*o esquilo é comido e limpa a posição*/ /*?*/
	} else {
		if (animalAuxTo->type == SQRL || animalAuxTo->type == SONT) {
			/*Breeding Maior*/
			if(animalAuxFrom->breeding_period > animalAuxTo->breeding_period){
				setPosition(world_to, x_to, y_to, animalAuxTo->type, animalAuxFrom->breeding_period,animalAuxFrom->starvation_period); /*to type pq pode ser SONT*/
			}
			else{
				if(animalAuxFrom->breeding_period <= animalAuxTo->breeding_period){
					setPosition(world_to, x_to, y_to, animalAuxTo->type, animalAuxTo->breeding_period,animalAuxTo->starvation_period); /*to type pq pode ser SONT*/
				}
				else{
					printf("BUG stupid BUG");
				}

			}
		} else {
			printf("[BUG] the gosths apear in MoveSQRL ToMatrix :s");
		}
	}
	checkBabies(world_from,world_to,animalAuxFrom, x_from, y_from);
	cleanWorldPos(world_from, world_to, x_from,y_from);
}


void move(sworld world_from, int x_from, int y_from, sworld world_to, int x_to,
		int y_to) {
	/*XXX be careful with   SONT&WES*/
	/*
	 If a squirrel/wolf completes a breeding period if he moves it breeds
	 Handle colissions
	 */
	int fromAnimalType;
	sworld fromAuxPos = getPositionStructure(world_from, x_from, y_from);
	sworld toAuxPos = getPositionStructure(world_from, x_to, y_to);
	sworld toPos = getPositionStructure(world_to, x_to, y_to);
	sworld toAnimal;

	/* Ve qual é o tipo de animal para qual se está a mover,
	 * ve primeiro no mapa  (possivelmente) actualizado e so depois no antigo*/
	if (isAnimal(toPos->type)) {
		toAnimal = getPositionStructure(world_to, x_to, y_to);
	} else {
		if (isAnimal(toAuxPos->type)) {
			toAnimal = getPositionStructure(world_from, x_to, y_to);
		} else {
			/*Não há animal na casa de destino*/
			/*check type and then move*/
			if (fromAuxPos->type == WES)
				setPosition(world_to, x_to, y_to, WOLF, fromAuxPos->breeding_period,
						wolfStarvP);
			else {
				setPosition(world_to, x_to, y_to, fromAuxPos->type,
						fromAuxPos->breeding_period, fromAuxPos->starvation_period);
			}

			/*check if they have babies*/
			checkBabies(world_from, world_to, toAuxPos, x_from, y_from);
			cleanWorldPos(world_from, world_to, x_from, y_from);
			return;
		}
	}
	/*Ha animal na casa de destino*/
	/* WOLFS vs WOLFS*/
	/* WOLFS vs SQRLS*/
	fromAnimalType = fromAuxPos->type;
	if (fromAnimalType == WOLF || fromAnimalType == WES) {
		moveWOLFs(world_from, world_to,fromAuxPos, toAnimal, x_from, y_from, x_to, y_to);
		/*checkBabies(world_from, world_to, toAuxPos, x_from, y_from);*/
	}
	/*SQRLs vs SQRLs*/
	/*SQRLs vs WOLFS*/
	else {
		if (fromAnimalType == SQRL || fromAnimalType == SONT) {
			moveSQRLs(world_from, world_to,fromAuxPos, toAnimal, x_from, y_from, x_to, y_to);
			/*checkBabies(world_from, world_to, toAuxPos, x_from, y_from);*/
		} else {
			printf(
					"[BUG] We have am huge Error we have gosths in our MAP RUN RUN");
		}
	}
}

	void calcCords(int pos, int* x, int* y) {
		int aux1 = 0, aux2 = 0;
		aux1 = pos / worldsize;
		(*x) = aux1;
		aux2 = pos - aux1 * worldsize;
		(*y) = aux2;
	}

	int calcMovePos(sworld world, int x, int y, int type) { /*calc the Next pos*/
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

		/* numerar posições*/
		int i, nPos;
		int numbPossible = 0;
		int theChoosenOne;
		int vec[POSSIBLE_POS] = { -1, -1, -1, -1 };
		int vecWolf[POSSIBLE_POS] = { -1, -1, -1, -1 };
		int ret = 0;
		int numbPossibleWolf = 0;
		/*Pos Left 0*/
		/*x y-1*/

		if (isAble(world, x, y - 1, type)) {
			numbPossible++;
			vec[0] = calcPos(x,y-1,worldsize);
		}
		/*Pos Up 0*/
		/*(x-1)*worldsize y*/
		if (isAble(world, (x + 1), y, type)) {
			numbPossible++;
			vec[1] = calcPos((x+1) ,y,worldsize);
		}
		/*Pos Rigth 0*/
		/*x y+1*/

		if (isAble(world, x, y + 1, type)) {
			numbPossible++;
			vec[2] = calcPos(x,y+1,worldsize);
		}
		/*Pos Down 0*/
		/*(x+1)*worldsize y*/
		if (isAble(world, (x - 1), y, type)) {

			numbPossible++;

			vec[3] = calcPos((x-1),y,worldsize);
		}

		if (type == WOLF || type == WES) {
			if (isAble(world, x, y - 1, type)) {
				if (isAble(world, x, y - 1, type) == 2) {
					numbPossibleWolf++;
				}
				numbPossible++;
				vec[0] = calcPos(x,y-1,worldsize);
				vecWolf[0] = calcPos(x,y-1,worldsize);
			}
			/*Pos Up 0*/
			/*(x-1)*worldsize y*/
			if (isAble(world, (x + 1), y, type)) {
				if (isAble(world, (x + 1), y, type) == 2) {
					numbPossibleWolf++;

				}
				numbPossible++;
				vec[1] = calcPos((x+1) ,y,worldsize);
				vecWolf[1] = calcPos((x+1) ,y,worldsize);
			}
			/*Pos Rigth 0*/
			/*x y+1*/

			if (isAble(world, x, y + 1, type)) {
				if (isAble(world, x, y + 1, type) == 2) {
					numbPossibleWolf++;
				}
				numbPossible++;
				vec[2] = calcPos(x,y+1,worldsize);
				vecWolf[2] = calcPos(x,y+1,worldsize);
			}
			/*Pos Down 0*/
			/*(x+1)*worldsize y*/
			if (isAble(world, (x - 1), y, type)) {
				if (isAble(world, (x - 1), y, type) == 2) {
					numbPossibleWolf++;

				}
				numbPossible++;

				vec[3] = calcPos((x-1),y,worldsize);
				vecWolf[3] = calcPos((x-1),y,worldsize);
			}
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
		if (numbPossible == 0)
			return -1;
		nPos = calcPos(x,y,worldsize);
		theChoosenOne = nPos % numbPossible;
		/*printf("\n\ntheChoosenOne: %d calcPos:%d  numbPossible: %d calcPos(x,y,worldsize)MODnumbPossible: %d \n\n",theChoosenOne,calcPos(x,y,worldsize),numbPossible, calcPos(x,y,worldsize)%numbPossible);
		 printf("[%d , %d , %d , %d]\n\n",vec[0],vec[1],vec[2],vec[3]);*/
		if (type == WOLF || type == WES)
			if (numbPossibleWolf > 0) {
				for (i = 0; i < POSSIBLE_POS; i++) {
					if (vecWolf[i] != -1) {
						if (theChoosenOne == 0) {
							ret = vecWolf[i];
							return ret;
						} else {
							theChoosenOne--;
						}
					}
				}
			} else {
				for (i = 0; i < POSSIBLE_POS; i++) {
					if (vec[i] != -1) {
						if (theChoosenOne == 0) {
							ret = vec[i];
							return ret;
						} else {
							theChoosenOne--;
						}
					}
				}
			}
		else {
			for (i = 0; i < POSSIBLE_POS; i++) {
				if (vec[i] != -1) {
					if (theChoosenOne == 0) {
						ret = vec[i];
						return ret;
					} else {
						theChoosenOne--;
					}
				}
			}
		}

		return ret;
	}
	void goAnimal(sworld world_init, sworld world_final, int pos, int type) {
		int y_init, x_init, y_final, x_final, posFinal;
		calcCords(pos, &x_init, &y_init);

		posFinal = /* (2 < rand()%5)?pos+1:pos+worldsize;*/calcMovePos(
				world_init, x_init, y_init, type);
		/*posFinal = posFinal<(worldsize*worldsize)? posFinal: posFinal%(worldsize*worldsize);*/
		if (posFinal < 0)
			return;

		/*	printf("posição: %d final: %d\n",pos, posFinal);*/
		calcCords(posFinal, &x_final, &y_final);
		move(world_init, x_init, y_init, world_final, x_final, y_final);
	}
