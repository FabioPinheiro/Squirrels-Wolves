#!bin/bash
make cleanA
make all;
make utils;

for((i=0;i<5;i++)); do make runP; done

for((i=0;i<5;i++)); do make runS; done

make runUtils
