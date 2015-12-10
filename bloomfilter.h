#ifndef _BLOOMFILTER_H
#define _BLOOMFILTER_H

#include <stdbool.h>
#include <math.h>

typedef struct {
	int *bits;             // bit array
	int m;                 // number of bits
	int len;               // length of bit array (i.e. m/sizeof(int))
	int k;                 // number of hash functions
	unsigned int *hashes;  // hash functions
} bloomfilter;

// Create a new bloomfilter with m bits and k hash functions
bloomfilter *bloomfilter_new(int m, int k);
// Adds a string to a bloomfilter
bool bloomfilter_add(bloomfilter *b, const char *str);
// Queries whether a string appears in a bloomfilter
bool bloomfilter_get(bloomfilter *b, const char *str);
// Frees all dynamic memory allocations associated with a bloomfilter (including itself)
void bloomfilter_free(bloomfilter *b);

#endif