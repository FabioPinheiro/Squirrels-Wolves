#!bin/bash
make cleanA
make all;
make utils;
n=1;

OMP_NUM_THREADS=$n;
echo $n;
for((i=0;i<1;i++)); do make runP; done

for((i=0;i<1;i++)); do make runS; done

make runUtils

if diff ParallelOut.out SerialOut.out > diff.out; then
    echo "Success Matrix are equal"
else
    echo "Fail See diff.out"
fi
