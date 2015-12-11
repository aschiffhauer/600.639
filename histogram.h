#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__

#include <stdbool.h>

typedef enum {
	BLOOMFILTER, // Denotes a bloomfilter
	MINSKETCH    // Denotes a minsketch
} counter;

typedef struct {
	void *data;    // The allocated data structure
	counter type;  // The type of the allocated data structure
} histogram;

// Creates a new histogram around an already allocated bloomfilter of minsketch.
histogram *histogram_new(counter type, void *data);
// Reads the all the kmers (length k) from a file at path into a histogram.
// Returns true if successful; false otherwise
bool histogram_read(histogram *h, const char *path, int k);
// Returns an estimated count of occurences a kmer occurs in the underlying data structure of a histogram
int histogram_count(histogram *h, const char *kmer);
// Gets the load factor (%) of the underlying data structure of a histogram
float histogram_load_factor(histogram *h);
// Frees any memory used by a histogram (including the underlying data structure and itself)
void histogram_free(histogram *h);

#endif