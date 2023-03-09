# Config file for bridges.  Note that runs using this configuration
# (e.g., make test) will only work in interactive mode.  It is
# recommended to use batch mode for larger or longer-running jobs.

# Shell used to execute recipes
SHELL = /bin/bash

# Preprocessor flags for all compilers
CPPFLAGS = -I/ocean/projects/see200002p/shared/include
# Compilation flags for all compilers
CFLAGS = -std=c18 -pedantic -Wall
# Linking flags for all compilers
LDFLAGS = -L/ocean/projects/see200002p/shared/lib

# The compiler for sequential C programs
#CC = cc
CC = gcc
# C/MPI compiler
#MPICC = mpiicc  # Intel compiler
MPICC = mpicc
# Compile a Pthread program
PTHREADCC = $(CC) -pthread
# C/OpenMP compiler
OMPCC = $(CC) -fopenmp
# CUDA-C compiler (possibly including MPI)
# NVCC = nvcc --compiler-bindir mpicc
NVCC = nvcc
# C/MPI/OpenMP compiler
MPIOMPCC = $(MPICC) -fopenmp
# CUDA/OpenMP compiler
CUDAOMPCC = nvcc --compiler-options -fopenmp
# MPI/CUDA compiler
MPICUDACC = nvcc --compiler-bindir mpic++
# MPI/CUDA/OpenMP compiler
MPICUDAOMPCC = nvcc --compiler-options -fopenmp --compiler-bindir mpic++
# MPI/Pthread compiler
MPIPTHREADCC = $(MPICC) -pthread
# CUDA/Pthread compiler
CUDAPTHREADCC = nvcc --compiler-options -pthread

# Use batch scripts for Bridges-2

# Command to run a sequential program
# RUN = srun -n 1 -p RM-shared
RUN =
# Command to run an MPI program
MPIRUN = mpirun
# Command to run a Pthread program
PTHREADRUN =
# Command to run a CUDA program: work on this
CUDARUN =
# Command to run an OpenMP program
OMPRUN =
# Command to run an MPI/OpenMP hybrid program
MPIOMPRUN =
