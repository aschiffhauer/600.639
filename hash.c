#include <stdlib.h>
#include <string.h>

#define NELEM(x) (sizeof(x) / sizeof((x)[0]))
#define A 0  // 00
#define C 1  // 01
#define G 2  // 10
#define T 3  // 11

unsigned long hash(const char *str, int seed) {
	if (!*str) {
		return 0;
	}
	unsigned long hash = 0;
	for (int i = 0; i < strlen(str); i++) {
		hash <<= 2;
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
	hash += seed;
	return hash;
}