
# Defining the compilation arguments
CFLAGS=-Wall -I .
CFLAGS=-std=c99 -Wall -Wextra -lm
LIBS=-lraylib

#vpath %.h src:src/screens
#vpath %.c src:src/screens

# Including screens directory, for the files to be found
vpath %.h screens
vpath %.c screens

all: main

# Add the file you want to join, from screens/
#main: main.o menu.o level1.o level2.o level3.o gameover.o
main: main.o menu.o

# This satisfy all the above targets, with a single regex (compile the .c and .h to generate the .o compiled file)
%.o: %.c %.h
	$(CC) -c $< $(CFLAGS) $(LIBS) -o $@

# Use "make clean" to remove all .o files in the current folder
clean:
	rm -rf *.o
