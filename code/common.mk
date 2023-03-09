# Common definitions for all make files.

# If there is a file config.mk, load it.  Otherwise, if this machine is
# named cisc372, load config-372.mk.  Otherwise, load config-default.mk.

CONFIGS = $(ROOT)/config
HOST := $(shell hostname)
ifneq ("$(wildcard $(CONFIGS)/config.mk)","")
CONFIG = $(CONFIGS)/config.mk
else
ifeq ($(HOST),cisc372)
CONFIG = $(CONFIGS)/config-372.mk
else
ifneq (,$(findstring bridges,$(HOST)))
CONFIG = $(CONFIGS)/config-bridges.mk
else
CONFIG = $(CONFIGS)/config-default.mk
endif
endif
endif
include $(CONFIG)

# Important subdirectories...
SRC = $(ROOT)/src
INC = $(ROOT)/include
LIB = $(ROOT)/lib
BIN = $(ROOT)/bin
ADIR = $(SRC)/anim
SEQ = $(SRC)/seq

# Flags for preprocessor:
CPPFLAGS += -I$(INC)
# Flags for compiler (nothing to add for now):
# CFLAGS +=
# Flags for linker:
LDFLAGS += -L$(LIB)

# preprocess, compile and link with all flags:
CCC = $(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS)
# preprocess and compile only with corresponding flags:
CCO = $(CC) $(CPPFLAGS) $(CFLAGS) -c

# ditto for MPICC...
MPICCC = $(MPICC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS)
MPICCO = $(MPICC) $(CPPFLAGS) $(CFLAGS) -c

# ditto for PTHREADCC
PTHREADCCC = $(PTHREADCC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS)
PTHREADCCO = $(PTHREADCC) $(CPPFLAGS) $(CFLAGS) -c

# ditto for OMPCC
OMPCCC = $(OMPCC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS)
OMPCCO = $(OMPCC) $(CPPFLAGS) $(CFLAGS) -c

# ditto for CUDA
NVCCC = $(NVCC) $(CPPFLAGS) $(LDFLAGS)
# NVCCO = $(NVCC) $(CPPFLAGS) -c

# ditto for MPI/OpenMP hybrids
MPIOMPCCC = $(MPIOMPCC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS)
MPIOMPCCO = $(MPIOMPCC) $(CPPFLAGS) $(CFLAGS) -c

# ditto for CUDA/OpenMP hybrids
CUDAOMPCCC = $(CUDAOMPCC) $(CPPFLAGS) $(LDFLAGS)

# ditto for MPI/CUDA hybrids
MPICUDACCC = $(MPICUDACC) $(CPPFLAGS) $(LDFLAGS)

# ditto for MPI/CUDA/OpenMP hybrids
MPICUDAOMPCCC = $(MPICUDAOMPCC) $(CPPFLAGS) $(LDFLAGS)

# ditto for MPI/Pthread hybrids
MPIPTHREADCCC = $(MPIPTHREADCC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS)
MPIPTHREADCCO = $(MPIPTHREADCC) $(CPPFLAGS) $(CFLAGS) -c

# ditto for CUDA/Pthread hybrids
CUDAPTHREADCCC = $(CUDAPTHREADCC)  $(CPPFLAGS) $(LDFLAGS)



A2M = $(BIN)/anim2mp4
A2T = $(BIN)/anim2txt
A2G = $(BIN)/anim2gif
ANIM_HEADERS = $(INC)/anim.h $(INC)/anim_dev.h $(INC)/mpianim.h
ANIM_TOOLS = $(LIB)/libanim.a $(A2M) $(A2T) $(A2G)
ANIM_SRC = $(ADIR)/anim.c $(ADIR)/mpianim.c $(ADIR)/convert.c $(ADIR)/convert.h \
           $(ADIR)/anim2mp4.c $(ADIR)/anim2txt.c $(ADIR)/anim2gif.c
# Any rule that requires something from the ANIM library can just
# include $(ANIM) as a prereq:
ANIM = $(ANIM_HEADERS) $(ANIM_TOOLS)

# need to make this the first rule since this file is typically loaded
# before any other rules, so whatever is the first rule here will
# become the default...
myall: all

$(ANIM_TOOLS): $(ANIM_HEADERS) $(ANIM_SRC) $(ADIR)/Makefile
	$(MAKE) -C $(ADIR) clean
	$(MAKE) -C $(ADIR)

clean::
	rm -f *~ *.tmp a.out *.exec *.o *.gif *.anim *.mp4 .*.shfp.*

.PHONY: myall clean
