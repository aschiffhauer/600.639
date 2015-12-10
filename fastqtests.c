#include <stdbool.h>
#include <stdio.h>

#include "fastq.h"
#include "fastqtests.h"
#include "minsketch.h"
#include "bloomfilter.h"
#include "tests.h"

// Global parameters for this test
#define KMER_SIZE 10
#define FASTQ_FILE "test.fastq"

// Macros that wrap common fastq uses for tests
#define USING(path, expressions) fastq *f = fastq_new(path); ASSERT(f != NULL); expressions; fastq_free(f);
#define FOR_EACH(kmer, expressions) fastq_for_each_kmer(FASTQ_FILE, KMER_SIZE, kmer, expressions);
#define READ(...) fastq_read(f)
#define SEQUENCE f->sequence
#define QUALITIES f->qualities

// Create a new random fastq file at FASTQ_FILE
// These are flags for the first test. 
#define GENERATE_NEW_FASTQ_FILE false
#define NREADS 100
TEST(fastq_test1(), {
	#if GENERATE_NEW_FASTQ_FILE
		fastq *f = fastq_generate(FASTQ_FILE, NREADS);
		ASSERT(f != NULL);
		fastq_free(f);
	#endif
})

// Reads in FASTQ_FILE and asserts that sequences and qualities are being read
TEST(fastq_test2(), {
	USING(FASTQ_FILE, {
		while (READ()) {
			ASSERT(SEQUENCE[0] != '\0');
			ASSERT(QUALITIES[0] != '\0');
		}
	});
})

// Reads in FASTQ_FILE and asserts that sequences and qualities are being read
// Also asserts that a minsketch is recording these reads
TEST(fastq_test3(), {
	minsketch *m = minsketch_new(128, 1);
	ASSERT(m != NULL);
	USING(FASTQ_FILE, {
		while (READ()) {
			minsketch_add(m, SEQUENCE);
			ASSERT(minsketch_get(m, SEQUENCE) > 0);
		}
	});
	minsketch_free(m);
})

// Reads in FASTQ_FILE and asserts that sequences and qualities are being read
// Also asserts that a bloomfilter is recording these reads
TEST(fastq_test4(), {
	bloomfilter *b = bloomfilter_new(128, 1);
	ASSERT(b != NULL);
	USING(FASTQ_FILE, {
		while (READ()) {
			bloomfilter_add(b, SEQUENCE);
			ASSERT(bloomfilter_get(b, SEQUENCE) > 0);
		}
	});
	bloomfilter_free(b);
})

// Tests that the fastq_for_each_kmer macro works
TEST(fastq_test5(), {
	bool entered = false;
	FOR_EACH(kmer, {
		entered = true;
	});
	ASSERT(entered == true);
})

// Driver for fastq tests
TEST(fastq_test(), {
	ASSERT(fastq_test1() == true);
	ASSERT(fastq_test2() == true);
	ASSERT(fastq_test3() == true);
	ASSERT(fastq_test4() == true);
	ASSERT(fastq_test5() == true);
})
