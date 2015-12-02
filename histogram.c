#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "histogram.h"
#include "bloomfilter.h"
#include "minsketch.h"
#include "fastq.h"

histogram *histogram_new(counter type, void *data) {
	if (data == NULL) {
		return NULL;
	}
	histogram *h = malloc(sizeof *h);
	if (h == NULL) {
		return NULL;
	}
	h->type = type;
	h->data = data;
	return h;
}

bool histogram_add(histogram *h, const char *path, int k) {
	bool (*add) (void *, const char *);
	switch(h->type) {
		case BLOOMFILTER:
			add = (bool (*)(void *, const char *))bloomfilter_add;
            break;
		case MINSKETCH:
			add = (bool (*)(void *, const char *))minsketch_add;
            break;
		default:
			return NULL;
	}
	fastq* f = fastq_new(path);
	if (f == NULL) {
		return false;
	}
	char *kmer = malloc(k + 1);
	if (kmer == NULL) {
		fastq_free(f);
		return false;
	}
	while (fastq_read_line(f)) {
		for (int i = 0; i < BUFFER_LENGTH - k - 2; i++) {
			strncpy(kmer, f->sequence + i, k);
            kmer[k] = '\0';
			if (add(h->data, kmer) == false) {
				return false;
			}
		}
	}
	free(kmer);
	fastq_free(f);
	return true;
}

int histogram_get(histogram *h, const char *kmer) {
	int (*get) (void *, const char *);
	switch(h->type) {
		case BLOOMFILTER:
			get = (int (*)(void *, const char *))bloomfilter_get;
		case MINSKETCH:
			get = (int (*)(void *, const char *))minsketch_get;
		default:
			return 0;
	}
	return get(h->data, kmer);
}

void histogram_free(histogram *h) {
	if (h != NULL) {
		free(h->data);
	}
	free(h);
}