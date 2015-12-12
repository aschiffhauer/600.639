#include <stdlib.h>

#include "algorithm.h"

// Experiment-related constants (file name, kmer size, min sketch dimensions, etc.)
// 12,800 lines / 4 lines per sequence = 3,200 sequences 
// 3,200 sequences * 90 kmers per sequence = 288,000 kmers
// 288,000 kmers * 10 nucleotides per sequence = 2,880,000 nucleotides
// 2,880,000 nucleotides = 2.88 MB to store all kmers in memory
// Since we duplicated the file 31 times, 2.88 MB/32 yields the expected amount of memory
// (100 KB) to store each references to each (can't forget the terminating '\0').
#define FASTQ_INPUT_NAME "synthetic.fastq"
#define FASTQ_OUTPUT_NAME "synthetic_corrected.fastq"
#define KMER_SIZE 10
#define MINSKETCH_WIDTH 0x8000 // 32.768 kilobytes (~33% of memory!)
#define MINSKETCH_HEIGHT 1
#define FREQUENCY_CUTOFF 1

/*
Runs an experiment based off synthetic (i.e. self-generated) reads.
A nucleotide in a kmer that appears frequently was maliciously changed in exactly one read.
The read changed:  CGTTTGGACCCTTTCAATGCTGATGGACTCACTTGACTTTTGTATGCCCAAAACCTTGAAACCCTTCAGAGACACACTCAGACGGCCCCCGTGA----A----GTGCT (correct)
The resuling read: CGTTTGGACCCTTTCAATGCTGATGGACTCACTTGACTTTTGTATGCCCAAAACCTTGAAACCCTTCAGAGACACACTCAGACGGCCCCCGTGA----T----GTGCT (incorrect)
Our hypothesis is that our correction algorithm with find this error and fix it to the original read.
*/
int main(void) {
	if (algorithm_correct(FASTQ_INPUT_NAME, FASTQ_OUTPUT_NAME, KMER_SIZE, MINSKETCH_WIDTH, MINSKETCH_HEIGHT, FREQUENCY_CUTOFF) == false) {
		return EXIT_FAILURE;
	}
}