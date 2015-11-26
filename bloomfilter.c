#include <stdlib.h>
#include <string.h>

#include "bloomfilter.h"
#include "hash.h"

bloomfilter *bloomfilter_new(int m, int k) {
	bloomfilter *b = malloc(sizeof b);
	if (b == NULL) {
		return 0;
	}
	memset(b, 0, sizeof *b);
	b->k = k;
	int len = m/sizeof(int) + 1;
	b->len = len;
	b->bit_array = malloc(sizeof(int) * b->len);
	if (b->bit_array == NULL) {
		free(b);
		return 0;
	}
	memset(b->bit_array, 0, sizeof(int) * b->len);
	return b;
}

bool bloomfilter_add(bloomfilter *b, const char *str) {
	if (b == NULL || str == NULL) {
		return false;
	}
	for (int i = 0; i < b->len; i++) {
		int val = hash(str, i);
		int index = val / sizeof(int);
		int offset = val % index;
		b->bit_array[index] = b->bit_array[index] | (1 << offset);
	}
	return true;
}

bool bloomfilter_get(bloomfilter *b, const char *str) {
	for (int i = 0; i < b->len; i++) {
		int val = hash(str, i);
		int index = val / sizeof(int);
		int offset = val % index;
		if (b->bit_array[index] & (1 << offset)) {
			return true;
		}
	}
	return false;
}

void bloomfilter_free(bloomfilter *b) {
	memset(b->bit_array, 0, sizeof(int) * b->len);
	free(b);
}
