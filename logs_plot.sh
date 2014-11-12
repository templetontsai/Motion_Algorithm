#!/bin/bash


cat logs | grep r | awk ' { print (($7 + $8) - ($5 + $6)), (($5 + $8) - ($6 + $7))}' > plot_r
cat logs | grep k | awk ' { print (($7 + $8) - ($5 + $6)), (($5 + $8) - ($6 + $7))}' > plot_k
cat logs | grep n | awk ' { print (($7 + $8) - ($5 + $6)), (($5 + $8) - ($6 + $7))}' > plot_n

gnuplot plot.plt
