#Makefile
SRC=src/
PARALLEL_CFLAGS=-g -Wall -ansi -pedantic -fopenmp
SERIAL_CFLAGS=-g -Wall -ansi -pedantic -fopenmp

make all: parallel serial

parallel:
	gcc $(PARALLEL_CFLAGS) -o parallel_proj $(SRC)ProjParallel.c $(SRC)Mover.c
serial:
	gcc $(SERIAL_CFLAGS) -o serial_proj $(SRC)ProjSerial.c $(SRC)Mover.c

utils:
	gcc -o utils $(SRC)Utils.c

#Paralelo
runP:
	./parallel_proj inputs/input3000 2 2 6 200 POutput.out
runSmalP:
	./parallel_proj inputs/smalInput 10 12 10 1 PSmalOutput.out
runUP:
	./utils POutput.out
	
#Em serie		
runS:
	./serial_proj inputs/input3000 2 2 6 200 SOutput.out
runSmal:
	./serial_proj inputs/smalInput 10 12 10 1 SmalOutput.out
runUtils:
	./utils timeOut.out
runF:
	gcc $(CFLAGS) -o fabio_open_file_test1 $(SRC)fabio_open_file_test1.c;
	./fabio_open_file_test1 inputs/input
	rm fabio_open_file_test1


cleanS:
	rm -f serial_proj
cleanP:
	rm -f parallel_proj
clean:
	rm -f serial_proj parallel_proj
cleanA:
	rm -f *proj *.out
