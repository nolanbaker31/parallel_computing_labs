set terminal pdf  # size 4, 4
# set tmargin at screen 0.9
# set lmargin at screen 0.1
# set rmargin at screen -0.1
# set bmargin at screen 0.1
# set size ratio 1.2
set output "sat_mpi.pdf"
# unset log
# unset label
# set key vertical top left 
set xlabel center "Number of processes"
set ylabel center "time (seconds)"
# set logscale y
# set format y "10^{%T}"
# set xtics 1, 1, 10
# set ytics 
set xr [0:32]
set yr [0:45]
plot "sat_mpi.dat" using 1:2 title 'MPI' with linespoints

set output "sat_speedup.pdf"
set xlabel center "Number of processes"
set ylabel center "speedup"
set xr [0:32]
set yr [0:32]
first(x) = ($0 > 0 ? base : base = x)
plot "sat_mpi.dat" using 1:(first($2), base/$2) title 'Speedup' with linespoints

set output "sat_efficiency.pdf"
set xlabel center "Number of processes"
set ylabel center "efficiency"
set xr [0:32]
set yr [0:1]
first(x) = ($0 > 0 ? base : base = x)
plot "sat_mpi.dat" using 1:(first($2), base/($2*$1)) title 'Efficiency' with linespoints
