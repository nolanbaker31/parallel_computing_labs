set terminal pdf  # size 4, 4
set output "perfect_mpi.pdf"
set xlabel center "Number of processes"
set ylabel center "time (seconds)"
set xr [1:35]
set yr [0:90]
plot "perfect_mpi.dat" using 1:2 title 'MPI' with linespoints
