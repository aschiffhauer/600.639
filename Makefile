CC = gcc
CFLAGS = -std=c99 -Wall -Wextra 

HEADERS = hash.h bloomfilter.h minsketch.h
OBJECTS = hash.o bloomfilter.o minsketch.o main.o

default: ec

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

ec: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f ec