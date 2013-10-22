#Makefile
SRC=src/
CFLAGS=-g -Wall -ansi -pedantic


serial:
	gcc $(CFLAGS) -o serial_proj $(SRC)ProjSerial.c $(SRC)Mover.c
	
runS:
	./serial_proj inputs/input 10 12 3 5
runSmal:
	./serial_proj inputs/smalInput 10 12 3 3
		
make all: serial

clean:
	rm -f serial_proj