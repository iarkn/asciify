CFLAGS := -Wall -Wextra -Wno-unused-but-set-variable -Wno-sign-compare \
		  -Ilib/
LDLIBS := -lm

SRC := $(wildcard src/*.c)
OBJ := $(patsubst %.c, %.o, $(SRC))

bin/asciify: $(OBJ) | bin
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

bin:
	mkdir bin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -rf bin
	rm -f *.o

.PHONY:	clean
