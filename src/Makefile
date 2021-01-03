
# Include the compiling flags and required libraries
include ../conf/makeconf

OBJECTS_SCREENS = $(patsubst %.c, %.o, $(wildcard screens/*.c))
OBJECTS_MAIN = $(patsubst %.c, %.o, $(wildcard *.c))

all: $(APP)

# Add the file you want to join, from screens/ ($^ takes all the dependencies, ../@ output the result in the project's root directory)
$(APP): $(OBJECTS_MAIN) $(OBJECTS_SCREENS)
	$(CC) -o ../$@ $^ $(CLIBS)

# This satisfy all the above targets, with a single regex (compile the .c to generate the .o compiled file)
%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@

# Use "make clean" to remove all .o files in the current folder
clean:
	rm -rf $(OBJECTS_MAIN) $(OBJECTS_SCREENS)

# Use "make clean all" to remove all *.o files and the binary
purge:
	rm -rf $(OBJECTS_MAIN) $(OBJECTS_SCREENS)
	rm -f ../$(APP)

