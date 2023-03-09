dub: dub.c
	gcc -o dub.exec dub.c

.PHONY: test
test: dub
#test with three names
	./dub.exec Nolan Paige Maggie
#test with no arguments
	./dub.exec
#test with very long arguments & short
	./dub.exec s loooooooooooooooooooooooong
.PHONY: clean
clean:
	rm dub.exec
