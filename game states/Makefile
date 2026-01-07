# variables
compiler = gcc

# explicit default
.DEFAULT_GOAL := all

# high-level target builds the executable      IN ORDER TO EXECUTE: ./main.out 
all: main.out

# link step: link object files to final executable
main.out: main.o title.o gameplay.o
	$(compiler) main.o title.o gameplay.o -o main.out -L lib/ -lraylib -lm

# compile main.c to object
main.o: main.c
	$(compiler) -c main.c -o main.o -I include/

# compile test.c to object
gameplay.o: gameplay.c
	$(compiler) -c gameplay.c -o gameplay.o -I include/

# compile othertest.c to object
title.o: title.c
	$(compiler) -c title.c -o title.o -I include/

# clean up generated .o files
clean:
	rm -f *.o main.out