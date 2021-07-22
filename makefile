CC=gcc

CFLAGS=-I.

DEPS = 

OBJ = q2_2019337.o 

co:	


	gcc q2_2019337.c -pthread -lrt -o q2_2019337.o

	

%.o: %.c $(DEPS)

	$(CC) -o $@ $< $(CFLAGS)



all: $(OBJ)



clean:

	rm -f $(OBJ)

