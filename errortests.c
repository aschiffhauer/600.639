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

#define KMER_SIZE 50
#define FASTQ_FILE "reads.fastq"

#define USING(x,y,z) histogram *h = histogram_new(x, y); ASSERT(h != NULL); z; histogram_free(h);
#define READ(...) histogram_read(h, FASTQ_FILE, KMER_SIZE)
#define COUNT(x) histogram_count(h, x)
#define FOR_EACH(x, y) fastq_for_each_kmer(FASTQ_FILE, KMER_SIZE, x, y);

TEST(error_test1(), {

})

TEST(error_test(), {
	ASSERT(error_test1() == true);
})