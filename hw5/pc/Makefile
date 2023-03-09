ROOT = ../../../../372-2022F/code
include $(ROOT)/common.mk

all: pc0.exec pc.exec

pc0.exec: pc0.c
	$(PTHREADCCC) -o pc0.exec pc0.c -lm

test0: pc0.exec
	./pc0.exec 5

bigtest0: pc0.exec
	for i in {1..1000}; do ./pc0.exec 5; done | sort -n | uniq -c

pc.exec: pc.c
	$(PTHREADCCC) -o pc.exec pc.c -lm

test: pc.exec
	./pc.exec 5

bigtest: pc.exec
	for i in {1..1000}; do ./pc.exec 5; done | sort -n | uniq -c


.PHONY: all test0 bigtest0 test bigtest
