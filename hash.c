#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "hash.h"

// Returns the djb2 hash of a string
static inline unsigned int djb2(const char *str) {
	unsigned int hash = 5381;
	while (*str++) {
		hash = ((hash << 5) + hash) + *(str - 1);
	}
	return hash;
}

// Returns the binary representation of the kmer
// Max size of the kmer is MAX_KMER_SIZE, otherwise there will be collisions.
// Further, only A, C, G, and T are supported
static inline unsigned int kmer_to_int(const char *str) {
	if (!*str) {
		return 0;
	}
	unsigned int hash = 0;
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

// Hashes a kmer; h(x) = ((djb2(x) + binary(x)) ^ seed)
unsigned int hash(const char *str, unsigned int seed) {
	unsigned int hash = djb2(str) + kmer_to_int(str);
	return hash ^ seed;
}