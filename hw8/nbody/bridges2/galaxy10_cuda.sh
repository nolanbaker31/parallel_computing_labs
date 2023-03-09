#!/bin/bash
#SBATCH -p GPU-shared
#SBATCH -t 00:05:00
#SBATCH -N 1
#SBATCH --gres=gpu:1
#SBATCH --ntasks-per-node 4
set -x
cd ..
nvcc -I/ocean/projects/see200002p/shared/include -I../../../../372-2022F/code/include -L/ocean/projects/see200002p/shared/lib -L../../../../372-2022F/code/lib --compiler-options -Wno-missing-braces --compiler-options -fopenmp -fmad false -o galaxy10_par.exec nbody_par.cu galaxy10.c -lanim -lm

OMP_NUM_THREADS=4 srun --unbuffered -n 1 --gres=gpu:1 ./galaxy10_par.exec 4 galaxy10_par.anim
