#include <stdbool.h>
#include <stdio.h>

#include "fastq.h"
#include "fastqtests.h"
#include "minsketch.h"
#include "bloomfilter.h"
#include "tests.h"

#define CREATE(x) fastq *f = fastq_new("reads.fastq"); ASSERT(f != NULL);
#define FINISH(...) PRINT(__VA_ARGS__); fastq_free(f); return true;

#define READ fastq_read_line(f)
#define SEQUENCE f->sequence
#define QUALITIES f->qualities

static bool fastq_test1() {
	CREATE("reads.fastq");
	while (READ) {
		ASSERT(SEQUENCE[0] != '\0');
		ASSERT(QUALITIES[0] != '\0');
	}
	FINISH("passed fastq_test1");
}

static bool fastq_test2() {
	minsketch *m = minsketch_new(100, 100);
	CREATE("reads.fastq");
	while (READ) {
		minsketch_add(m, SEQUENCE);
	}
	minsketch_free(m);
	FINISH("passed fastq_test2");
}

static bool fastq_test3() {
	bloomfilter *b = bloomfilter_new(100, 100);
	CREATE("reads.fastq");
	while (READ) {
		bloomfilter_add(b, SEQUENCE);
	}
	bloomfilter_free(b);
	FINISH("passed fastq_test3");
}

bool fastq_test() {
	ASSERT(fastq_test1() == true);
	ASSERT(fastq_test2() == true);
	ASSERT(fastq_test3() == true);
	PRINT("fastq tests passed");
	return true;
}
