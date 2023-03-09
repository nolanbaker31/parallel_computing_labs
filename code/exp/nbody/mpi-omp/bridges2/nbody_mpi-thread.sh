#!/bin/bash
#SBATCH -p RM
#SBATCH -t 00:01:00
#SBATCH -N 2
#SBATCH --ntasks-per-node 5
# echo commands to stdout
set -x
OMP_NUM_THREADS=5 mpirun -np 2 ./planets-elliptical.exec out.anim
