#include <stdlib.h>
#include <stdio.h>

#include "histogram.h"
#include "minsketch.h"
#include "fastq.h"
#include "error.h"

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

	if (input_file_name == NULL || *input_file_name == '\0'
  ||  output_file_name == NULL || *output_file_name == '\0'
	||  kmer_size <= 0
	||  minsketch_width <= 0 || minsketch_height <= 0) {
		USAGE("invalid arguments\n")
	}

	// Setup the minsketch-based histogram and read in all the kmers from fastq file
	histogram *h = histogram_new(MINSKETCH, minsketch_new(minsketch_width, minsketch_height));
	if (histogram_read(h, input_file_name, kmer_size) == false) {
		fprintf(stderr, "failed to open %s for reading\n", input_file_name);
		return -1;
	}

	// Setup writing to file
	FILE *file = fopen(output_file_name, "w");
	if (file == NULL) {
		fprintf(stderr, "failed to open %s for writing\n", output_file_name);
		return -1;
	}

	// Print out some debug information for the user
	printf("experiment parameters:\n");
	printf("  kmer size: %d\n", kmer_size);
	printf("  frequency cutoff for error correction: %d\n", frequency_cutoff);

	// Iterate through all of the reads of the fastq file, outputting any corrections
	fastq *f = fastq_new(input_file_name);
	if (f == NULL) {
		fprintf(stderr, "failed to open %s for reading\n", input_file_name);
		return -1;
	}
	char sequence_copy[MAX_READ_LENGTH + 1];
	char kmer[kmer_size + 1];
	int corrections = 0;
	int length = 0;
	while (fastq_read(f)) {
		length++;
		strcpy(sequence_copy, f->sequence);
		int i = 0;
		while (error_correct(h, f->sequence, kmer_size, frequency_cutoff)) {
			strncpy(kmer, f->sequence, kmer_size);
			// Guard against infinite looping
			if (++i >= kmer_size) {
				break;
			}
		}
		// Output any corrections to stdout
		if (strcmp(sequence_copy, f->sequence) != 0) {
			corrections++;
			printf("correction:\n  old: %s\n  new: %s\n", sequence_copy, f->sequence);
		}
		// Write to corrected fastq file
		if (fprintf(file, "%s\n%s\n%s\n%s\n", f->identifier, f->sequence, f->optional, f->qualities) < 0) {
			fprintf(stderr, "failed to write to %s\n", output_file_name);
			return -1;
			fclose(file);
		}
	}

	// Print out summary
	printf("corrected %f%% of sequences\n", 100 * ((float)(corrections)/((float) (length))));
	printf("%s contains corrected reads\n", output_file_name);
	fclose(file);

	// Clean up file handles and allocations
	fastq_free(f);
	histogram_free(h);
}