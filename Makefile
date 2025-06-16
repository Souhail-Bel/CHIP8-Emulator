# Project configs
debug 		?= 0
NAME		:= chip8emu
SRC_DIR		:= src
BUILD_DIR	:= build
INCLUDE_DIR := include
BIN_DIR 	:= bin


# Object file paths
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(SRC_DIR)/*.c)) # it's patsubst not patsubset smh

# Compiler settings
CC := gcc

# Flags settings (Compiler and Linker)
CFLAGS := -Wall -Wextra -pedantic -D_XOPEN_SOURCE=500 -I$(INCLUDE_DIR) -lSDL2
LFLAGS := 

ifeq ($(debug), 1)
	CFLAGS := $(CFLAGS) -g -D DEBUG_MODE
endif


all: $(NAME)

# Build exec
$(NAME): dir $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o $(BIN_DIR)/$@

# Object build rule
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | dir
	$(CC) $(CFLAGS) -c $< -o $@

dir:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean dir
