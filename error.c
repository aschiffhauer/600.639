#include <stdbool.h>
#include <stdlib.h>

#include "error.h"
#include "fastq.h"
#include "histogram.h"

bool error_detect(histogram *h, char *sequence, int k, int cutoff) {
	bool error = false;
	sequence_for_each_kmer(sequence, k, kmer, {
		if (histogram_count(h, kmer) <= cutoff) {
			error = true;
			break;
		}
	});
	return error;
}

char *error_correct(histogram *h, char *sequence, int k, int cutoff) {
	if (error_detect(h, sequence, k, cutoff) == false) {
		return NULL;
	}
	return NULL;
}