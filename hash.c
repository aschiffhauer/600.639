#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "hash.h"

#define NELEM(x) (sizeof(x) / sizeof((x)[0]))

static inline unsigned int djb2(const char *str) {
	unsigned long hash = 5381;
	while (*str++) {
		hash = ((hash << 5) + hash) + *(str - 1);
	}
	return (unsigned int)hash;
}

static inline unsigned int kmer_to_int(const char *str) {
	if (!*str) {
		return 0;
	}
	unsigned int hash = 0x1;
	unsigned long n = strlen(str);
	for (unsigned int i = 0; i < n; i++) {
		hash <<= BITS_PER_CHAR;
		switch (str[i]) {
			case 'A':
				hash += A;
				break;
			case 'C':
				hash += C;
				break;
			case 'G':
				hash += G;
				break;
			case 'T':
				hash += T;
				break;
			default:
				hash += str[i];
				break;
		}
	}
	return hash;
}

unsigned int hash(const char *str, unsigned int seed) {
	unsigned int hash = djb2(str) + kmer_to_int(str);
	return hash ^ seed;
}