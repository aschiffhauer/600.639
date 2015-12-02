#include <stdbool.h>
#include <stdio.h>

#include "fastq.h"
#include "fastqtests.h"
#include "minsketch.h"
#include "bloomfilter.h"
#include "tests.h"

#define KMER_SIZE 10
#define FASTQ_FILE "reads.fastq"
#define USING(x, y) fastq *f = fastq_new(x); ASSERT(f != NULL); y; fastq_free(f);
#define FOR_EACH(x, y) fastq_for_each_kmer(FASTQ_FILE, KMER_SIZE, x, y);

#define READ fastq_read_line(f)
#define SEQUENCE f->sequence
#define QUALITIES f->qualities

TEST(fastq_test1(), {
	USING(FASTQ_FILE, {
		while (READ) {
			ASSERT(SEQUENCE[0] != '\0');
			ASSERT(QUALITIES[0] != '\0');
		}
	});
})

TEST(fastq_test2(), {
	minsketch *m = minsketch_new(100, 100);
	ASSERT(m != NULL);
	USING(FASTQ_FILE, {
		while (READ) {
			minsketch_add(m, SEQUENCE);
			ASSERT(minsketch_get(m, SEQUENCE) > 0);
		}
	});
	minsketch_free(m);
})

TEST(fastq_test3(), {
	bloomfilter *b = bloomfilter_new(100, 100);
	ASSERT(b != NULL);
	USING(FASTQ_FILE, {
		while (READ) {
			bloomfilter_add(b, SEQUENCE);
			ASSERT(bloomfilter_get(b, SEQUENCE) > 0);
		}
	});
	bloomfilter_free(b);
})

TEST(fastq_test4(), {
	bool entered = false;
	FOR_EACH(kmer, {
		entered = true;
	});
	ASSERT(entered == true);
})

TEST(fastq_test(), {
	ASSERT(fastq_test1() == true);
	ASSERT(fastq_test2() == true);
	ASSERT(fastq_test3() == true);
	ASSERT(fastq_test4() == true);
})
