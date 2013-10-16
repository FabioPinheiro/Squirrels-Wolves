#Makefile
SRC=src/
CFLAGS=

serial:
	gcc $(CFLAGS) -o serial-proj $(SRC)ProjSerial.c
	
runS:
	./serial-proj inputs/input 10 12 3 5