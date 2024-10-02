BUILD_DIR = ./build
SOURCE_DIR = ./src
COMPILER ?= clang
LIBS_SRC = ./libs
YASYC_PATH =../yasyc/

OPS = -g -Wvarargs -Wall -Werror -std=c23
INCLUDE_FLAGS = -I./include -I./libs/include -I../yasyc/include
LINKER_FLAGS = -L$(YASYC_PATH) -Wl,-rpath,$(YASYC_PATH)  -lm -lc -lglfw -lGL -ldl -lpthread -lX11 -lXrandr -lXi -lyasyc 

OUT = renderer

SRC_C_FILES = $(shell find $(SOURCE_DIR) -name '*.c')
LIB_C_FILES = $(shell find $(LIBS_SRC) -name '*.c')


SRC_OBJ_FILES = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/src/%.o, $(SRC_C_FILES))
LIB_OBJ_FILES = $(patsubst $(LIBS_SRC)/%.c, $(BUILD_DIR)/libs/%.o, $(LIB_C_FILES))


OBJ_FILES = $(SRC_OBJ_FILES) $(LIB_OBJ_FILES)

.PHONY: all clean

all: ogl_renderer
	$(BUILD_DIR)/$(OUT)

ogl_renderer: $(OBJ_FILES)
	$(COMPILER) -o $(BUILD_DIR)/$(OUT) $(OBJ_FILES) $(DEFINES) $(INCLUDE_FLAGS) $(LINKER_FLAGS) $(OPS)


$(BUILD_DIR)/src/%.o: $(SOURCE_DIR)/%.c
	mkdir -p $(@D)
	$(COMPILER) $(OPS) -c $< -o $@ $(DEFINES) $(INCLUDE_FLAGS)


$(BUILD_DIR)/libs/%.o: $(LIBS_SRC)/%.c
	mkdir -p $(@D)
	$(COMPILER) $(OPS) -c $< -o $@ $(INCLUDE_FLAGS)

clean:
	rm -rf $(BUILD_DIR)/*
