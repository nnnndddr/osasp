#!/bin/bash
fileSave=$2
directory=$1

if [ "$#" != 2 ]; then 
	echo "problem with parametrs"
	echo "first parametr - directory for search" 2> errors
	echo "second parametr - file for saving result" 2> errors
	exit 0
fi 

if [[ ! -d "$directory" || ! -f "$fileSave" ]]; then
	if [[ ! -d "$directory" ]]; then 
		echo "Problem with file for saving" 2> errors
		echo "first parametr - directory for search" 2> errors
	fi

	if [[ ! -f "$fileSave" ]]; then 
		echo "Problem with directory" 2> errors
		echo "second parametr - file for saving result" 2> errors
	fi	
	exit 0	
fi


for i in $( find "$directory" -type d); do
	amountSize=$( du -b -h --summarize "$i" | cut -f1 )
	amountFiles=$( find "$i" -maxdepth 1 -type f | wc -l )
	echo "Directory: $i Size: $amountSize Value: $amountFiles" >> "$fileSave"

done

exit 0
