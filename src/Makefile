# Include the compiling flags and required libraries
include ../conf/config

#vpath %.h src:src/screens
#vpath %.c src:src/screens

# Including screens directory, for the files to be found
vpath %.h screens 
vpath %.c screens

all: soinc

# Add the file you want to join, from screens/ ($^ takes all the dependencies, ../@ output the result in the project's root directory)
#main: main.o menu.o level1.o level2.o level3.o gameover.o
soinc: main.o menu.o level1.o
	$(CC) -o ../$@ $^ $(LIBS)

# This satisfy all the above targets, with a single regex (compile the .c and .h to generate the .o compiled file)
%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

# Use "make clean" to remove all .o files in the current folder
clean:
	rm -rf *.o
