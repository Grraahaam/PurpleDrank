all: config
	@make -C src

config:
	cp conf/config.default conf/config

unix:
	@echo "generate Bath config"
	@echo "# Configuration for make" > conf/config
	@echo "CC=gcc" >> conf/config
	@echo "CFLAGS=-std=c99 -Wall -Wextra -lraylib" >> conf/config
	@echo "LIBS= -lraylib" >> conf/config
	@echo "OS=`uname`"  >> conf/config

windows:
	@echo "generate PowerShell config"
	@echo # Configuration for make > config
	@echo OS=Windows >> conf/config
	@echo CC=gcc >> conf/config
	@echo CFLAGS=-std=c99 -Wall -Wextra` >> conf/config
	@echo LIBS= -lraylib >> conf/config

clean:
	@make -C src clean
