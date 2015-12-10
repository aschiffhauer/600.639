#include <stdlib.h>
#include <string.h>

#include "bloomfilter.h"
#include "hash.h"

// Create a new bloomfilter with m bits and k hash functions
bloomfilter *bloomfilter_new(int m, int k) {
	bloomfilter *b = malloc(sizeof *b);
	if (b == NULL) {
		return NULL;
	}
	b->k = k;
	b->len = m/sizeof *b->hashes + 1;
	b->bits = calloc(b->len, sizeof *b->bits);
	if (b->bits == NULL) {
		free(b);
		return NULL;
	}
	if ((b->hashes = malloc(k * sizeof *b->hashes)) == NULL) {
		free(b->bits);
		free(b);
		return NULL;
	}
	for (int i = 0; i < b->k; i++) {
		b->hashes[i] = rand();
	}
	return b;
}

// Adds a string to the bloom filter
bool bloomfilter_add(bloomfilter *b, const char *str) {
	for (int i = 0; i < b->k; i++) {
		int bit = hash(str, b->hashes[i]) % (b->len * sizeof *b->bits);
		int index = bit / sizeof *b->bits;
		int offset = bit % sizeof *b->bits;
		b->bits[index] = b->bits[index] | (1 << offset);
	}
	return true;
}

bool bloomfilter_get(bloomfilter *b, const char *str) {
	for (int i = 0; i < b->len; i++) {
		int bit = hash(str, b->hashes[i]) % (b->len * sizeof(int));
		int index = bit / sizeof(int);
		int offset = bit % sizeof(int);
		if (!(b->bits[index] & (1 << offset))) {
			return false;
		}
	}
	return true;
}

void bloomfilter_free(bloomfilter *b) {
	free(b->bits);
	free(b->hashes);
	free(b);
}
