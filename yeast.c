#include <stdlib.h>
#include <stdio.h>

#include "histogram.h"
#include "minsketch.h"
#include "fastq.h"
#include "error.h"

// Experiment-related constants (file name, kmer size, min sketch dimensions, etc.)
// 1,380,564 lines / 4 lines per sequence = 345,141 sequences 
// 345,141 sequences * 24 kmers per sequence = 8,283,384 kmers
// 8,283,384 kmers * 8 nucleotides per sequence = 66,267,072 nucleotides
// 66,267,072 nucleotides = 66.2 MB to store all kmers in memory
// However, there are exactly 970,056 unique 8-mers.
// Thus, it takes 970,056 * 9 bytes = 8.730504 megabytes
#define FASTQ_FILE_NAME "yeast.fastq"
#define FASTQ_CORRECTED_FILE_NAME "yeast_corrected.fastq"
#define KMER_SIZE 8
#define MINSKETCH_WIDTH 0x800000 // 1 megabytes (~10% of the memory!)
#define MINSKETCH_HEIGHT 1
#define FREQUENCY_CUTOFF 1

/*
Runs an experiment based off real (i.e. self-generated) reads.
Our hypothesis is that our correction algorithm with find some errors and fix them.
*/
int main(void) {
	// Setup the minsketch-based histogram and read in all the kmers from fastq file
	histogram *h = histogram_new(MINSKETCH, minsketch_new(MINSKETCH_WIDTH, MINSKETCH_HEIGHT));
	histogram_read(h, FASTQ_FILE_NAME, KMER_SIZE);

	// Setup writing to file
	FILE *file = fopen(FASTQ_CORRECTED_FILE_NAME, "w");
	if (file == NULL) {
		fprintf(stderr, "failed to open %s for writing\n", FASTQ_CORRECTED_FILE_NAME);
		return -1;
	}

	// Print out some debug information for the user
	printf("experiment parameters:\n");
	printf("  kmer size: %d\n", KMER_SIZE);
	printf("  frequency cutoff for error correction: %d\n", FREQUENCY_CUTOFF);

	// Iterate through all of the reads of the fastq file, outputting any corrections
	fastq *f = fastq_new(FASTQ_FILE_NAME);
	char sequence_copy[MAX_READ_LENGTH + 1];
	char kmer[KMER_SIZE + 1];
	int corrections = 0;
	int length = 0;
	while (fastq_read(f)) {
		length++;
		strcpy(sequence_copy, f->sequence);
		int i = 0;
		while (error_correct(h, f->sequence, KMER_SIZE, FREQUENCY_CUTOFF)) {
			strncpy(kmer, f->sequence, KMER_SIZE);
			// Guard against infinite looping
			if (++i >= KMER_SIZE) {
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
			fprintf(stderr, "failed to write to %s\n", FASTQ_CORRECTED_FILE_NAME);
			return -1;
			fclose(file);
		}
	}

	// Print out summary
	printf("corrected %f%% of sequences\n", 100 * ((float)(corrections)/((float) (length))));
	printf("%s contains corrected reads\n", FASTQ_CORRECTED_FILE_NAME);
	fclose(file);

	// Clean up file handles and allocations
	fastq_free(f);
	histogram_free(h);
}