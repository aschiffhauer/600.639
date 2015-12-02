#include <stdbool.h>
#include <stdlib.h>

#include "histogram.h"
#include "histogramtests.h"
#include "bloomfilter.h"
#include "minsketch.h"
#include "tests.h"

bool histogram_test1() {
	bloomfilter *b = bloomfilter_new(100, 100);
	ASSERT(b != NULL);
	histogram *h = histogram_new(BLOOMFILTER, b);
	ASSERT(h != NULL);
	ASSERT(histogram_add(h, "reads.fastq", 50) == true);
	histogram_free(h);
	PRINT("histogram_test1 passed");
	return true;
}

bool histogram_test2() {
	minsketch *m = minsketch_new(100, 100);
	ASSERT(m != NULL);
	histogram *h = histogram_new(MINSKETCH, m);
	ASSERT(h != NULL);
	ASSERT(histogram_add(h, "reads.fastq", 50) == true);
	histogram_free(h);
	PRINT("histogram_test2 passed");
	return true;
}

bool histogram_test() {
	ASSERT(histogram_test1() == true);
	ASSERT(histogram_test2() == true);
	PRINT("histogram tests passed");
	return true;
}