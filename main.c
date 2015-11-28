#include <stdio.h>

#include "minsketchtests.h"
#include "bloomfiltertests.h"
#include "fastqtests.h"

int main (void) {
	bloomfilter_test();
	minsketch_test();
	fastq_test();
}