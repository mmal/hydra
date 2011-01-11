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

    FIRSTLINE=$(head -n 1 $f)
    t=${FIRSTLINE:5}  

    echo t=$t file=$f frame=$frame

    output="movie/m_$frame.png"	# output file name

    echo "set output \"$output\"" >> plotter.gp
    
    cat $f >> plotter.gp

    # 	echo "t = $t"		 # some output for user
    # 	# echo "$cmd" > plotter.gp # echo the command to the temporary file
    ./snapshot_to_png_template.gp # execute the template (it loads the temporary file)
    # fi
    

done

./make_movie.sh
#rm movie/*.png
mplayer -loop 0 -speed 0.8 movie.avi
