CC = gcc -Wall 
SRC = main.c inference.c initialisation.c tools.c train.c 
OBJ = main.o inference.o initialisation.o tools.o train.o

SOM:	$(OBJ) header.h
	$(CC) -o main $(OBJ) -lm
%.o:	%.c header.h
	$(CC) -c $<

clean:
	rm *.o 
