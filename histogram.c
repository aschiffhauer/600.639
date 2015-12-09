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

bool histogram_read(histogram *h, const char *path, int k) {
	bool (*add) (void *, const char *);
	switch(h->type) {
		case BLOOMFILTER:
			add = (bool (*)(void *, const char *))bloomfilter_add;
			break;
		case MINSKETCH:
			add = (bool (*)(void *, const char *))minsketch_add;
			break;
		default:
			return false;
	}
	fastq_for_each_kmer(path, k, kmer, {
		add(h->data, kmer);
	});
	return true;
}

int histogram_count(histogram *h, const char *kmer) {
	int (*get) (void *, const char *);
	switch(h->type) {
		case BLOOMFILTER:
			get = (int (*)(void *, const char *))bloomfilter_get;
			break;
		case MINSKETCH:
			get = (int (*)(void *, const char *))minsketch_get;
			break;
		default:
			return 0;
	}
	return get(h->data, kmer);
}

float histogram_load_factor(histogram *h) {
		switch (h->type) {
				case BLOOMFILTER:
						return -1;
				case MINSKETCH:
						return minsketch_load_factor(h->data) * 100.0f;
				default:
						return -1;
		}
}

void histogram_free(histogram *h) {
	if (h != NULL) {
		free(h->data);
	}
	free(h);
}