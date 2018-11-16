# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -Weffc++
LFLAGS  = -L/usr/lib

# All Targets
all: rest

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
rest: bin/Action.o bin/Customer.o bin/Dish.o bin/Table.o bin/Restaurant.o bin/main.o
	@echo 'Building target: ass1'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/ass1 bin/Action.o bin/Customer.o bin/Dish.o bin/Table.o bin/Restaurant.o bin/main.o $(LFLAGS)
	@echo 'Finished building target: ass1'
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
