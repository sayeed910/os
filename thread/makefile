CC=gcc
CFLAGS=-pthread
DEPS=LinkedList.h
OBJ=LinkedList.o SearchByThreading.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
search: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o 
.PHONY: clean


