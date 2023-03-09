#!/bin/bash
#SBATCH -p RM-shared
#SBATCH -t 00:05:00
#SBATCH -N 1
#SBATCH --ntasks-per-node 16
set -x
cd ..
srun --unbuffered -n 1 ./mandelbrot.exec 800     400 mb4_seq.anim
