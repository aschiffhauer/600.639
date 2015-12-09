#ifndef _BLOOMFILTER_H
#define _BLOOMFILTER_H

#include <stdbool.h>
#include <math.h>

#define OPT_BITS(n, p) (int)(-((n) * log((p)))/(0.48045301391))
#define OPT_HASH(m, n) (int)(0.69314718056 * ((m)/(n)))
#define CLOSEST_POW2(x) {( \
	(x) = (x) - 1; \
	(x) = (x) | ((x) >> 1); \
	(x) = (x) | ((x) >> 2); \
	(x) = (x) | ((x) >> 4); \
	(x) = (x) | ((x) >> 8); \
	(x) + 1; \
)}

typedef struct {
	int *bits;
	int k;
	int len;
	unsigned int *hashes;
} bloomfilter;

bloomfilter *bloomfilter_new(int m, int k);
bool bloomfilter_add(bloomfilter *b, const char *str);
bool bloomfilter_get(bloomfilter *b, const char *str);
void bloomfilter_free(bloomfilter *b);

#endif