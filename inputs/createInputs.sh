#!/bin/bash
n=100
i=0
echo "$n" > inputShell100
for (( h = 0; h < 100; h++ ));
do
	for((j=0;j<100;j++));
	do
		if ((h+j%11 == 0)); then
			#statements tree
			echo "$h $j 't'" >> inputShell100;
		elif (( h+j%17== 0)); then
			#statements ice
			echo "$h $j 'i'" >> inputShell100;
		elif ((j%3 == 0)); then
			#statements wolf
			echo "$h $j 'w'" >> inputShell100;
		elif ((j%2 == 0)); then
			#statements squirrel
			echo "$h $j 's'" >> inputShell100;
		fi
		
	done;
done
