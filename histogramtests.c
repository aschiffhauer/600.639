#include <stdbool.h>
#include <stdlib.h>

#include "histogram.h"
#include "histogramtests.h"
#include "bloomfilter.h"
#include "minsketch.h"
#include "tests.h"

#define USING(x,y,z) histogram *h = histogram_new(x, y); ASSERT(h != NULL); z; histogram_free(h);
#define ADD(x, y) histogram_add(h, x, y)

TEST(histogram_test1(), {
	USING(BLOOMFILTER, (bloomfilter_new(100, 100)), {
		ASSERT(ADD("reads.fastq", 50) == true);
	});
})

TEST(histogram_test2(), {
	USING(MINSKETCH, (minsketch_new(100, 100)), {
		ASSERT(ADD("reads.fastq", 50) == true);
	});
})

TEST(histogram_test(), {
	ASSERT(histogram_test1() == true);
	ASSERT(histogram_test2() == true);
})