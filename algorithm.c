#include <stdbool.h>

#include "algorithm.h"
#include "histogram.h"
#include "minsketch.h"
#include "fastq.h"
#include "error.h"

bool algorithm_correct(const char *input_file_name, const char *output_file_name, int kmer_size, int minsketch_width, int minsketch_height, int frequency_cutoff) {
	if (input_file_name == NULL || *input_file_name == '\0'
  ||  output_file_name == NULL || *output_file_name == '\0'
	||  kmer_size <= 0
	||  minsketch_width <= 0 || minsketch_height <= 0
	||  frequency_cutoff <= 0) {
		return false;
	}

	// Setup the minsketch-based histogram and read in all the kmers from fastq file
	histogram *h = histogram_new(MINSKETCH, minsketch_new(minsketch_width, minsketch_height));
	if (histogram_read(h, input_file_name, kmer_size) == false) {
		fprintf(stderr, "failed to open %s for reading\n", input_file_name);
		return false;
	}

	// Setup writing to file
	FILE *file = fopen(output_file_name, "w");
	if (file == NULL) {
		fprintf(stderr, "failed to open %s for writing\n", output_file_name);
		return false;
	}

	// Print out some debug information for the user
	printf("experiment parameters:\n");
	printf("  kmer size: %d\n", kmer_size);
	printf("  frequency cutoff for error correction: %d\n", frequency_cutoff);

	// Iterate through all of the reads of the fastq file, outputting any corrections
	fastq *f = fastq_new(input_file_name);
	if (f == NULL) {
		fprintf(stderr, "failed to open %s for reading\n", input_file_name);
		return false;
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
			return false;
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
	return true;
}