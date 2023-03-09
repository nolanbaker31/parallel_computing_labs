#!/bin/bash
#SBATCH -p GPU-shared
#SBATCH -t 00:01:00
#SBATCH -N 1
#SBATCH --ntasks-per-node 3
#SBATCH --gpus=1
# echo commands to stdout
set -x
./planets-elliptical.exec out.anim
