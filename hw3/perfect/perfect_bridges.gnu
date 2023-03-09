set terminal pdf  # size 4, 4
set output "perfect_bridges.pdf"
set xlabel center "Number of processes"
set ylabel center "time (seconds)"
set xr [1:128]
set yr [0:140]
plot "bridges.dat" using 1:2 title 'Processes vs time on bridges' with linespoints
