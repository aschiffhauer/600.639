#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "errortests.h"
#include "fastq.h"
#include "histogram.h"
#include "histogramtests.h"
#include "bloomfilter.h"
#include "minsketch.h"
#include "tests.h"

#define USING(x, y, z) histogram *h = histogram_new(x, y); ASSERT(h != NULL); z; histogram_free(h);
#define ADD(x, y) histogram_add(h, x, y)
#define GET(x) histogram_get(h, x)
#define FOR_EACH(x, y) fastq_for_each_kmer("reads.fastq", 50, x, y);

TEST(error_test1(), {
	USING(BLOOMFILTER, bloomfilter_new(100, 100), {
		ASSERT(ADD("reads.fastq", 50) == true);
	});
})

TEST(error_test(), {
	ASSERT(error_test1() == true);
})