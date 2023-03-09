# This is a default config file that will work for most UNIX-like systems.
# It will be used by default, if no file named config.mk is found in this
# directory.  If you want to make changes, copy this file to config.mk
# and edit config.mk.

# Shell used to execute recipes
SHELL = /bin/bash

# Preprocessor flags for all compilers
CPPFLAGS =
# Compilation flags for all compilers
CFLAGS = -std=c18 -pedantic -Wall
# Linking flags for all compilers
LDFLAGS =

# The compiler for sequential C programs
CC = cc
# C/MPI compiler
MPICC = mpicc
# Pthread compiler
PTHREADCC = $(CC) -pthread
# C/OpenMP compiler
OMPCC = $(CC) -fopenmp
# CUDA-C compiler (possibly including MPI).  Empty means don't do CUDA.
NVCC = nvcc --compiler-bindir mpicc
# C/MPI/OpenMP compiler
MPIOMPCC = $(MPICC) -fopenmp

# Command to run a sequential program
RUN =
# Command to run an MPI program
# Add --oversubscribe for OpenMPI if you want more MPI procs than cores
MPIRUN = mpiexec
# Command to run a Pthread program
PTHREADRUN =
# Command to run a CUDA program
CUDARUN =
# Command to run an OpenMP program
OMPRUN =
# Command to run an MPI/OpenMP hybrid program
MPIOMPRUN = $(MPIRUN)
