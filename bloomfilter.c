#include <stdlib.h>
#include <string.h>

#include "bloomfilter.h"
#include "hash.h"

bloomfilter *bloomfilter_new(int m, int k) {
	bloomfilter *b = malloc(sizeof *b);
	if (b == NULL) {
		return NULL;
	}
	b->k = k;
	b->len = m/sizeof(int) + 1;
	b->bits = calloc(b->len, sizeof *b->bits);
	if (b->bits == NULL) {
		free(b);
		return NULL;
	}
	return b;
}

bool bloomfilter_add(bloomfilter *b, const char *str) {
	for (int i = 0; i < b->len; i++) {
		int val = hash(str, i);
		int index = val / sizeof(int);
		int offset = val % index;
		b->bits[index] = b->bits[index] | (1 << offset);
	}
	return true;
}

bool bloomfilter_get(bloomfilter *b, const char *str) {
	for (int i = 0; i < b->len; i++) {
		int val = hash(str, i);
		int index = val / sizeof(int);
		int offset = val % index;
		if (!(b->bits[index] & (1 << offset))) {
			return false;
		}
	}
	return true;
}

void bloomfilter_free(bloomfilter *b) {
	free(b->bits);
	free(b);
}
