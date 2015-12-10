CC = gcc
CFLAGS = -std=c99 -Wall -Wextra 

HEADERS = \
	bloomfilter.h \
	bloomfiltertests.h \
	error.h \
	errortests.h \
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
	error.o \
	errortests.o \
	fastq.o \
	fastqtests.o \
	hash.o \
	histogram.o \
	histogramtests.o \
	minsketch.o \
	minsketchtests.o \

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

default: tests experiment_synthetic experiment_yeast

tests: $(OBJECTS) tests.o
	$(CC) $(OBJECTS) tests.o -o $@

experiment_synthetic: $(OBJECTS) synthetic.o
	$(CC) $(OBJECTS) synthetic.o -o $@

experiment_yeast: $(OBJECTS) yeast.o
	$(CC) $(OBJECTS) yeast.o -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f experiment_yeast
	-rm -f experiment_synthetic
	-rm -f tests
	-rm -f tests.o
	-rm -f synthetic.o
	-rm -f yeast.o