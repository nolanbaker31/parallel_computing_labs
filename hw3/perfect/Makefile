all: perfect_mpi.exec run

perfect_mpi.exec: perfect_mpi.c
	mpicc -o perfect_mpi.exec perfect_mpi.c

run: perfect_mpi.exec
	srun -n 1 ./perfect_mpi.exec 10000000 #> perfect_mpi.dat
	srun -n 5 ./perfect_mpi.exec 10000000 #>> perfect_mpi.dat
	srun -n 10 ./perfect_mpi.exec 10000000 #>> perfect_mpi.dat
	srun -n 15 ./perfect_mpi.exec 10000000 #>> perfect_mpi.dat
	srun -n 20 ./perfect_mpi.exec 10000000 #>> perfect_mpi.dat
	srun -n 25 ./perfect_mpi.exec 10000000 #>> perfect_mpi.dat
	srun -n 30 ./perfect_mpi.exec 10000000 #>> perfect_mpi.dat
	srun -n 35 ./perfect_mpi.exec 10000000 #>> perfect_mpi.dat

plot: perfect_mpi.dat perfect_mpi.gnu
	gnuplot perfect_mpi.gnu

plotbridges: bridges.dat perfect_bridges.gnu
	gnuplot perfect_bridges.gnu

clean:
	rm perfect_mpi.exec
