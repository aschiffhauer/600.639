#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "fastq.h"
#include "histogram.h"
#include "histogramtests.h"
#include "bloomfilter.h"
#include "minsketch.h"
#include "tests.h"

// Global parameters for all histogram tests
#define FASTQ_FILE "test.fastq"
#define KMER_SIZE 10

// Macros that wrap common histogram uses for tests
#define USING(type, counter, expressions) histogram *h = histogram_new(type, counter); ASSERT(h != NULL); expressions; histogram_free(h);
#define READ(...) histogram_read(h, FASTQ_FILE, KMER_SIZE)
#define COUNT(str) histogram_count(h, str)
#define FOR_EACH_KMER(kmer, expressions) fastq_for_each_kmer(FASTQ_FILE, KMER_SIZE, kmer, expressions);

// Asserts fastq files can be read into a histogram with an underlying bloomfilter data structure
TEST(histogram_test1(), {
	USING(BLOOMFILTER, bloomfilter_new(128, 4), {
		ASSERT(READ() == true);
		FOR_EACH_KMER(kmer, {
			ASSERT(COUNT(kmer) > 0);
		});
	});
})

// Asserts fastq files can be read into a histogram with an underlying minsketch data structure
TEST(histogram_test2(), {
	USING(MINSKETCH, minsketch_new(128, 4), {
		ASSERT(READ() == true);
		FOR_EACH_KMER(kmer, {
			ASSERT(COUNT(kmer) > 0);
		});
	});
})

// Driver for histogram tests
TEST(histogram_test(), {
	ASSERT(histogram_test1() == true);
	ASSERT(histogram_test2() == true);
})