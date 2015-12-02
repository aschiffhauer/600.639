#include <stdbool.h>
#include <stdio.h>

#include "fastq.h"
#include "fastqtests.h"
#include "minsketch.h"
#include "bloomfilter.h"
#include "tests.h"

#define USING(x, y) fastq *f = fastq_new("reads.fastq"); ASSERT(f != NULL); y; fastq_free(f);

#define READ fastq_read_line(f)
#define SEQUENCE f->sequence
#define QUALITIES f->qualities

TEST(fastq_test1(), {
	USING("reads.fastq", {
		while (READ) {
			ASSERT(SEQUENCE[0] != '\0');
			ASSERT(QUALITIES[0] != '\0');
		}
	});
})

TEST(fastq_test2(), {
	minsketch *m = minsketch_new(100, 100);
	USING("reads.fastq", {
		while (READ) {
			minsketch_add(m, SEQUENCE);
		}
	});
	minsketch_free(m);
})

TEST(fastq_test3(), {
	bloomfilter *b = bloomfilter_new(100, 100);
	USING("reads.fastq", {
		while (READ) {
			bloomfilter_add(b, SEQUENCE);
		}
	});
	bloomfilter_free(b);
})

TEST(fastq_test(), {
	ASSERT(fastq_test1() == true);
	ASSERT(fastq_test2() == true);
	ASSERT(fastq_test3() == true);
})
