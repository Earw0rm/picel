
SOURCE_DIR =./src
TESTS_DIR  = ./tests

SOURCES = $(shell find $(SOURCE_DIR) -name '*.c')
TESTS = $(shell find $(TESTS_DIR) -name '*.c')
INCLUDE = -I./include/
TESTBED_NAME = testbet
#INCLUDE = $(addprefix -I, $(shell ls -d ./include/*))


all:
	clang -g $(SOURCES) $(INCLUDE)  

test:
	clang -g $(SOURCES) $(TESTS) $(INCLUDE) -o $(TESTBED_NAME)
	./$(TESTBED_NAME)

dtest:
	clang -g $(SOURCES) $(TESTS) $(INCLUDE) -o $(TESTBED_NAME)
	gdb ./$(TESTBED_NAME)