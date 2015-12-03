#include <stdio.h>

#include "minsketchtests.h"
#include "bloomfiltertests.h"
#include "fastqtests.h"
#include "histogramtests.h"
#include "errortests.h"

#define GENERATE_NEW_FASTQ_FILE true

int main (void) {
	fastq_test(GENERATE_NEW_FASTQ_FILE);
	bloomfilter_test();
	minsketch_test();
	histogram_test();
	error_test();
}