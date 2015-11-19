#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "minsketch.h"
#include "hash.h"

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

static int minsketch_hash(minsketch *m, const char *str, int row);

minsketch* minsketch_new(int w, int d) {
	minsketch *m = malloc(sizeof (minsketch));
	if (m == NULL) {
		return NULL;
	}
	m->w = w;
	m->d = d;
	m->rows = malloc(sizeof(int) * d);
	if (m->rows == NULL) {
		free(m);
		return NULL;
	}
	for (int i = 0; i < d; i++) {
		if ((m->rows[i] = malloc(sizeof(int) * w)) == 0) {
			for (int j = i; j >= 0; j--) {
				free(m->rows[j]);
			}
			free(m->rows);
			free(m);
			return NULL;
		}
		memset(m->rows[i], 0, d * sizeof(int));
	}
	return m;
}

bool minsketch_add(minsketch *m, const char *str) {
	if (m == NULL || str == NULL) {
		return false;
	}
	for (int i = 0; i < m->d; i++) {
		int j = minsketch_hash(m, str, i);
		m->rows[i][j]++;
	}
	return true;
}

int minsketch_get(minsketch *m, const char *str) {
	if (m == NULL || str == NULL) {
		return -1;
	}
	int min = ~0U >> 1;
	for (int i = 0; i < m->d; i++) {
		int j = minsketch_hash(m, str, i);
		min = MIN(min, m->rows[i][j]);
		if (min == 0) {
			return 0;
		}
	}
	return min;
}

void minsketch_free(minsketch *m) {
	for (int i = 0; i < m->d; i++) {
		free(m->rows[i]);
	}
	free(m->rows);
	free(m);
}

static int minsketch_hash(minsketch *m, const char *str, int row) {
	return (hash(str) + row)%(m->d);
}
