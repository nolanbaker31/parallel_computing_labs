ROOT = ../../../../372-2022F/code
include $(ROOT)/common.mk
NCORES = 4
CONFIGS = wavemaker1d_configs.mk
include $(CONFIGS)
stems = w1 w2
movies_seq = $(addsuffix _seq.mp4,$(stems))
movies_par = $(addsuffix _par.mp4,$(stems))
anims_seq = $(addsuffix _seq.anim,$(stems))
anims_par = $(addsuffix _par.anim,$(stems))
tests = $(addsuffix .test,$(stems))

all: wavemaker1d.exec wavemaker1d_omp.exec

test: $(tests)

# w1.test w2.test
$(tests): %.test: %_seq.anim %_par.anim
	$(RUN) diff $^

# w1_seq.anim w2_seq.anim
$(anims_seq): w%_seq.anim: wavemaker1d.exec 
	$(RUN) ./wavemaker1d.exec $(CONFIG$*) $@

# w1_par.anim w2_par.anim
$(anims_par): w%_par.anim: wavemaker1d_omp.exec
	OMP_NUM_THREADS=$(NCORES) $(OMPRUN) ./wavemaker1d_omp.exec $(CONFIG$*) $@

wavemaker1d.exec: wavemaker1d.c $(ANIM)
	$(CCC) -o wavemaker1d.exec wavemaker1d.c -lanim -lm

wavemaker1d_omp.exec: wavemaker1d_omp.c $(ANIM)
	$(OMPCCC) -o wavemaker1d_omp.exec wavemaker1d_omp.c -lanim -lm

# w1_seq.mp4 w2_seq.mp4 w1_par.mp4 w2_par.mp4
$(movies_seq) $(movies_par): %.mp4: %.anim
	$(RUN) $(A2M) $< -o $@

big.anim: wavemaker1d_omp.exec
	OMP_NUM_THREADS=$(NCORES) $(OMPRUN) ./wavemaker1d_omp.exec \
          60000 40000 300 0.005 50000 50000 big.anim

.PHONY: all test $(tests)
