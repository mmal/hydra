#!/usr/local/bin/pyxplot

set terminal png

set nokey

set size 4 square

set zrange [-2:2]

set xlabel "$x$"

set ylabel "$y$"

set zlabel "$u(t,r)$"

set xtics 2

set ytics 2

set grid

load "plotter.gp"
