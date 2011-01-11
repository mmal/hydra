#!/bin/sh

ls -1 movie/*.png|sort -t'_' -k2n > filelist.txt
mencoder -ovc lavc -lavcopts vbitrate=800 -lavcopts threads=4 -mf type=png:fps=10 -o movie.avi mf://@filelist.txt
