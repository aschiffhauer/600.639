#include <stdio.h>

#include "minsketchtests.h"
#include "bloomfiltertests.h"
#include "fastqtests.h"
#include "histogramtests.h"
#include "errortests.h"

// Runs all of the tests for data structures, file handling, error correction, etc.
int main (void) {
	fastq_test();
	bloomfilter_test();
	minsketch_test();
	histogram_test();
	error_test();
}