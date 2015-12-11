#include <stdlib.h>
#include <stdio.h>

#include "histogram.h"
#include "minsketch.h"
#include "fastq.h"
#include "error.h"

// Experiment-related constants (file name, kmer size, min sketch dimensions, etc.)
// 12,800 lines / 4 lines per sequence = 3,200 sequences 
// 3,200 sequences * 90 kmers per sequence = 288,000 kmers
// 288,000 kmers * 10 nucleotides per sequence = 2,880,000 nucleotides
// 2,880,000 nucleotides = 2.88 MB to store all kmers in memory
#define FASTQ_FILE_NAME "synthetic.fastq"
#define KMER_SIZE 10
#define MINSKETCH_WIDTH 8192*2*2*2
#define MINSKETCH_HEIGHT 4
#define FREQUENCY_CUTOFF 1
#define CORRECT_KMER    "CCCCCGTGAA"
#define INCORRECT_KMER  "CCCCCGTGAT"

/*
Runs an experiment based off synthetic (i.e. self-generated) reads.
A nucleotide in a kmer that appears frequently was maliciously changed in exactly one read.
The read changed:  CGTTTGGACCCTTTCAATGCTGATGGACTCACTTGACTTTTGTATGCCCAAAACCTTGAAACCCTTCAGAGACACACTCAGACGGCCCCCGTGA----A----GTGCT (correct)
The resuling read: CGTTTGGACCCTTTCAATGCTGATGGACTCACTTGACTTTTGTATGCCCAAAACCTTGAAACCCTTCAGAGACACACTCAGACGGCCCCCGTGA----T----GTGCT (incorrect)
Our hypothesis is that our correction algorithm with find this error and fix it to the original read.
*/
int main(void) {
	// Setup the minsketch-based histogram and read in all the kmers from fastq file
	histogram *h = histogram_new(MINSKETCH, minsketch_new(MINSKETCH_WIDTH, MINSKETCH_HEIGHT));
	histogram_read(h, FASTQ_FILE_NAME, KMER_SIZE);

	// Print out some debug information for the user
	printf("experiment parameters:\n");
	printf("  estimated frequency of \"correct\" kmer: %d\n", histogram_count(h, CORRECT_KMER));
	printf("  estimated frequency of \"incorrect\" kmer: %d\n", histogram_count(h, INCORRECT_KMER));
	printf("  frequency cutoff for error correction: %d\n", FREQUENCY_CUTOFF);

	// Iterate through all of the reads of the fastq file, outputting any corrections
	fastq *f = fastq_new(FASTQ_FILE_NAME);
	char sequence_copy[MAX_READ_LENGTH + 1];
	char kmer[KMER_SIZE + 1];
	while (fastq_read(f)) {
		strcpy(sequence_copy, f->sequence);
		if (error_correct(h, f->sequence, KMER_SIZE, FREQUENCY_CUTOFF)) {
			strncpy(kmer, f->sequence, KMER_SIZE);
			printf("correction:\n  old: %s\n  new: %s\n", sequence_copy, f->sequence);
		}
	}

	// Clean up file handles and allocations
	fastq_free(f);
	histogram_free(h);
}