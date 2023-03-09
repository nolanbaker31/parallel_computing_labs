ROOT = ../../
include $(ROOT)/common.mk
NAME = sat_mpi

all: $(NAME).exec

$(NAME).exec: $(NAME).c  Makefile
	$(MPICCC) -o $@ $<

$(NAME).dat: $(NAME).exec
	$(MPIRUN) -n 1 ./$(NAME).exec > $(NAME).dat
	$(MPIRUN) -n 2 ./$(NAME).exec >> $(NAME).dat
	$(MPIRUN) -n 4 ./$(NAME).exec >> $(NAME).dat
	$(MPIRUN) -n 8 ./$(NAME).exec >> $(NAME).dat
	$(MPIRUN) -n 16 ./$(NAME).exec >> $(NAME).dat
	$(MPIRUN) -n 32 ./$(NAME).exec >> $(NAME).dat

graphs:
	gnuplot $(NAME).gnu

clean::
	rm -f *.dat *.pdf

.PHONY: all graphs
