
# Build instructions

# C complier
CC ?= cc  
CFLAGS = -Wall -Wextra

# always execute: 'all', 'tests' and 'clean'
.PHONY: all tests clean 

# build all targets: main execuatable and shared library
all: main keccak.so 

# compiles main executable - linking to keccak.o, round_functions.o and utils.o
main: keccak.o c_library/main.c round_functions.o utils.o
	$(CC) $(CFLAGS) -o main c_library/main.c keccak.o round_functions.o utils.o 

# compiles keccak.c to keccak.o (object file)- dependencies round_functions.o and utils.o
keccak.o: c_library/keccak.c c_library/keccak.h round_functions.o utils.o 
	$(CC) $(CFLAGS) -o keccak.o -fPIC -c c_library/keccak.c

# compiles round_functions.c to round_functions.o (object file)  - dependency utils.o
round_functions.o: c_library/round_functions.c c_library/round_functions.h utils.o
	$(CC) $(CFLAGS) -o round_functions.o -fPIC -c c_library/round_functions.c

# Compiles utils.c to utils.o (object file)  
utils.o: c_library/utils.c c_library/utils.h
	$(CC) $(CFLAGS) -o utils.o -fPIC -c c_library/utils.c

# build shared library keccak.so - comprised of keccak.o, round_functions.o and utils.o
keccak.so: keccak.o round_functions.o utils.o
	$(CC) -o keccak.so -shared keccak.o round_functions.o utils.o

# run unit tests
tests:
	python3 unit-tests/keccak-tests.py

# clean up compiled c files
clean:
		@echo "Cleaning up..."
ifeq ($(OS),Windows_NT)
				del /Q *.o *.so main.exe 2>nul
else
				rm -f *.o *.so main
endif
		@echo "Cleaned up!"

