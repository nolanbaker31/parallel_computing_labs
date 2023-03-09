#!/bin/bash
#SBATCH -p RM-shared
#SBATCH -t 00:05:00
#SBATCH -N 1
#SBATCH --ntasks-per-node 64
set -x
cd
mpirun -np 64 /jet/home/nolanb/372-nolanb/hw/hw4/advect2d/advect2d_mpi.exec
