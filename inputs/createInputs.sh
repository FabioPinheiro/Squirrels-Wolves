#!/bin/bash

#Change N to define the dimension of the matrix.
n=5000
FILENAME="input$n"
rm $FILENAME


echo "$n" >> "$FILENAME"
for (( h = 0; h < "$n"; h++ ));
do
	for((j=0;j<"$n";j++));
	do
		if ((h+j%11 == 0)); then
			#statements tree
			echo "$h $j t" >> "$FILENAME";
		elif ((h+j%17== 0)); then
			#statements ice
			echo "$h $j i" >> "$FILENAME";
		elif ((j%11 == 0)); then
			#statements wolf
			echo "$h $j w" >> "$FILENAME";
		elif ((j%7 == 0)); then
			#statements squirrel
			echo "$h $j s" >> "$FILENAME";
		fi
	done;
done
