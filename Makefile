#Makefile
SRC=src/
CFLAGS=-g


serial:
	gcc $(CFLAGS) -o serial_proj $(SRC)ProjSerial.c $(SRC)Mover.c
	
runS:
	./serial_proj inputs/input 10 12 3 5
	
make all: serial runS

clean:
	rm serial_proj