#include <stdio.h>

#include "minsketchtests.h"
#include "bloomfiltertests.h"
#include "fastqtests.h"
#include "histogramtests.h"
#include "errortests.h"

#define GENERATE_NEW_FASTQ_FILE false
#define RUN_TESTS false

static void test() {
	#if RUN_TESTS
	fastq_test(GENERATE_NEW_FASTQ_FILE);
	bloomfilter_test();
	minsketch_test();
	histogram_test();
	error_test();
	#endif
}

int main (void) {
	test();
}