#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "hash.h"

#define NELEM(x) (sizeof(x) / sizeof((x)[0]))

unsigned long long hash(const char *str, int seed) {
	if (!*str) {
		return 0;
	}
	unsigned long long hash = 0x1;
	for (int i = 0, n = strlen(str); i < n; i++) {
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