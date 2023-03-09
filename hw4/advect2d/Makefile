NAME = advect2d
MPINAME = advect2d_mpi
ROOT = ../../../../372-2022F/code
include $(ROOT)/common.mk
NPROCS = 4

all: $(NAME).exec $(MPINAME).exec

test: $(NAME).anim $(MPINAME).anim
	diff $(NAME).anim $(MPINAME).anim
	echo "Test passed"

$(NAME).exec: $(NAME).c $(ANIM)
	$(CCC) -o $(NAME).exec $(NAME).c -lanim

$(NAME).anim: $(NAME).exec
	$(RUN) ./$(NAME).exec

$(NAME).mp4: $(NAME).anim
	$(RUN) $(A2M) $(NAME).anim -o $(NAME).mp4

$(MPINAME).exec: $(MPINAME).c
	$(MPICCC) -o $(MPINAME).exec $(MPINAME).c -lanim

$(MPINAME).anim: $(MPINAME).exec
	$(MPIRUN) -n $(NPROCS) ./$(MPINAME).exec

$(MPINAME).mp4: $(MPINAME).anim $(A2M)
	$(RUN) $(A2M) $< -o $@

.PHONY: all test
