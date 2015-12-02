#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__

#include <stdbool.h>

typedef enum {
	BLOOMFILTER, 
	MINSKETCH
} counter;

typedef struct {
	void *data;
	counter type;
} histogram;

histogram *histogram_new(counter type, void *data);
bool histogram_add(histogram *h, const char *path, int k);
int histogram_get(histogram *h, const char *kmer);
void histogram_free(histogram *h);

#endif