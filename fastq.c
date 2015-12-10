#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "tests.h"
#include "fastq.h"

// Strips all occurences of the newline character
static char *string_strip(char *str) {
	unsigned long i = strlen(str) - 1;
	if ((i > 0) && (str[i] == '\n')) {
		str[i] = '\0';
	}
	return str;
}

// Generates a fastq file at path with n random reads of size MAX_READ_LENGTH (and invalid phred scores).
fastq *fastq_generate(const char *path, int n) {
	static char temp[MAX_READ_LENGTH + 1];
	static char nucleotides[4] = { 'A', 'G', 'C', 'T' };

	// Open a file handle for writing
	FILE *file = fopen(path, "w");
	if (file == NULL) {
		return NULL;
	}

	// Create a fastq file
	fastq *f = fastq_new(path);
	if (f == NULL) {
		fclose(file);
		return NULL;
	}

	// Close the file handle from fastq_new
	fclose(f->file);
	f->file = file;
	
	// Generate the content of the file
	srand((unsigned int)time(NULL));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < MAX_READ_LENGTH; j++) {
			temp[j] = nucleotides[rand() % 4];
		}
		temp[MAX_READ_LENGTH] = '\0';
		fprintf(f->file, "%s\n", "IGNORE");
		fprintf(f->file, "%s\n", temp);
		fprintf(f->file, "%s\n", "IGNORE");
		fprintf(f->file, "%s\n", "IGNORE");
	}
	// Close the write-access file handle
	fclose(f->file);
	// Open a read-only file handle
	if ((f->file = fopen(path, "r")) == NULL) {
		fastq_free(f);
		return NULL;
	}
	return f;
}

// Creates a new fastq with a read-only file handle at the designated path
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
	if ((f->qualities = malloc((BUFFER_LENGTH) * sizeof(char))) == NULL) {
		fclose(f->file);
		free(f->sequence);
		free(f);
		return NULL;
	}
	return f;
}

// Reads in the next sequence into f->sequence and qualities into f->qualities.
// Returns true if the next sequential read was available on disk; otherwise false
bool fastq_read(fastq *f) {
	static char temp[MAX_READ_LENGTH + 1];
	if (fgets(temp, BUFFER_LENGTH, f->file) == NULL) {
		return false;
	}
	if (fgets(f->sequence, BUFFER_LENGTH, f->file) == NULL) {
		return false;
	}
	if (string_strip(f->sequence) == NULL) {
		return false;
	}
	if (fgets(temp, BUFFER_LENGTH, f->file) == NULL) {
		return false;
	}
	if (fgets(f->qualities, BUFFER_LENGTH, f->file) == NULL) {
		return false;
	}
	if (string_strip(f->qualities) == NULL) {
		return false;
	}
	return true;
}

// Frees all dynamic memory allocations associated with a fastq (including itself)
void fastq_free(fastq* f) {
	fclose(f->file);
	free(f->sequence);
	free(f->qualities);
	free(f);
}