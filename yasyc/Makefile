
SOURCE_DIR =./src
TESTS_DIR  = ./tests
OPS = -g -Wvarargs -Wall -Werror -std=c23
SOURCES = $(shell find $(SOURCE_DIR) -name '*.c')
TESTS = $(shell find $(TESTS_DIR) -name '*.c')
INCLUDE = -I./include/
TESTBED_NAME = testbet
#INCLUDE = $(addprefix -I, $(shell ls -d ./include/*))




all:
	clang -g $(SOURCES) $(TESTS) $(OPS) $(INCLUDE) -o $(TESTBED_NAME)
	./$(TESTBED_NAME)

build:
	clang -g $(SOURCES) $(INCLUDE)  

clean:
	rm ./$(TESTBED_NAME)