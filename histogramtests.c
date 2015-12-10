#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "fastq.h"
#include "histogram.h"
#include "histogramtests.h"
#include "bloomfilter.h"
#include "minsketch.h"
#include "tests.h"

#define KMER_SIZE 6
#define FASTQ_FILE "reads_test.fastq"

#define USING(x,y,z) histogram *h = histogram_new(x, y); ASSERT(h != NULL); z; histogram_free(h);
#define READ(...) histogram_read(h, FASTQ_FILE, KMER_SIZE)
#define COUNT(x) histogram_count(h, x)
#define FOR_EACH(x, y) fastq_for_each_kmer(FASTQ_FILE, KMER_SIZE, x, y);

TEST(histogram_test1(), {
	USING(BLOOMFILTER, bloomfilter_new(100, 100), {
		ASSERT(READ() == true);
		FOR_EACH(kmer, {
			ASSERT(COUNT(kmer) > 0);
		});
	});
})

TEST(histogram_test2(), {
	USING(MINSKETCH, minsketch_new(100, 100), {
		ASSERT(READ() == true);
		FOR_EACH(kmer, {
			ASSERT(COUNT(kmer) > 0);
		});
	});
})

TEST(histogram_test(), {
	ASSERT(histogram_test1() == true);
	ASSERT(histogram_test2() == true);
})