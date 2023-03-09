#!/bin/bash
#SBATCH -p RM-shared
#SBATCH -t 00:05:00
#SBATCH -N 1
#SBATCH --ntasks-per-node 2
set -x
cd ..
mpirun -np 2 /jet/home/nolanb/372-nolanb/hw/hw3/perfect/perfect_mpi.exec 10000000
