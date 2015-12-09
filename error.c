#include <stdbool.h>
#include <stdlib.h>

#include "error.h"
#include "fastq.h"
#include "histogram.h"

char *error_correct(histogram *h, char *sequence, int k, int cutoff) {
	char errant_kmer[MAX_READ_LENGTH + 1];
	int start = -1;
	int position = 0;
	sequence_for_each_kmer(sequence, k, kmer, {
		if (histogram_count(h, kmer) <= cutoff) {
			if (start == -1) {
				start = position;
			}
			strcpy(errant_kmer, kmer);
		}
		else if (start >= 0) {
			break;
		}
		position++;
	});
	if (start >= 0) {
		// Calculate position of errant nucleotide
		int end = position;
		printf("position: %d, start: %d, end: %d\n", position, start, end);
		int errant_position = end - 1;
		int max_count = ~0;
		int count = 0;
		char max_nucleotide = '\0';
		// Check count with A
		errant_kmer[errant_position] = 'A';
		if ((count = histogram_count(h, errant_kmer)) > max_count) {
			max_count = count;
			max_nucleotide = 'A';
		}
		// Check count with C
		errant_kmer[errant_position] = 'C';
		if ((count = histogram_count(h, errant_kmer)) > max_count) {
			max_count = count;
			max_nucleotide = 'C';
		}
		// Check count with G
		errant_kmer[errant_position] = 'G';
		if ((count = histogram_count(h, errant_kmer)) > max_count) {
			max_count = count;
			max_nucleotide = 'G';
		}
		// Check count with T
		errant_kmer[errant_position] = 'T';
		if ((count = histogram_count(h, errant_kmer)) > max_count) {
			max_count = count;
			max_nucleotide = 'T';
		}
		sequence[end - 1] = max_nucleotide;
		errant_kmer[errant_position] = max_nucleotide;
		return sequence;
	}
	return NULL;
}