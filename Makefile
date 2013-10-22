#Makefile
SRC=src/
CFLAGS=-g -Wall -ansi -pedantic #-fopenmp


serial:
	gcc $(CFLAGS) -o serial_proj $(SRC)ProjSerial.c $(SRC)Mover.c
	
runS:
	./serial_proj inputs/input 10 12 3 1
runSmal:
	./serial_proj inputs/smalInput 10 12 3 2
		
make all: serial

clean:
	rm -f serial_proj