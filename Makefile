CC := gcc
CFLAGS := -Wall
LDLIBS := -lm -lz -lpng

bin/asciify: src/main.c | bin
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

bin:
	mkdir bin

clean:
	rm -rf bin

.PHONY:	clean
