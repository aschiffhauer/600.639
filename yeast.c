#include <stdlib.h>
#include <stdio.h>

#include "histogram.h"
#include "minsketch.h"
#include "fastq.h"
#include "error.h"

// Experiment-related constants (file name, kmer size, min sketch dimensions, etc.)
// 1,380,564 lines / 4 lines per sequence = 345,141 sequences 
// 345,141 sequences * 16 kmers per sequence = 5,522,256 kmers
// 5,522,256 kmers * 16 nucleotides per sequence = 88,356,096 nucleotides
// 88,356,096 nucleotides = 88.4 MB to store all kmers in memory
#define FASTQ_FILE_NAME "yeast.fastq"
#define KMER_SIZE 8
#define MINSKETCH_WIDTH 0x800000 // 8 megabytes (~10% of the memory!)
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

	// Print out some debug information for the user
	printf("experiment parameters:\n");
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
		int corrected = false;
		while (error_correct(h, f->sequence, KMER_SIZE, FREQUENCY_CUTOFF)) {
			corrections++;
			corrected = true;
			strncpy(kmer, f->sequence, KMER_SIZE);
			// Infinite loop
			if (++i >= KMER_SIZE) {
				break;
			}
		}
		if (corrected) {
			printf("correction:\n  old: %s\n  new: %s\n", sequence_copy, f->sequence);
		}
	}
	
	printf("corrected %f%% of sequences\n", 100 * ((float)(corrections)/((float) (length))));

	// Clean up file handles and allocations
	fastq_free(f);
	histogram_free(h);
}