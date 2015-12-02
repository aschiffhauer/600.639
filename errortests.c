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

#define KMER_SIZE 10
#define FASTQ_FILE "reads.fastq"

#define USING(x,y,z) histogram *h = histogram_new(x, y); ASSERT(h != NULL); z; histogram_free(h);
#define READ(...) histogram_read(h, FASTQ_FILE, KMER_SIZE)
#define COUNT(x) histogram_count(h, x)
#define FOR_EACH(x, y) fastq_for_each_kmer(FASTQ_FILE, KMER_SIZE, x, y);

TEST(error_test1(), {
	USING(MINSKETCH, minsketch_new(100, 100), {
		READ();
		int min = ~0U >> 1;
		char min_kmer[KMER_SIZE + 1];
		FOR_EACH(kmer, {
			int count = COUNT(kmer);
			if (count < min) {
				PRINT("%s", kmer);
				min = count;
				strcpy(min_kmer, kmer);
			}
		});
		PRINT("BAAGTTGAGT: %d", COUNT("BAAGTTGAGT"));
		PRINT("%s: %d", min_kmer, min);
	});
})

TEST(error_test(), {
	ASSERT(error_test1() == true);
})