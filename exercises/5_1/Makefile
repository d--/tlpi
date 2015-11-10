CC = gcc
CFLAGS = -Wall -D_FILE_OFFSET_BITS=64
MAIN = large_file

$(MAIN): $(MAIN).c
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm $(MAIN) 
