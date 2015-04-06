#!/usr/bin/gnuplot

#set multiplot layout 1,2
set term png 
set output "FFT_vs_naive_log.png"
set logscale y
set title "Naive vs FFT polynomial multiplication"
set xlabel "Polynomial lenght"
set ylabel "Execution time (µs)"
set ytics "4"
set format y "%10.0f"
set grid ytics lt 0 lw 1 lc rgb "#bbbbbb"
set grid xtics lt 0 lw 1 lc rgb "#bbbbbb"
plot "measures.txt" using 1:2 with lines title "Naive" lw 2, \
     "measures.txt" using 1:3 with lines title "FFT" lw 2  

set output "FFT_vs_naive.png"
unset logscale y
set ytics "100000"
plot "measures.txt" using 1:2 with lines title "Naive" lw 2, \
     "measures.txt" using 1:3 with lines title "FFT" lw 2  

#unset multiplot
