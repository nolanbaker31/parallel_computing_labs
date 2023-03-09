# Makefile for nbody graviational simulation, Pthread version
ROOT = ../../../../372-2022F/code
NCORES = 4
include $(ROOT)/common.mk
# You may want to comment out some of the bigger ones here for testing:
stems = planets-elliptical figure8 galaxy galaxy2 galaxy4 galaxy6 galaxy10
movies_seq = $(addsuffix _seq.mp4,$(stems))
movies_par = $(addsuffix _par.mp4,$(stems))
execs_seq = $(addsuffix _seq.exec,$(stems))
execs_par = $(addsuffix _par.exec,$(stems))
anims_seq = $(addsuffix _seq.anim,$(stems))
anims_par = $(addsuffix _par.anim,$(stems))
tests = $(addsuffix .test,$(stems))

all: $(execs_seq) $(execs_par)

test: $(tests)

# $^ stands for all prerequisites; see
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
$(tests): %.test: %_seq.anim %_par.anim
	$(RUN) diff $^

$(anims_seq): %_seq.anim: %_seq.exec
	$(RUN) ./$< $@

$(anims_par): %_par.anim: %_par.exec
	OMP_NUM_THREADS=$(NCORES) $(CUDARUN) ./$< $(NCORES) $@

# $* matches the stem (%)
$(execs_seq): %_seq.exec: nbody.h nbody.c %.c
	$(CCC) -O3 -Wno-missing-braces -o $@ nbody.c $*.c -lanim -lm

$(execs_par): %_par.exec: nbody.h nbody_par.cu %.c
	$(NVCCC) --compiler-options -Wno-missing-braces --compiler-options -fopenmp -fmad false -o $@ nbody_par.cu $*.c -lanim -lm

$(movies_seq) $(movies_par): %.mp4: %.anim
	$(RUN) $(A2M) $< -o $@

.PHONY: all test $(tests)
