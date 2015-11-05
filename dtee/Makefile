CC = gcc
CFLAGS = -Wall
MAIN = dtee

$(MAIN): $(MAIN).c
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm $(MAIN) 
