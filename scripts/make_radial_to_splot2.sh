#!/bin/bash

N=0
PI=3.1415
Nphi=4; 


filein=$1
fileout=$2


tempfile=tmp

sort -n $filein > $tempfile

echo "" > $fileout # reset output file


while read R U # read two variables
do
    N=$(($N+1| bc))

    if [ $R != '#' ] 
    then
	if [ $N -lt 40 ]
	then
	    for ((a=0; a <= $Nphi ; a++))
	    do
		phi=$(echo $a ${Nphi} ${PI} | awk '{printf "%f\n", $1/$2*$3/2.0 }')
	# echo $phi
		X=$(echo ${phi} ${R} | awk '{printf "%.16e\n", $2*cos($1) }')
		Y=$(echo ${phi} ${R} | awk '{printf "%.16e\n", $2*sin($1) }')
		echo ${X} ${Y} ${U} >> $fileout
	    done
	else 
	    break
	fi
    fi

done < $tempfile
