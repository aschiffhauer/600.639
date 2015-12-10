#include <stdio.h>

#include "minsketchtests.h"
#include "bloomfiltertests.h"
#include "fastqtests.h"
#include "histogramtests.h"
#include "errortests.h"
#include "experiment.h"

#define GENERATE_NEW_FASTQ_FILE false
#define RUN_TESTS true
#define RUN_EXPERIMENT true

static void test() {
	#if RUN_TESTS
		fastq_test(GENERATE_NEW_FASTQ_FILE);
		bloomfilter_test();
		minsketch_test();
		histogram_test();
		error_test();
	#endif
}

static void experiment() {
	#if RUN_EXPERIMENT
		experiment_run();
	#endif
}

int main (void) {
	test();
	experiment();
}