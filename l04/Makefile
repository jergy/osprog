CFLAGS = -Wall -Werror -g
CXXFLAGS = -Wall -Werror -std=c++11 -g

TESTS := testCBuffer testBuffer
RUN_TEST_DEFAULT := run-testBuffer
RUN_TESTS := $(foreach t,$(TESTS),run-$t)

.PHONY: default all test
default: test
all: $(RUN_TESTS)
test: $(RUN_TEST_DEFAULT)

memcheck: testBuffer
	valgrind --leak-check=yes ./testBuffer

testCBuffer.o cbuffer.o: cbuffer.h Makefile
testCBuffer: testCBuffer.o cbuffer.o

testBuffer.o Buffer.o: Buffer.h Makefile
testBuffer: CC = g++  # we need to use g++ to link in C++ libs
testBuffer: testBuffer.o Buffer.o

run-%: %
	@echo
	@echo "Running $<"
	./$<

.PHONY: clean distclean test
clean:
	rm -f *.o
distclean: clean
	rm -f $(TESTS)
