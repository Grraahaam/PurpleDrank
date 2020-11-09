# Documentation

Purple Drank documentation

# Build

At the project's root level, type : `make`
And to run, type : `./soinc`

The first Makefile generate the compiling flags configuration, and the src/Makefile define the links between files and build the executable

If you are adding a new file to the project, add its name to the Makefile following line:

* `soinc: main.o menu.o [filename].o`

It will be found automatically, if it is in `src` or `src/screens` directories. No need to write the full path

# Structure

*The structure tries to follow git/dev best practices*

* `/lib`
    * Contains external libraries (e.g. raylib)

* `/src`
    * Contains our source code and header file (*.c and *.h)

* `/tests`
    * Contains ALL the tests, keep them separate HERE

* `/scripts`
    * Automatization scripts, for custom operations not including Makefile which is at the project's root level

* `/res`
    * Contains all the static ressources, as images and sounds

* `/doc`
    * Contains all the documentation and useful tips
