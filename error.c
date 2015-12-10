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
		int sequence_position = position - 1;
		int kmer_position = 0;
		// Is the error is in the last kmer?
		// Feel free to ignore this chunk of code, as it covers an edge case (and does so confusingly)
		if (position - 1 >= MAX_READ_LENGTH - k) {
			int n = (int)strlen(sequence);
			char kmer_copy[MAX_READ_LENGTH + 1];
			for (int i = n - 1, j = k - 1; i >= n - 1 - k; i--, j--) {
				strncpy(kmer_copy, &sequence[i - k + 1], k);
				kmer_copy[k] = '\0';
				if (histogram_count(h, kmer_copy) > cutoff) {
					sequence_position = i + 1; // The *previous* kmer was wrong!
					kmer_position = j + 1;
					break;
				}
			}
		}
		char kmer[MAX_READ_LENGTH + 1];
		memcpy(kmer, &sequence[kmer_offset], k);
		kmer[k] = '\0';

		int count = ~0;
		char nucleotide = '\0';
		error_correct_sequence(h, kmer, kmer_position, &count, &nucleotide, 'A');
		error_correct_sequence(h, kmer, kmer_position, &count, &nucleotide, 'C');
		error_correct_sequence(h, kmer, kmer_position, &count, &nucleotide, 'G');
		error_correct_sequence(h, kmer, kmer_position, &count, &nucleotide, 'T');

		if (sequence[sequence_position] != kmer[kmer_position]) {
			sequence[sequence_position] = kmer[kmer_position];
			return sequence;
		}
	}
	return NULL;
}