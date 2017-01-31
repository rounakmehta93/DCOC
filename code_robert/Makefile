CC = gcc
SRC = test.c


gsl_example: $(SRC)
	$(CC) $(SRC) aux_functions.c interpn.c -std=c99 -mcmodel=large -lm -fopenmp -O3 -Wall -Wextra -o p 

clean:
	p.o
