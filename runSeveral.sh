#!bin/bash
make cleanA
make all;
make utils;
n=4;

OMP_NUM_THREADS=$n;
echo $n;
for((i=0;i<1;i++)); do make runP; done

for((i=0;i<1;i++)); do make runS; done

make runUtils
