# Makefile for compiling Shell program files

CC     = gcc
CFLAGS = -Wall
EXE    = shell
OBJ    = main.o logic.o

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ)

main.o: logic.h
logic.o: logic.h

clean:
	rm -f $(OBJ) $(EXE)