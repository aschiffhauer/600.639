#include <stdbool.h>
#include <stdlib.h>

#include "error.h"
#include "fastq.h"
#include "histogram.h"

// kmer[position] = nucleotide if the frequency it appears in histogram is greater than max_count.
// max_count and max_nucleotide are updated if the swap occurs
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

// Given a pre-populated histogram, a sequence, the kmer length for the sequence, and a 'cutoff' frequency: 
// returns and modifies the sequence if an error is detected;
// otherwise, returns NULL and does not modify the sequence.
char *error_correct(histogram *h, char *sequence, int k, int cutoff) {
	// Find the "start" index where the frequency drops at or below cutoff. 
	// Then find the "end" index where the frequency recovers and rises above cutoff.
	// Using these two indices, we can determine the index of the errant nucleotide.
	// We greedily correct the error, choosing the nucleotide that maximizes the frequency in the histogram.
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
		// Copy the errant kmer into its own string
		char kmer[MAX_READ_LENGTH + 1];
		memcpy(kmer, &sequence[kmer_offset], k);
		kmer[k] = '\0';
		// Calculate the position of the errant nucleotide in the sequence and the kmer
		int sequence_position = position - 1;
		int kmer_position = 0;
		// Is the errant nucleotide in the last kmer?
		// Feel free to ignore this chunk of code.
		// It covers an (important) edge case, but it isn't very straightforward.
		// It updates sequence_position and kmer_position to be correct when this occurs.
		if (position - 1 >= MAX_READ_LENGTH - k) {
			int n = (int) strlen(sequence);
			char kmer_copy[MAX_READ_LENGTH + 1];
			for (int i = n - 1, j = k - 1; i >= n - 1 - k; i--, j--) {
				strncpy(kmer_copy, &sequence[i - k + 1], k);
				kmer_copy[k] = '\0';
				if (histogram_count(h, kmer_copy) > cutoff) {
					sequence_position = i + 1; // The *previous* kmer was wrong! (hance the + 1)
					kmer_position = j + 1; // The *previous* kmer was wrong! (hence the + 1)
					break;
				}
			}
		}
		// At this point, both sequence_position and kmer_position are correct
		// We maximize the frequency the kmer occurs, trying all values of A, C, G, or T
		int count = ~0;
		char nucleotide = '\0';
		error_correct_sequence(h, kmer, kmer_position, &count, &nucleotide, 'A');
		error_correct_sequence(h, kmer, kmer_position, &count, &nucleotide, 'C');
		error_correct_sequence(h, kmer, kmer_position, &count, &nucleotide, 'G');
		error_correct_sequence(h, kmer, kmer_position, &count, &nucleotide, 'T');

		// Only correct the sequence if we change the nucleotide; otherwise, this was all for naught
		// This occurs when all other combinations are at least as unlikely.
		// No confident correction can be made, and this nothing is corrected.
		if (sequence[sequence_position] != kmer[kmer_position]) {
			sequence[sequence_position] = kmer[kmer_position];
			return sequence;
		}
	}
	return NULL;
}