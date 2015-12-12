CC = gcc
CFLAGS = -std=c99 -Wall -Wextra 

HEADERS = \
	algorithm.h \
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
	algorithm.o \
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

default: tests experiment_synthetic experiment_yeast correct kmerprinter

tests: $(OBJECTS) tests.o
	$(CC) $(OBJECTS) tests.o -o $@

experiment_synthetic: $(OBJECTS) synthetic.o
	$(CC) $(OBJECTS) synthetic.o -o $@

experiment_yeast: $(OBJECTS) yeast.o
	$(CC) $(OBJECTS) yeast.o -o $@

correct: $(OBJECTS) correct.o
	$(CC) $(OBJECTS) correct.o -o $@

kmerprinter: $(OBJECTS) kmerprinter.o
	$(CC) $(OBJECTS) kmerprinter.o -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f experiment_yeast
	-rm -f experiment_synthetic
	-rm -f tests
	-rm -f tests.o
	-rm -f synthetic
	-rm -f synthetic.o
	-rm -f yeast
	-rm -f yeast.o
	-rm -f correct
	-rm -f correct.o
	-rm -f kmerprinter
	-rm -f kmerprinter.o