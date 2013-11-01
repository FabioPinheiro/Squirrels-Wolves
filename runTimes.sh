#!bin/bash
make cleanA
make all;
make utils;
n=4;
it=3;
OMP_NUM_THREADS=$n;
echo $n;
for((i=0;i<10;i++)); 
do 

	for((i=0;i<$it;i++)); do make runP; done

	for((i=0;i<$it;i++)); do make runS; done

	make runUtils

done