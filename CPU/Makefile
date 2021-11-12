all: build run	

build:
	g++ -g Assembler.cpp -o Assembler
	g++ -c -g CPU.cpp  ../Stack/Stack.c
	g++ CPU.o ../Stack/Stack.o -o CPU -lm
run:
	./Assembler CMD_ENG.txt
	./CPU Commands.txt