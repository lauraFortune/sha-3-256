
# Build instructions

# C complier
CC ?= cc  
CFLAGS = -Wall -Wextra

# always execute: 'all', 'tests' and 'clean'
.PHONY: all tests clean 

# build all targets: main execuatable and shared library
all: main keccak.so 

# compiles main executable - linking to keccak.o, round_functions.o and utils.o
main: keccak.o lib/main.c round_functions.o utils.o
	$(CC) $(CFLAGS) -o main lib/main.c keccak.o round_functions.o utils.o 

# compiles keccak.c to keccak.o (object file)- dependencies round_functions.o and utils.o
keccak.o: lib/keccak.c lib/keccak.h round_functions.o utils.o 
	$(CC) $(CFLAGS) -o keccak.o -fPIC -c lib/keccak.c

# compiles round_functions.c to round_functions.o (object file)  - dependency utils.o
round_functions.o: lib/round_functions.c lib/round_functions.h utils.o
	$(CC) $(CFLAGS) -o round_functions.o -fPIC -c lib/round_functions.c

# Compiles utils.c to utils.o (object file)  
utils.o: lib/utils.c lib/utils.h
	$(CC) $(CFLAGS) -o utils.o -fPIC -c lib/utils.c

# build shared library keccak.so - comprised of keccak.o, round_functions.o and utils.o
keccak.so: keccak.o round_functions.o utils.o
	$(CC) -o keccak.so -shared keccak.o round_functions.o utils.o

# run unit tests
tests:
	python3 unit-tests/keccak-tests.py

# clean up compiled c files
clean:
	@echo "Cleaning up..."
	del /f /q *.o *.so main.exe
	@echo "Cleaned up!"

