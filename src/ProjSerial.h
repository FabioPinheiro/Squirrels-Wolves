#ifndef PROJ_SERIAL_H
#define PROJ_SERIAL_H

#define EPTY 0
#define WOLF 1
#define SQRL 2
#define ICE 3
#define TREE 4
#define SONT 6 //squirrel on a tree
#define printInt(i) printf("%d\n",i)
#define calcPos(x, y, worldsize) y + x*worldsize

struct world {
	int type; /* Wolf, Squirrel, etc. */
 	int breeding_period;
 	int starvation_period;
 };


typedef struct world *sworld;



extern int worldsize, wolfBP, sqrlBP, wolfStarvP, genNum;

#endif /* PROJ_SERIAL_H */
