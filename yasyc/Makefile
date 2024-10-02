
SOURCE_DIR =./src
TESTS_DIR  = ./tests
OPS = -g -Wvarargs -Wall -Werror -std=c23 -fvisibility=hidden

SOURCES = $(shell find $(SOURCE_DIR) -name '*.c')
TESTS = $(shell find $(TESTS_DIR) -name '*.c')

INCLUDE = -I./include/
TESTBED_NAME = testbet
LIB_NAME = libyasyc.so

all:
	clang -g $(SOURCES) $(TESTS) $(OPS) $(INCLUDE) -o $(TESTBED_NAME)
	./$(TESTBED_NAME)
build:
	clang -g -shared $(SOURCES) $(TESTS) $(OPS) $(INCLUDE) -o $(LIB_NAME)

clean:
	rm ./$(LIB_NAME)
	rm ./$(TESTBED_NAME)
