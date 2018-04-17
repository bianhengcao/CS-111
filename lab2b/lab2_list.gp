#! /usr/bin/gnuplot
#

# general plot parameters
set terminal png
set datafile separator ","

set title "List-1: Total operations per second"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Operations per second"
set logscale y
set output 'lab2b_1.png'
set key left top
plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'list w/mutex' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'list w/spin-lock' with linespoints lc rgb 'green'

set title "List-2: Total operations per second"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Nanoseconds"
set output 'lab2b_2.png'
set key left top
plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($7) \
        title 'completion time' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($8) \
        title 'waiting time' with linespoints lc rgb 'green'

set title "List-3: Sublists with yielding that run without failure"
unset logscale x
set xrange [0:16]
set xlabel "Threads"
set ylabel "successful iterations"
set logscale y 10
set output 'lab2b_3.png'
plot \
    "< grep 'list-id-none,[0-9]*,[0-9]*,4,' lab2b_list.csv" using ($2):($3) \
       with points lc rgb "blue" title "unprotected", \
    "< grep 'list-id-m,[0-9]*,[0-9]*,4,' lab2b_list.csv" using ($2):($3) \
       with points lc rgb "green" title "mutex", \
    "< grep 'list-id-s,[0-9]*,[0-9]*,4,' lab2b_list.csv" using ($2):($3) \
       with points lc rgb "red" title "spin-lock"

set title "List-4: Total operations per second for Mutex Protected Sublists"
unset xrange
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Operations per second"
set output 'lab2b_4.png'
set key left top
plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
        title '--lists=1' with linespoints lc rgb 'red', \
     "< grep -e 'list-none-m,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000/($7)) \
        title '--lists=4' with linespoints lc rgb 'orange', \
     "< grep -e 'list-none-m,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000/($7)) \
        title '--lists=8' with linespoints lc rgb 'yellow', \
     "< grep -e 'list-none-m,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000/($7)) \
        title '--lists=12' with linespoints lc rgb 'green'

set title "List-5: Total operations per second for Spin-lock Protected Sublists"
unset xrange
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Operations per second"
set output 'lab2b_5.png'
set key left top
plot \
     "< grep -e 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
        title '--lists=1' with linespoints lc rgb 'red', \
     "< grep -e 'list-none-s,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000/($7)) \
        title '--lists=4' with linespoints lc rgb 'orange', \
     "< grep -e 'list-none-s,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000/($7)) \
        title '--lists=8' with linespoints lc rgb 'yellow', \
     "< grep -e 'list-none-s,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000/($7)) \
        title '--lists=12' with linespoints lc rgb 'green'