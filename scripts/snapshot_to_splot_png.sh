#!/bin/bash


name="list_of_scr"
path=""
frame=0case 
# files=$(find $path -name "$name" |sort -t'_' -k3n)


files=$(cat $name)

mkdir -p movie
rm -f movie/*.png

for f in $files; do

    frame=$(echo $frame | awk '{printf "%010d\n", $1+1}') 

    echo "" > plotter.gp	# clear the temporary file

    #get time
    FIRSTLINE=$(head -n 1 $f)
    t=${FIRSTLINE:5}  

    #get file with data
    FIRSTLINE=$(grep plot $f)
    data=${FIRSTLINE:20:10}

    ./make_radial_to_splot2.sh $data test.dat

    echo t=$t file=$f frame=$frame data=$data

    output="movie/m_$frame.png"	# output file name

    echo "set output \"$output\"" >> plotter.gp

    echo "set title \"Spherically symetric wave equation\\ at time t=$t\"" >> plotter.gp

    echo "plot 3d \"test.dat\" u 1:2:3 with surface fillc rgb(1-\$3/2):(1-\$3/2):(1.-\$3/10)" >> plotter.gp

    # 	echo "t = $t"		 # some output for user
    # 	# echo "$cmd" > plotter.gp # echo the command to the temporary file
    ./snapshot_to_splot_png_template.gp # execute the template (it loads the temporary file)
    # fi
    

done

./make_movie.sh
#rm movie/*.png
mplayer -loop 0 -speed 0.8 movie.avi
