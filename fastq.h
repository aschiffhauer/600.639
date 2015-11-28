#ifndef _FASTQ_H_
#define _FASTQ_H_

#include <stdbool.h>
#include <stdio.h>

#define MAX_READ_LENGTH 100
#define BUFFER_LENGTH (MAX_READ_LENGTH + 2)

typedef struct {
	FILE *file;
	char* sequence;
	int* qualities;
} fastq;

fastq *fastq_new(const char *path);
bool fastq_read_line(fastq *f); 
void fastq_free(fastq* f);

#endif
