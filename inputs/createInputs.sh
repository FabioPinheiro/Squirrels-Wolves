#!/bin/bash
n=10
i=0
FILENAME="inputShelltest"
echo rm $FILENAME

echo "$n" >> "$FILENAME"
for (( h = 0; h < "$n"; h++ ));
do
	for((j=0;j<"$n";j++));
	do
		if ((h+j%11 == 0)); then
			#statements tree
			echo "$h $j 't'" >> "$FILENAME";
		elif (( h+j%17== 0)); then
			#statements ice
			echo "$h $j 'i'" >> "$FILENAME";
		elif ((j%3 == 0)); then
			#statements wolf
			echo "$h $j 'w'" >> "$FILENAME";
		elif ((j%2 == 0)); then
			#statements squirrel
			echo "$h $j 's'" >> "$FILENAME";
		fi
		
	done;
done
