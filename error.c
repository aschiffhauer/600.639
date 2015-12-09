#include <stdbool.h>
#include <stdlib.h>

#include "error.h"
#include "fastq.h"
#include "histogram.h"

static void error_correct_sequence(histogram *h, char *kmer, int position, int *max_count, char *max_nucleotide, char nucleotide) {
	char kmer_copy[MAX_READ_LENGTH + 1];
	strcpy(kmer_copy, kmer);
	kmer_copy[position] = nucleotide;
	int count = histogram_count(h, kmer_copy);
	if (count > *max_count) {
		*max_count = count;
		*max_nucleotide = nucleotide;
		kmer[position] = nucleotide;
	}
}

char *error_correct(histogram *h, char *sequence, int k, int cutoff) {
	int kmer_offset = -1;
	int position = 0;
	sequence_for_each_kmer(sequence, k, kmer, {
		if (histogram_count(h, kmer) <= cutoff) {
			if (kmer_offset == -1) {
				kmer_offset = position;
			}
		}
		else if (kmer_offset >= 0) {
			break;
		}
		position++;
	});
	if (kmer_offset >= 0) {
		if (position - 1 >= MAX_READ_LENGTH - k) {
			printf("warning: the last kmer has an errant nucleotide...we can't fix that just yet\n");
		}
		char kmer[MAX_READ_LENGTH + 1];
		memcpy(kmer, &sequence[kmer_offset], k);
		kmer[k] = '\0';

		int count = ~0;
		char nucleotide = '\0';
		error_correct_sequence(h, kmer, 0, &count, &nucleotide, 'A');
		error_correct_sequence(h, kmer, 0, &count, &nucleotide, 'C');
		error_correct_sequence(h, kmer, 0, &count, &nucleotide, 'G');
		error_correct_sequence(h, kmer, 0, &count, &nucleotide, 'T');

		sequence[position - 1] = kmer[0];
		return sequence;
	}
	return NULL;
}