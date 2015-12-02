#include <stdio.h>

#include "minsketchtests.h"
#include "bloomfiltertests.h"
#include "fastqtests.h"
#include "histogramtests.h"

int main (void) {
	bloomfilter_test();
	minsketch_test();
	fastq_test();
	histogram_test();
}