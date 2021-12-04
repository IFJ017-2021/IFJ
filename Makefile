RM=rm -f
CC=gcc
CFLAGS=-c -Wall -std=c99 -O3 -pedantic

#timto se sestavi spustitelny soubor z *.o binarek
#create executable file
all: error.o main.o parser.o prec_table.o scanner.o stack.o str.o symtable.o tokenList.o
	$(CC) error.o main.o parser.o prec_table.o scanner.o stack.o str.o symtable.o tokenList.o -lm -o ifj

clean:
	$(RM) *.o $(objects) ifj
