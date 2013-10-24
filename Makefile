#Makefile
SRC=src/
PARALLEL_CFLAGS=-g -Wall -ansi -pedantic -fopenmp
SERIAL_CFLAGS=-g -Wall -ansi -pedantic

make all: parallel serial

parallel:
	gcc $(PARALLEL_CFLAGS) -o parallel_proj $(SRC)ProjParallel.c $(SRC)Mover.c
serial:
	gcc $(SERIAL_CFLAGS) -o serial_proj $(SRC)ProjSerial.c $(SRC)Mover.c

runP:
	./parallel_proj inputs/input 10 12 10 4 POutput
runSmalP:
	./parallel_proj inputs/smalInput 10 12 10 1 PSmalOutput
		
runS:
	./serial_proj inputs/input 10 12 10 4 SOutput
runSmal:
	./serial_proj inputs/smalInput 10 12 10 1 SmalOutput
runF:
	gcc $(CFLAGS) -o fabio_open_file_test1 $(SRC)fabio_open_file_test1.c;
	./fabio_open_file_test1 inputs/input
	rm fabio_open_file_test1

cleanS:
	rm -f serial_proj
cleanP:
	rm -f parallel_proj
clean:
	rm -f serial_proj