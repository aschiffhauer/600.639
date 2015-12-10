#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "histogram.h"
#include "bloomfilter.h"
#include "minsketch.h"
#include "fastq.h"

// Creates a new histogram around an already allocated bloomfilter of minsketch.
histogram *histogram_new(counter type, void *data) {
	if (data == NULL) {
		return NULL;
	}
	histogram *h = malloc(sizeof *h);
	if (h == NULL) {
		return NULL;
	}
	h->type = type;
	h->data = data;
	return h;
}

// Reads the all the kmers (length k) from a file at path into a histogram.
// Returns true if successful; false otherwise
bool histogram_read(histogram *h, const char *path, int k) {
	bool (*add) (void *, const char *);
	switch(h->type) {
		case BLOOMFILTER:
			add = (bool (*)(void *, const char *))bloomfilter_add;
			break;
		case MINSKETCH:
			add = (bool (*)(void *, const char *))minsketch_add;
			break;
		default:
			return false;
	}
	// Reading can fail (if it doesn't enter the lambda, then it failed)
	bool read = false;
	fastq_for_each_kmer(path, k, kmer, {
		read = true;
		add(h->data, kmer);
	});
	return read;
}

// Returns an estimated count of occurences a kmer occurs in the underlying data structure of a histogram
int histogram_count(histogram *h, const char *kmer) {
	int (*get) (void *, const char *);
	switch(h->type) {
		case BLOOMFILTER:
			get = (int (*)(void *, const char *))bloomfilter_get;
			break;
		case MINSKETCH:
			get = (int (*)(void *, const char *))minsketch_get;
			break;
		default:
			return 0;
	}
	return get(h->data, kmer);
}

// Gets the load factor (%) of the underlying data structure of a histogram
float histogram_load_factor(histogram *h) {
	switch (h->type) {
		case BLOOMFILTER:
			return bloomfilter_load_factor(h->data) * 100.0f;
		case MINSKETCH:
			return minsketch_load_factor(h->data) * 100.0f;
		default:
			return -1;
	}
}

// Frees any memory used by a histogram (including the underlying data structure and itself)
void histogram_free(histogram *h) {
	switch(h->type) {
		case BLOOMFILTER:
			bloomfilter_free(h->data);
			break;
		case MINSKETCH:
			minsketch_free(h->data);
			break;
		default:
			free(h->data);
			break;
	}
	free(h);
}