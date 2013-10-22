#Makefile
SRC=src/
CFLAGS=-g -Wall -ansi -pedantic #-fopenmp


serial:
	gcc $(CFLAGS) -o serial_proj $(SRC)ProjSerial.c $(SRC)Mover.c
	
runS:
	./serial_proj inputs/input 10 12 3 1
runSmal:
	./serial_proj inputs/smalInput 10 12 3 2
runF:
	gcc $(CFLAGS) -o fabio_open_file_test1 $(SRC)fabio_open_file_test1.c;
	./fabio_open_file_test1 inputs/input
	rm fabio_open_file_test1
	
make all: serial runS clean

clean:
	rm -f serial_proj