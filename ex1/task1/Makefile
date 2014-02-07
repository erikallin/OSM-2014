SOURCE=dlList.c
TARGET=dlList

OBJ=$(SOURCE:.c=.o)
OBJDIR=obj/

CC=gcc
CFLAGS= -Wall -Wpedantic -Wextra -std=c99 -O2

.start:
	mkdir -p $(OBJDIR)

all: build
#	$(CC) -c $(SOURCE) $(CFLAGS) -o $(TARGET)

build: .start $(OBJ)
	$(CC) -c $(SOURCE) $(CFLAGS) -o $(TARGET)

clean:
	rm -rf $(OBJDIR) $(TARGET)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $(OBJDIR)$@
