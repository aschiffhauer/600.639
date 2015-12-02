CC = gcc
CFLAGS = -std=c99 -Wall -Wextra 

HEADERS = \
	bloomfilter.h \
	bloomfiltertests.h \
	fastq.h \
	fastqtests.h \
	hash.h \
	histogram.h \
	histogramtests.h \
	minsketch.h \
	minsketchtests.h \
	tests.h \

OBJECTS = \
	bloomfilter.o \
	bloomfiltertests.o \
	fastq.o \
	fastqtests.o \
	hash.o \
	histogram.o \
	histogramtests.o \
	minsketch.o \
	minsketchtests.o \
	main.o \

default: ec

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

ec: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f ec