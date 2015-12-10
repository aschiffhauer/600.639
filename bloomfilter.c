#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "bloomfilter.h"
#include "hash.h"

// Create a new bloomfilter with m bits and k hash functions
bloomfilter *bloomfilter_new(int m, int k) {
	bloomfilter *b = malloc(sizeof *b);
	if (b == NULL) {
		return NULL;
	}
	b->m = m;
	b->len = b->m/(sizeof *b->hashes * 8) + 1;
	b->bits = calloc(b->len, (sizeof *b->bits));
	if (b->bits == NULL) {
		free(b);
		return NULL;
	}
	b->k = k;
	if ((b->hashes = malloc(b->k * (sizeof *b->hashes))) == NULL) {
		free(b->bits);
		free(b);
		return NULL;
	}
	for (int i = 0; i < b->k; i++) {
		b->hashes[i] = rand();
	}
	return b;
}

// Adds a string to a bloomfilter
bool bloomfilter_add(bloomfilter *b, const char *str) {
	for (int i = 0; i < b->k; i++) {
		int bit = hash(str, b->hashes[i]) % b->m;
		int index = bit / ((sizeof *b->bits) * 8);
		int offset = bit % ((sizeof *b->bits) * 8);
		b->bits[index] = b->bits[index] | (1 << offset);
	}
	return true;
}

// Queries whether a string appears in a bloomfilter
bool bloomfilter_get(bloomfilter *b, const char *str) {
	for (int i = 0; i < b->k; i++) {
		int bit = hash(str, b->hashes[i]) % b->m;
		int index = bit / ((sizeof *b->bits) * 8);
		int offset = bit % ((sizeof *b->bits) * 8);
		if (((b->bits[index]) & (1 << offset)) == 0) {
			return false;
		}
	}
	return true;
}

// Gets the load factor (%) of a bloomfilter
float bloomfilter_load_factor(bloomfilter *b) {
	float hits;
	for (int bit = 0; bit < b->m; bit++) {
		int index = bit / ((sizeof *b->bits) * 8);
		int offset = bit % ((sizeof *b->bits) * 8);
		hits += ((b->bits[index]) & (1 << offset));
	}
	return hits/b->m;
}

// Frees all dynamic memory allocations associated with a bloomfilter (including itself)
void bloomfilter_free(bloomfilter *b) {
	free(b->bits);
	free(b->hashes);
	free(b);
}
