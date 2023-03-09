NAME = mandelbrot
ROOT = ../../../../372-2022F/code
include $(ROOT)/common.mk
CONFIGS = $(NAME)_configs.mk
include $(CONFIGS)
stems = mb1 mb2 mb3 mb4
movies_seq = $(addsuffix _seq.mp4,$(stems))
movies_cuda = $(addsuffix _cuda.mp4,$(stems))
anims_seq = $(addsuffix _seq.anim,$(stems))
anims_cuda = $(addsuffix _cuda.anim,$(stems))
tests = $(addsuffix .test,$(stems))

all: $(NAME).exec $(NAME)_cuda.exec

test: $(tests)

$(tests): %.test: %_seq.anim %_cuda.anim
	$(RUN) diff $^

$(anims_seq): mb%_seq.anim: $(NAME).exec 
	$(RUN) ./$(NAME).exec $(CONFIG$*) $@

$(anims_cuda): mb%_cuda.anim: $(NAME)_cuda.exec
	$(CUDARUN) ./$(NAME)_cuda.exec $(CONFIG$*) $@

$(NAME).exec: $(NAME).c $(ANIM)
	$(CCC) -O3 -o $(NAME).exec $(NAME).c -lanim

$(NAME)_cuda.exec: $(NAME).cu $(ANIM)
	$(NVCCC) --fmad false -o $@ $< -lanim

$(movies_seq) $(movies_par): %.mp4: %.anim
	$(RUN) $(A2M) -color gray $< -o $@

.PHONY: all test $(tests)
