#!/bin/bash
#SBATCH -p GPU-shared
#SBATCH -t 00:05:00
#SBATCH -N 1
#SBATCH --gres=gpu:1
#SBATCH --ntasks-per-node 4
set -x
cd ..
srun --unbuffered -n 1 ./mandelbrot_cuda.exec 800     400 mb4_seq.anim
