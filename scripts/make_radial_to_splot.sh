#!/bin/bash

N=0
PI=3.1415
Nphi=10;


filein=$1
fileout=$2

while read R U # read three variables
do
    if [ $R != '#' ]
    then
        Rarray[$N]=$R
        Uarray[$N]=$U
        N=$(($N+1| bc))
    fi
done < $filein


echo "" > $fileout # reset output file


for i in {1..20}
do
    for ((a=0; a <= $Nphi ; a++))
    do
	phi=$(echo $a ${Nphi} ${PI} | awk '{printf "%f\n", $1/$2*$3/2.0 }')
	# echo $phi
	X=$(echo ${phi} ${Rarray[$i]} | awk '{printf "%.16e\n", $2*cos($1) }')
	Y=$(echo ${phi} ${Rarray[$i]} | awk '{printf "%.16e\n", $2*sin($1) }')
	echo ${X} ${Y} ${Uarray[$i]} >> $fileout
    done
done
