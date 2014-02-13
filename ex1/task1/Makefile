MAIN=main.c
SOURCE=dlList.c
TARGET=dlList

OBJ=$(SOURCE:.c=.o)
OBJDIR=obj/

CC=gcc
CFLAGS=-Wall -Wpedantic -Wextra -std=c99 -O2

.start:
	mkdir -p $(OBJDIR)

all:
	$(CC) $(MAIN) $(OBJDIR)$(OBJ) -o $(TARGET)

build: .start $(OBJ)

clean:
	rm -rf $(OBJDIR) $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $(OBJDIR)$@
