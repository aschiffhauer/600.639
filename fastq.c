#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "fastq.h"

fastq *fastq_new(const char *path) {
	fastq *f = malloc(sizeof *f);
	if (f == NULL) {
		return NULL;
	}
	if ((f->file = fopen(path, "r")) == NULL) {
		free(f);
		return NULL;
	}
	if ((f->sequence = malloc((BUFFER_LENGTH) * sizeof(char))) == NULL) {
		fclose(f->file);
		free(f);
		return NULL;
	}
	if ((f->qualities = malloc((BUFFER_LENGTH) * sizeof(int))) == NULL) {
		fclose(f->file);
		free(f->sequence);
		free(f);
		return NULL;
	}
	return f;
}

bool fastq_read_line(fastq *f) {
	static char temp[MAX_READ_LENGTH + 1];
	if (fgets(temp, BUFFER_LENGTH, f->file) == NULL) {
		return false;
	}
	if (fgets(f->sequence, BUFFER_LENGTH, f->file) == NULL) {
		return false;
	}
	if (fgets(temp, BUFFER_LENGTH, f->file) == NULL) {
		return false;
	}
	if (fgets(f->qualities, BUFFER_LENGTH, f->file) == NULL) {
		return false;
	}
	return true;
}

void fastq_free(fastq* f) {
	fclose(f->file);
	free(f->sequence);
	free(f->qualities);
	free(f);
}