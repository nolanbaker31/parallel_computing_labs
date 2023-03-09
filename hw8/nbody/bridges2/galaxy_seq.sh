#!/bin/bash
#SBATCH -p RM-shared
#SBATCH -t 00:05:00
#SBATCH -N 1
#SBATCH --ntasks-per-node 16
set -x
cd ..
gcc -I/ocean/projects/see200002p/shared/include -I../../../../372-2022F/code/include -std=c18 -pedantic -Wall -L/ocean/projects/see200002p/shared/lib -L../../../../372-2022F/code/lib -O3 -Wno-missing-braces -o galaxy_seq.exec nbody.c galaxy.c -lanim -lm
./galaxy_seq.exec galaxy_seq.anim
