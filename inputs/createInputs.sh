#!/bin/sh
n=1000

echo "$n" > inputShell
for (( h = 0; h < 10; h++ ));
do
	for((j=0;j<1000;j++));
	do
		printf "%d %d %c\n" "$h $j t" >> inputShell;
	done;
done
