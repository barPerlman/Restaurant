# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -std=c++11
LFLAGS  = -L/usr/lib

# All Targets
all: rest

# Tool invocations
# Executable "rest" depends on the following .o files
rest: bin/Action.o bin/Customer.o bin/Dish.o bin/Table.o bin/Restaurant.o bin/main.o
	@echo 'Building target: rest'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/rest bin/Action.o bin/Customer.o bin/Dish.o bin/Table.o bin/Restaurant.o bin/main.o $(LFLAGS)
	@echo 'Finished building target: rest'
	@echo ' '

# Depends on the source and header files
bin/Action.o: src/Action.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/Action.o src/Action.cpp

# Depends on the source and header files 
bin/Customer.o: src/Customer.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/Customer.o src/Customer.cpp

# Depends on the source and header files
bin/Dish.o: src/Dish.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/Dish.o src/Dish.cpp

# Depends on the source and header files 
bin/Table.o: src/Table.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/Table.o src/Table.cpp

# Depends on the source and header files
bin/main.o: src/main.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/main.o src/main.cpp

# Depends on the source and header files
bin/Restaurant.o: src/Restaurant.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/Restaurant.o src/Restaurant.cpp

#Clean the build directory
clean: 
	rm -f bin/*
