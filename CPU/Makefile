all: build run	

build:
	g++ -g sourseAssembler.cpp -o build/Assembler
	g++ -c -g source/CPU.cpp  ../Stack/Stack.c
	g++ build/CPU.o ../Stack/Stack.o -o build/CPU -lm
run:
	./build/Assembler build/CMD_ENG.txt
	./build/CPU build/Commands.txt