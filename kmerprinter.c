#include <stdlib.h>
#include <stdio.h>

#include "histogram.h"
#include "minsketch.h"
#include "fastq.h"
#include "error.h"

#define ARGC 2 + 1         // The number of arguments expected
#define INPUT 1            // The index of argv for input
#define KMER_SIZE 2        // The size of kmers

// Defines a macro that prints out an error message, usage, and exits.
#define USAGE(...) { \
	fprintf(stderr, __VA_ARGS__); \
	fprintf(stderr, "./kmerprinter <input> <kmer_size>\n"); \
	return EXIT_FAILURE; \
}

/*
Prints to stdout the kmers of a file
./kmerprinter <input> <kmer_size>
*/
int main(int argc, const char* argv[]) {
	if (argc != ARGC) {
		USAGE("invalid number of arguments\n")
	}

	const char *input_file_name = argv[INPUT];
	int kmer_size = atoi(argv[KMER_SIZE]);

	// Validate parameters
	if (input_file_name == NULL || *input_file_name == '\0' ||  kmer_size <= 0) {
		USAGE("invalid arguments\n")
	}

	// Iterate through all of the reads of the fastq file, outputting all kmers
	fastq_for_each_kmer(input_file_name, kmer_size, kmer, {
		fprintf(stdout, "%s\n", kmer);
	});
}