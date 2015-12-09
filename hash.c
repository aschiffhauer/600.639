#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "hash.h"

#define NELEM(x) (sizeof(x) / sizeof((x)[0]))

unsigned long long hash(const char *str, int seed) {
	if (!*str) {
		return 0;
	}
	unsigned long long hash = seed;
	for (int i = 0, n = strlen(str); i < n; i++) {
		switch (str[i]) {
			case 'A':
				hash = (hash << 3) + A;
				break;
			case 'C':
				hash = (hash << 3) + C;
				break;
			case 'G':
				hash = (hash << 3) + G;
				break;
			case 'T':
				hash = (hash << 3) + T;
				break;
			default:
				hash += str[i];
				break;
		}
	}
	if (strlen(str) < 5) {
			//printf("h(%s): %llu\n", str, hash);
	}
	return hash;
}