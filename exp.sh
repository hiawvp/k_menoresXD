#!/bin/bash

#cambiar n, b, k 
# b -> {128, 1024, 4096}
# n -> 10^8
# seg = j-i = 100
# k -> {1,2,4,8,16,32}
# 

bs=""
bs="${bs} 128"
bs="${bs} 1024"
bs="${bs} 4096"

: ${n:=100000000}
: ${maxk:=32}
: ${i=10}
: ${j=110}
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

FILE="./$output""_exp.json"
echo "Output file: $FILE"
> $FILE
#echo [ > $FILE
variable=[
for b in ${bs}; 
#for (( b=1; b<=3; b++ ))
do
    echo "current method : $b"
    for (( k=1; k<=$maxk; k = k*2 ))
    do
			echo $variable >> $FILE
			variable="$(./main $n $b $i $j $k  ),"
			#variable="$(./prog $x $threads $m $chunksize),"
    done
done
withoutcomma=${variable::-1}
echo $withoutcomma >> $FILE
echo ] >> $FILE