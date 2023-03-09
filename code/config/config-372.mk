# Config file for cisc372

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
# Compile a Pthread program
PTHREADCC = $(CC) -pthread
# C/OpenMP compiler
OMPCC = $(CC) -fopenmp
# CUDA-C compiler.  Note this no longer works:  nvcc --compiler-bindir mpicc
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

# Command to run a sequential program
RUN = srun --unbuffered -n 1
# Command to run an MPI program
MPIRUN = srun --unbuffered
# Command to run a Pthread program
PTHREADRUN = srun --unbuffered -n 1 -c $(NCORES)
# Command to run a CUDA program
CUDARUN = srun --unbuffered -n 1 --gres=gpu:1
# Command to run an OpenMP program
OMPRUN = srun --unbuffered -n 1 -c $(NCORES)
# Command to run an MPI/OpenMP hybrid program
MPIOMPRUN = srun --unbuffered -c $(NCORES)
# Command to run a CUDA/OpenMP hybrid program
CUDAOMPRUN = srun --unbuffered -n 1 --gres=gpu:$(NGPUS) -c $(NCORES)
# Command to run an MPI/CUDA program
MPICUDARUN = srun --unbuffered --gres=gpu:$(NGPUS)
# Command to run an MPI/CUDA/OpenMP program
MPICUDAOMPRUN = srun --unbuffered --gres=gpu:$(NGPUS) -c $(NCORES)
# Command to run an MPI/Pthread hybrid program
MPIPTHREADRUN = srun --unbuffered -c $(NCORES)
# Command to run a CUDA/Pthread hybrid program
CUDAPTHREADRUN = $(CUDAOMPRUN)
