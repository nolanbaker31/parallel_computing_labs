#!/bin/bash
#SBATCH -p RM
#SBATCH -t 00:05:00
#SBATCH -N 1
#SBATCH --ntasks-per-node 128
set -x
cd ..
mpirun -np 128 /jet/home/nolanb/372-nolanb/hw/hw3/perfect/perfect_mpi.exec 50000000
