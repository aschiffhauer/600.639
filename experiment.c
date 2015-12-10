#include <stdlib.h>
#include <stdio.h>

#include "experiment.h"
#include "histogram.h"
#include "minsketch.h"
#include "fastq.h"
#include "error.h"

#define KMER_SIZE 10
#define FASTQ_FILE_NAME "reads_experiment.fastq"
#define MINSKETCH_WIDTH 10000
#define MINSKETCH_HEIGHT 3
#define KMER_CUTOFF 1

#define CORRECT_KMER "CCCCCGTGAA"
#define ERRANT_KMER  "CCCCCGTGAT"

#define PRINT_CORRECTIONS true
#define PRINT_CORRECT_VS_ERRANT false

// CGTTTGGACCCTTTCAATGCTGATGGACTCACTTGACTTTTGTATGCCCAAAACCTTGAAACCCTTCAGAGACACACTCAGACGGCCCCCGTGAAGTGCT
// was changed to
// CGTTTGGACCCTTTCAATGCTGATGGACTCACTTGACTTTTGTATGCCCAAAACCTTGAAACCCTTCAGAGACACACTCAGACGGCCCCCGTGATGTGCT
void experiment_run() {
	histogram *h = histogram_new(MINSKETCH, minsketch_new(MINSKETCH_WIDTH, MINSKETCH_HEIGHT));
	histogram_read(h, FASTQ_FILE_NAME, KMER_SIZE);
	fastq *f = fastq_new(FASTQ_FILE_NAME);
	char sequence_copy[MAX_READ_LENGTH + 1];
	while (fastq_read_line(f)) {
		strcpy(sequence_copy, f->sequence);
		if (error_correct(h, f->sequence, KMER_SIZE, KMER_CUTOFF)) {
			#if PRINT_CORRECTIONS
				printf("correction:\n  old: %s\n  new: %s\n", sequence_copy, f->sequence);
			#endif
		}
	}
	
	#if PRINT_CORRECT_VS_ERRANT
		printf("correct: %d\n", histogram_count(h, CORRECT_KMER));
		printf("errant: %d\n", histogram_count(h, ERRANT_KMER));
	#endif

	fastq_free(f);
	histogram_free(h);
}