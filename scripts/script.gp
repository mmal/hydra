#set terminal postscript eps enhanced color solid 20

set terminal x11 persist

# set border linewidth 2.4
set nokey
set size 1.,0.55
set origin 0,0

set ylabel "u(t, x)"
set xlabel ""
set format x ""

set multiplot

set size 0.85,0.55

set rmargin 0 
set lmargin 0 
set tmargin 0 
set bmargin 0

set origin 0.1,0.35
plot "data.dat" u 1:2 with points lc rgb "#ff6666" ti ""


# plot of the grids location

set size 0.85,0.25 # size of plot
set rmargin 0 #
set lmargin 0 # set
set tmargin 0 #  margins
set bmargin 0 #

set origin 0.1,0.1 # set plot location

unset title
unset ylabel # unset previous
unset xlabel #  label settings
set xlabel "x" # set xlabel name
set ylabel "l" # set ylabel name
set format x "% g" # set default fomat for the x axis
set ytics 0,1,5 # set ytics from 0 to l, step 1

set key
set grid x front
set boxwidth 0.7
set style fill solid 1.0 noborder


# plot grids location, yrange [:l+1]
plot [][:6] "position.dat" u 2:3 w filledcurve x1  lt 1 lc rgb "#542222"  ti "gh"
replot "position.dat" u 1:3 w filledcurve x1  lt 1 lc rgb "#a84343" ti "no gh"

unset multiplot

