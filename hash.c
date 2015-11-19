#include <stdlib.h>

#define PRIME 5381

int hash(const char *str) {
	if (str == NULL) {
		return 0;
	}
	int hash = PRIME;
	while (*str++) {
		hash = ((hash << 5) + hash) + *str;
	}
	return hash;
}