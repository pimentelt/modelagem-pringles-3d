# Compila a raylib 6.0 vendorizada e o projeto, sem instalação externa da raylib.

CC = gcc
AR = ar
CFLAGS ?= -Wall -Wextra -std=c11
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
RAYLIB_SRC = third_party/raylib/src
RAYLIB_BUILD_DIR = $(BUILD_DIR)/raylib
RAYLIB_LIB = $(RAYLIB_BUILD_DIR)/libraylib.a
SOURCES = main.c pringles.c formiga.c
OBJECTS = $(SOURCES:%.c=$(OBJ_DIR)/%.o)

ifeq ($(OS),Windows_NT)
    BIN = $(BUILD_DIR)/pringles.exe
    SYSTEM_LIBS = -static-libgcc -lopengl32 -lgdi32 -lwinmm -lm
else
    BIN = $(BUILD_DIR)/pringles
    SYSTEM_LIBS = -lGL -lm -lpthread -ldl -lrt -lX11
endif

.PHONY: all run clean FORCE

all: $(BIN)

$(BIN): $(OBJECTS) $(RAYLIB_LIB)
	$(CC) $(OBJECTS) $(RAYLIB_LIB) -o $@ $(SYSTEM_LIBS)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(RAYLIB_SRC) -I. -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(RAYLIB_BUILD_DIR):
	mkdir -p $(RAYLIB_BUILD_DIR)

# Always call raylib's own makefile; it incrementally rebuilds changed modules.
$(RAYLIB_LIB): FORCE | $(RAYLIB_BUILD_DIR)
	$(MAKE) -C $(RAYLIB_SRC) CC=$(CC) AR=$(AR) PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=STATIC RAYLIB_MODULE_AUDIO=FALSE RAYLIB_RELEASE_PATH=../../../$(RAYLIB_BUILD_DIR)

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(RAYLIB_SRC)/*.o
	rm -rf $(BUILD_DIR)

FORCE:
