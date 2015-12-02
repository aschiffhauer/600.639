#ifndef _FASTQ_H_
#define _FASTQ_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_READ_LENGTH 100
#define BUFFER_LENGTH (MAX_READ_LENGTH + 2)

typedef struct {
	FILE *file;
	char *sequence;
	char *qualities;
} fastq;

fastq *fastq_new(const char *path);
bool fastq_read_line(fastq *f); 
void fastq_free(fastq* f);
#define fastq_for_each_kmer(path, k, x, z) { \
	fastq* f = fastq_new(path); \
	if (f != NULL) { \
		char *(x) = malloc(k + 1); \
		if ((x) == NULL) { \
			free(f); \
		} \
		else { \
			while (fastq_read_line(f)) { \
				for (int i = 0; i < BUFFER_LENGTH - k - 2; i++) { \
					strncpy((x), f->sequence + i, k); \
					(x)[k] = '\0'; \
					z; \
				} \
			} \
			free((x)); \
			fastq_free(f); \
		} \
	} \
}

#endif
