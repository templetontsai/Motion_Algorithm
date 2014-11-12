set terminal png
set output 'Raw.png'
plot "plot_r" index 0 using 1 with lines title "Raw_x", "plot_r" index 0 using 2 with lines title "Raw_y"
set terminal png
set output 'Kalm.png'
plot "plot_k" index 0 using 1 with lines title "Kalm_x", "plot_k" index 0 using 2 with lines title "Kalm_y"

set terminal png
set output '9960.png'
plot "plot_n" index 0 using 1 with lines title "9960_x", "plot_n" index 0 using 2 with lines title "9960_y"
