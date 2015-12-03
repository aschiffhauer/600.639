#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "errortests.h"
#include "fastq.h"
#include "histogram.h"
#include "histogramtests.h"
#include "bloomfilter.h"
#include "minsketch.h"
#include "tests.h"

#define KMER_SIZE 10
#define FASTQ_FILE "reads.fastq"

#define DEBUG_STATS false
#define DEBUG_KMERS false
#define DEBUG_LIERS false
#define DEBUG_FAKES false

#define USING(x,y,z) histogram *h = histogram_new(x, y); ASSERT(h != NULL); z; histogram_free(h);
#define READ(...) histogram_read(h, FASTQ_FILE, KMER_SIZE)
#define COUNT(x) histogram_count(h, x)
#define FOR_EACH(x, y) fastq_for_each_kmer(FASTQ_FILE, KMER_SIZE, x, y);

TEST(error_test1(), {
	USING(MINSKETCH, minsketch_new(100, 100), {
		READ();
		int n = 0;
		float mean = 0.0;
		FOR_EACH(kmer, {
			n++;
			mean += COUNT(kmer);
		});
		mean /= (float)n;
		float variance = 0.0f;
		FOR_EACH(kmer, {
			variance += powf(COUNT(kmer) - mean, 2);
		});
		variance /= n - 1;
		float stddev = sqrtf(variance);
		int outliers = 0;
		FOR_EACH(kmer, {
			int count = COUNT(kmer);
			#if DEBUG_KMERS
				PRINT("%s: %d", kmer, count);
			#endif
			if (count <= ceil(mean - 2 * stddev)) {
				#if DEBUG_LIERS && DEBUG_KMERS == false
					PRINT("%s: %d", kmer, count);
				#endif
				outliers++;
			}
		});
		#if DEBUG_STATS
			PRINT("len: %d", n);
			PRINT("mean: %f", mean);
			PRINT("stddev: %f", stddev);
			PRINT("outliers: %d (%f%%)", outliers, 100*outliers/(float)(n));
		#endif
		#if DEBUG_FAKES
			char temp[KMER_SIZE * 2 + 1];
			memset(temp, 0, KMER_SIZE * 2 + 1);
			PRINT("fake reads:")
			for (int i = 0; i < KMER_SIZE * 2; i++) {
				temp[i] = 'A' + i;
				PRINT("  %s: %d", temp, COUNT(temp));
			}
		#endif
	});
})

TEST(error_test(), {
	ASSERT(error_test1() == true);
})