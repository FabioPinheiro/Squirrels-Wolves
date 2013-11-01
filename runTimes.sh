#!bin/bash
make cleanA
make all;
make utils;
n=4;
it=3;
OMP_NUM_THREADS=$n;
echo $n;
for((j=0;j<2;j++)); 
do 
	OMP_NUM_THREADS=4;
	rm timeOut.out
	for((i=0;i<$it;i++)); do make runP; done
	make runUtils
	
	OMP_NUM_THREADS=8;
	rm timeOut.out
	for((i=0;i<1;i++)); do make runP; done
	make runUtils

done