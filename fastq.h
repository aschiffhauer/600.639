#ifndef _FASTQ_H_
#define _FASTQ_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_READ_LENGTH 100 // The maximum size of ANY read. This is a global parameter. Do not change!
#define BUFFER_LENGTH (MAX_READ_LENGTH + 2) // Provides cushion for new line characters and null termination

typedef struct {
	FILE *file;      // file handle to fastq file
	char *sequence;  // the sequence currently in-memory
	char *qualities; // the qualities currently in-memory
} fastq;

// Generates a fastq file at path with n random reads of size MAX_READ_LENGTH (and invalid phred scores).
fastq *fastq_generate(const char *path, int n);
// Creates a new fastq with a read-only file handle at the designated path
fastq *fastq_new(const char *path);
// Reads in the next sequence into f->sequence and qualities into f->qualities.
// Returns true if the next sequential read was available on disk; otherwise false
bool fastq_read(fastq *f); 
// Frees all dynamic memory allocations associated with a fastq (including itself)
void fastq_free(fastq* f);
// A macro that iterates over all kmers in a fastq file that's located at path
#define fastq_for_each_kmer(path, k, x, z) { \
	fastq* f = fastq_new(path); \
	if (f != NULL) { \
		char *(x) = malloc(k + 1); \
		if ((x) == NULL) { \
			free(f); \
		} \
		else { \
			while (fastq_read(f)) { \
				for (int i = 0; i <= MAX_READ_LENGTH - k; i++) { \
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
// A macro that iterates over all kmers of a sequence
#define sequence_for_each_kmer(sequence, k, x, z) { \
	char *(x) = malloc(k + 1); \
	if (x != NULL) { \
		for (int i = 0; i <= MAX_READ_LENGTH - k; i++) { \
			strncpy((x), sequence + i, k); \
			(x)[k] = '\0'; \
			z; \
		} \
		free(x); \
	} \
}

#endif
