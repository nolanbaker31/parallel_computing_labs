# Config file for macOS with clang 9 installed by MacPorts.
# Copy this file to config.mk in this directory and edit if needed.
# Install MacPorts following instructions here: https://www.macports.org/install.php
# Then execute:
# sudo port install clang-9.0 mpich-clang90 gd2 ffmpeg gnuplot

# Shell used to execute recipes
SHELL = /bin/bash

# Preprocessor flags for all compilers
CPPFLAGS = -I/opt/local/include
# Compilation flags for all compilers
CFLAGS = -std=c17 -pedantic -Wall
# Linking flags for all compilers
LDFLAGS = -L/opt/local/lib

# The compiler for sequential C programs
#CC = cc
CC = clang-mp-9.0

# C/MPI compiler
#MPICC = mpicc-mpich-clang90
MPICC = mpicc

# Pthread compiler
PTHREADCC = $(CC) -pthread

# C/OpenMP compiler
#OMPCC = gcc-mp-9 -fopenmp
#OMPCC = cc -fopenmp
OMPCC = clang-mp-9.0 -fopenmp -isystem /opt/local/include/libomp

# CUDA-C compiler (possibly including MPI).  Empty means don't do CUDA.
# NVCC = nvcc --compiler-bindir mpicc
NVCC =
# C/MPI/OpenMP compiler
MPIOMPCC = $(MPICC) -fopenmp -isystem /opt/local/include/libomp

# Command to run a sequential program
RUN =
# Command to run an MPI program
#MPIRUN = mpiexec-mpich-clang90
MPIRUN = mpiexec
# Command to run a Pthread program
PTHREADRUN =
# Command to run a CUDA program
CUDARUN =
# Command to run an OpenMP program
OMPRUN =
# Command to run an MPI/OpenMP hybrid program
#MPIOMPRUN = mpiexec-mpich-clang90
MPIOMPRUN = mpiexec
