NAME = nbody
ROOT = ../../..
NPROCS = 2
NGPUS = 1  # number of GPUs per node
include $(ROOT)/common.mk
stems = planets-elliptical
movies_seq = $(addsuffix _seq.mp4,$(stems))
movies_par = $(addsuffix _par.mp4,$(stems))
execs_seq = $(addsuffix _seq.exec,$(stems))
execs_par = $(addsuffix _par.exec,$(stems))
anims_seq = $(addsuffix _seq.anim,$(stems))
anims_par = $(addsuffix _par.anim,$(stems))
tests = $(addsuffix .test,$(stems))

all: $(execs_seq) $(execs_par)

test: $(tests)

$(tests): %.test: %_seq.anim %_par.anim
	$(RUN) diff $^

$(anims_seq): %_seq.anim: %_seq.exec
	$(RUN) ./$< $@

$(anims_par): %_par.anim: %_par.exec
	$(MPICUDARUN) -n $(NPROCS) ./$< $@

$(execs_seq): %_seq.exec: nbody.h nbody.c %.c
	$(CCC) -O3 -Wno-missing-braces -o $@ nbody.c $*.c -lanim -lm

$(execs_par): %_par.exec: nbody.h nbody.cu %.c
	$(MPICUDACCC) -O3 -o $@ nbody.cu $*.c -lanim -lm

$(movies_seq) $(movies_par): %.mp4: %.anim
	$(RUN) $(A2M) $< -o $@

.PHONY: all test $(tests)
