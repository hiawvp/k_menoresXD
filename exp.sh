#!/bin/bash

#cambiar n, b, k 

: ${size:=1024*6}
#: ${threads:=4}
#: ${chunksize:=1}
: ${iters:=5}
: ${output:=output}
#while getopts n:t:c:i:o: flag
while getopts n:i:o: flag
do
    case "${flag}" in
        n) size=${OPTARG};;
        #t) threads=${OPTARG};;
        #c) chunksize=${OPTARG};;
        i) iters=${OPTARG};;
        o) output=${OPTARG};;
    esac
done

FILE="./$output""_exp1.json"
echo "Output file: $FILE"
> $FILE
#echo [ > $FILE
variable=[
for (( m=1; m<=3; m++ ))
do

    echo "current method : $m"
    for (( x=1024; x<=$size; x=x*2 ))
    do

        for (( i=1; i<=$iters; i++ ))
        do
            echo $variable >> $FILE
            variable="$(./prog $x $m ),"
            #variable="$(./prog $x $threads $m $chunksize),"
        done

    done

done
withoutcomma=${variable::-1}
echo $withoutcomma >> $FILE
echo ] >> $FILE