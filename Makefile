CC := gcc
CFLAGS := -O3 -Wall -Wextra -Wpedantic \
	-Wno-unused-but-set-variable -Wno-sign-compare \
	-Iinclude
LDLIBS := -lm

SRC := $(shell find src -type f -name "*.c")
OBJ := $(SRC:src/%.c=build/%.o)

all: build/asciify

build:
	mkdir build

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/asciify: $(OBJ)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -rf build

.PHONY:	all clean
