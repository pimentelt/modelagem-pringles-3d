# Compila o projeto com raylib em Linux (Ubuntu) ou Windows com MSYS2 UCRT64.

CC = gcc
CFLAGS ?= -Wall -Wextra -std=c11
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
SOURCES = main.c pringles.c formiga.c
OBJECTS = $(SOURCES:%.c=$(OBJ_DIR)/%.o)

# Permite substituir esses valores na linha de comando, se a raylib estiver
# instalada em outro local ou usar uma configuração diferente.
ifeq ($(OS),Windows_NT)
    RAYLIB_PREFIX ?= C:/msys64/ucrt64
    RAYLIB_CFLAGS ?= -I$(RAYLIB_PREFIX)/include
    RAYLIB_LIBS ?= -L$(RAYLIB_PREFIX)/lib -lraylib -lopengl32 -lgdi32 -lwinmm
    BIN = $(BUILD_DIR)/pringles.exe
    RUN = ./$(BIN)
else
    RAYLIB_CFLAGS ?= $(shell pkg-config --cflags raylib 2>/dev/null)
    RAYLIB_LIBS ?= $(shell pkg-config --libs raylib 2>/dev/null)
    ifeq ($(strip $(RAYLIB_LIBS)),)
        RAYLIB_LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    endif
    BIN = $(BUILD_DIR)/pringles
    RUN = ./$(BIN)
endif

.PHONY: all run clean

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(RAYLIB_LIBS) -lm

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(RAYLIB_CFLAGS) -I. -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

run: $(BIN)
	$(RUN)

clean:
	rm -f $(OBJECTS) $(BIN)
