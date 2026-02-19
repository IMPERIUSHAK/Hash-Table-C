CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g
LDFLAGS = -lm

SRC = main.c hash_table.c prime.c
OBJ = $(SRC:%.c=../build/%.o)

TARGET = ../build/main
BUILD_DIR = ../build

# ─────── default target ───────
all: $(BUILD_DIR) $(TARGET)

# ─────── link ───────
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# ─────── compile ───────
../build/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ─────── create build dir ───────
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# ─────── clean ───────
clean:
	rm -rf $(BUILD_DIR)
