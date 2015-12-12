#include <stdlib.h>

#include "algorithm.h"

// Experiment-related constants (file name, kmer size, min sketch dimensions, etc.)
// 1,380,564 lines / 4 lines per sequence = 345,141 sequences 
// 345,141 sequences * 24 kmers per sequence = 8,283,384 kmers
// 8,283,384 kmers * 8 nucleotides per sequence = 66,267,072 nucleotides
// 66,267,072 nucleotides = 66.2 MB to store all kmers in memory
// However, there are exactly 71,031 unique 8-mers.
// Thus, it takes 71,031 * 9 bytes = 639 kilobytes
#define FASTQ_INPUT_NAME "yeast.fastq"
#define FASTQ_OUTPUT_NAME "yeast_corrected.fastq"
#define KMER_SIZE 8
#define MINSKETCH_WIDTH 0x40000 // 262 kilobytes (~40% of the memory!)
#define MINSKETCH_HEIGHT 1
#define FREQUENCY_CUTOFF 1

/*
Runs an experiment based off real (i.e. self-generated) reads.
Our hypothesis is that our correction algorithm with find some errors and fix them.
*/
int main(void) {
	if (algorithm_correct(FASTQ_INPUT_NAME, FASTQ_OUTPUT_NAME, KMER_SIZE, MINSKETCH_WIDTH, MINSKETCH_HEIGHT, FREQUENCY_CUTOFF) == false) {
		return EXIT_FAILURE;
	}
}