#include <stdlib.h>
#include <stdio.h>

#include "algorithm.h"

#define ARGC 6 + 1         // The number of arguments expected
#define INPUT 1            // The index of argv for input
#define OUTPUT 2           // The index of argv for output
#define KMER_SIZE 3        // The index of argv for kmer_size
#define MINSKETCH_WIDTH 4  // The index of argv for minsketch_width
#define MINSKETCH_HEIGHT 5 // The index of argv for minsketch_height
#define FREQUENCY_CUTOFF 6 // The index of argv for frequency_cutoff

// Defines a macro that prints out an error message, usage, and exits.
#define USAGE(...) { \
	fprintf(stderr, __VA_ARGS__); \
	fprintf(stderr, "./correct <input> <output> <kmer_size> <minsketch_width> <minsketch_height> <frequency_cutoff>\n"); \
	return EXIT_FAILURE; \
}

/*
Runs an experiment based off specified reads.
Our hypothesis is that our correction algorithm with find some errors and fix them.
./correct <input> <output> <kmer_size> <minsketch_width> <minsketch_height> <frequency_cutoff>
*/
int main(int argc, const char* argv[]) {
	if (argc != ARGC) {
		USAGE("invalid number of arguments\n")
	}

	const char *input_file_name = argv[INPUT];
	const char *output_file_name = argv[OUTPUT];
	int kmer_size = atoi(argv[KMER_SIZE]);
	int minsketch_width = atoi(argv[MINSKETCH_WIDTH]);
	int minsketch_height = atoi(argv[MINSKETCH_HEIGHT]);
	int frequency_cutoff = atoi(argv[FREQUENCY_CUTOFF]);

	if (algorithm_correct(input_file_name, output_file_name, kmer_size, minsketch_width, minsketch_height, frequency_cutoff) == false) {
		return EXIT_FAILURE;
	}
}