SOURCE=stack.c dlList.c 
CC=gcc
CFLAGS=-lpthread -Wpedantic -Wall -Werror -std=c99 -O2
OBJ=$(SOURCE:.c=.o)

all: $(OBJ)
	$(CC) main.c $(SOURCE) $(CFLAGS) -o stack

build: $(OBJ)

clean:
	rm stack *.o

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
