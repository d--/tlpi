CC = gcc
CFLAGS = -Wall
MAIN = dcp

$(MAIN): $(MAIN).c
	$(CC) -o $@ $^ $(CFLAGS)

holes: holes.c
	$(CC) -o $@ $^ $(CFLAGS) && ./$@

.PHONY: clean rmholes

clean:
	rm $(MAIN)

rmholes:
	rm holes holy_file
