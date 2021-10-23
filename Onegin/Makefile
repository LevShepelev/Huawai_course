
all: build run
	
build:
	gcc -c  Onegin.c -c Onegin_functions.c
	gcc -Wall Onegin.o Onegin_functions.o -o Onegin
run: build
	./Onegin