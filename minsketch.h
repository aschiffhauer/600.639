#ifndef _MINSKETCH_H
#define _MINSKETCH_H

#include <stdbool.h>

typedef struct {
	int w;                 // width
	int d;                 // height (number of hash functions)
	int **rows;            // matrix
	unsigned int *hashes;  // hash functions
} minsketch;

// Create a new bloomfilter with m bits and k hash functions
minsketch* minsketch_new(int w, int d);
// Adds a string to a bloomfilter
bool minsketch_add(minsketch *m, const char *str);
// Queries whether a string appears in a bloomfilter
int minsketch_get(minsketch *m, const char *str);
// Gets the load factor (%) of a bloomfilter
float minsketch_load_factor(minsketch *m);
// Frees all dynamic memory allocations associated with a bloomfilter (including itself)
void minsketch_free(minsketch *m);

#endif
