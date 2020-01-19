#!/bin/bash

INPUT_FILENAME=$1


BASENAME=${INPUT_FILENAME##*/}
BASENAME=${BASENAME%.*}



PSM=( "0" "1" "2" "3" "4" "5" "6" "7" "8" "9" "10" "11" "12" "13" )
 
for i in "${PSM[@]}"
do
	OUTPUT_FILENAME=$BASENAME
	OUTPUT_FILENAME+="_result_psm"$i
	
	echo "--- Processing for page segmentation mode: $i ---"
	tesseract $1 $OUTPUT_FILENAME --psm $i
	echo ""
	
done

echo "All done"

