#!/bin/bash
#SBATCH -p RM-shared
#SBATCH -t 00:05:00
#SBATCH -N 1
#SBATCH --ntasks-per-node 1
set -x
cd ..
./wavemaker1d.exec 20000000 10000 1000 0.005 1000 1000 wave_seq.out
