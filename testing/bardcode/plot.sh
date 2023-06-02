#!/bin/bash

./a.out $1 > 1.txt

gnuplot <<-EOF
set terminal png font arial 14 size 1800,100
set output "$2"
set xrange [0:400]
set yrange [-1:1.5]
plot '1.txt' with lines
EOF
