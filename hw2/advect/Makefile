NAME = advect
ROOT = ../../../../372-2022F/code
include $(ROOT)/common.mk

all: $(NAME).exec

test: $(NAME).mp4

$(NAME).exec: $(NAME).c Makefile $(ANIM)
	$(CCC) -o $(NAME).exec $(NAME).c -lanim

$(NAME).anim: $(NAME).exec Makefile
	$(RUN) ./$(NAME).exec

$(NAME).mp4: $(NAME).anim
	$(RUN) $(A2M) $(NAME).anim -o $(NAME).mp4

.PHONY: all test
