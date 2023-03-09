#!/bin/bash
#SBATCH -p RM
#SBATCH -t 00:05:00
#SBATCH -N 1
#SBATCH --ntasks-per-node 128
set -x
cd ..
make clean
gcc -pthread -I/ocean/projects/see200002p/shared/include -I../../../../372-2022F/code/include -std=c18 -pedantic -Wall -L/ocean/projects/see200002p/shared/lib -L../../../../372-2022F/code/lib -O3 -Wno-missing-braces -o galaxy2_par.exec nbody_par.c galaxy2.c -lanim -lm
./galaxy2_par.exec 128 galaxy2_par.anim
../../../../372-2022F/code/bin/anim2mp4 galaxy2_par.anim -o galaxy2_par.mp4
