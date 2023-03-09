#!/bin/bash
#SBATCH -p GPU
#SBATCH -t 00:01:00
#SBATCH -N 2
#SBATCH --ntasks-per-node 5
#SBATCH --gpus=16
# echo commands to stdout
set -x
OMP_NUM_THREADS=5 mpirun -np 2 ./planets-elliptical.exec out.anim
