#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "minsketch.h"
#include "hash.h"

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

static int minsketch_hash(minsketch *m, const char *str, int row);

minsketch* minsketch_new(int w, int d) {
	minsketch *m = malloc(sizeof *m);
	if (m == NULL) {
		return NULL;
	}
	m->w = w;
	m->d = d;
	if ((m->hashes = malloc(m->d * sizeof *m->hashes)) == NULL) {
		free(m);
		return NULL;
	}
	for (int i = 0; i < m->d; i++) {
		m->hashes[i] = rand();
	}
	m->rows = malloc(m->d * sizeof *m->rows);
	if (m->rows == NULL) {
		free(m->hashes);
		free(m);
		return NULL;
	}
	for (int i = 0; i < m->d; i++) {
		if ((m->rows[i] = calloc(m->w, sizeof *m->rows[i])) == NULL) {
			for (int j = i; j >= 0; j--) {
				free(m->rows[j]);
			}
			free(m->rows);
			free(m->hashes);
			free(m);
			return NULL;
		}
	}
	return m;
}

bool minsketch_add(minsketch *m, const char *str) {
	for (int i = 0; i < m->d; i++) {
		int j = minsketch_hash(m, str, i);
		m->rows[i][j]++;
	}
	return true;
}

int minsketch_get(minsketch *m, const char *str) {
	int min = ~0U >> 1;
	for (int i = 0; i < m->d; i++) {
		int j = minsketch_hash(m, str, i);
		if ((min = MIN(min, m->rows[i][j])) == 0) {
			return 0;
		}
	}
	return min;
}

float minsketch_load_factor(minsketch *m) {
	int hits = 0;
	for (int i = 0; i < m->d; i++) {
		for (int j = 0; j < m->w; j++) {
			hits += MIN(m->rows[i][j], 1);
		}
	}
	return ((float)hits)/(m->d * m->w);
}

void minsketch_free(minsketch *m) {
	for (int i = 0; i < m->d; i++) {
		free(m->rows[i]);
	}
	free(m->rows);
	free(m->hashes);
	free(m);
}

static int minsketch_hash(minsketch *m, const char *str, int row) {
	return hash(str, m->hashes[row])%(m->w);
}
