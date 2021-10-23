
all: build run
	
build:
	gcc -c -g SquareEquation.c -c SquareEquation_functions.c
	gcc -Wall SquareEquation.o SquareEquation_functions.o -o SquareEquation -lm
run: build
	./SquareEquation