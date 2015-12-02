#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "fastq.h"
#include "histogram.h"
#include "histogramtests.h"
#include "bloomfilter.h"
#include "minsketch.h"
#include "tests.h"

#define USING(x,y,z) histogram *h = histogram_new(x, y); ASSERT(h != NULL); z; histogram_free(h);
#define ADD(...) histogram_add(h, "reads.fastq", 50)
#define GET(x) histogram_get(h, x)
#define FOR_EACH(x, y) fastq_for_each_kmer("reads.fastq", 50, x, y);

TEST(histogram_test1(), {
	USING(BLOOMFILTER, bloomfilter_new(100, 100), {
		ASSERT(ADD() == true);
		FOR_EACH(kmer, {
			ASSERT(GET(kmer) > 0);
		});
	});
})

TEST(histogram_test2(), {
	USING(MINSKETCH, minsketch_new(100, 100), {
		ASSERT(ADD() == true);
		FOR_EACH(kmer, {
			ASSERT(GET(kmer) > 0);
		});
	});
})

TEST(histogram_test(), {
	ASSERT(histogram_test1() == true);
	ASSERT(histogram_test2() == true);
})