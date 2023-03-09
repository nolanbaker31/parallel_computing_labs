all: ring.exec test

ring.exec: ring.c
	mpicc -o ring.exec ring.c

test: ring.exec
	srun -n 5 ring.exec

clean:
	rm -f ring.exec
