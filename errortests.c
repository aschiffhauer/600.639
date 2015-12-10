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
#include "hash.h"
#include "error.h"

// Global parameters for all tests
#define FASTQ_FILE "test.fastq"
#define KMER_SIZE 10
#define MINSKETCH_WIDTH 50
#define MINSKETCH_HEIGHT 3

// Macros that wrap common histogram uses for tests
#define USING(type, counter, expressions) histogram *h = histogram_new(type, counter); ASSERT(h != NULL); expressions; histogram_free(h);
#define READ(...) histogram_read(h, FASTQ_FILE, KMER_SIZE)
#define COUNT(str) histogram_count(h, str)
#define FOR_EACH_KMER(kmer, expressions) fastq_for_each_kmer(FASTQ_FILE, KMER_SIZE, kmer, expressions);
#define LOAD_FACTOR(...) histogram_load_factor(h)
#define ERROR_CORRECT(sequence, cutoff) error_correct(h, sequence, KMER_SIZE, cutoff)

// Reads in a file comprising reads of all As with one errant T.
// Calculates the mean, variance, standard deviation, and outliers
// These are flags for the first test. 
// Setting these to true/false will result in more output for human verification
#define PRINT_STATS false
#define PRINT_OUTLIERS false
TEST(error_test1(), {
	USING(MINSKETCH, minsketch_new(MINSKETCH_WIDTH, MINSKETCH_HEIGHT), {
		// Populate the histogram
		READ();
		// Calculate the mean
		int n = 0;
		float mean = 0.0;
		FOR_EACH_KMER(kmer, {
			n++;
			mean += COUNT(kmer);
		});
		mean /= (float)n;
		// Calculate the variance and standard deviation
		float variance = 0.0f;
		FOR_EACH_KMER(kmer, {
			variance += powf(COUNT(kmer) - mean, 2);
		});
		variance /= n - 1;
		float stddev = sqrtf(variance);
		// Print out the outliers
		int outliers = 0;
		FOR_EACH_KMER(kmer, {
			int count = COUNT(kmer);
			if (count <= ceil(mean - 2 * stddev)) {
				#if PRINT_OUTLIERS
					PRINT("%s: %u", kmer, count);
				#endif
				outliers++;
			}
		});
		// Print stats about #kmers, mean, standard deviation, outliers, and load factor
		#if PRINT_STATS
			PRINT("len: %d", n);
			PRINT("mean: %f", mean);
			PRINT("stddev: %f", stddev);
			PRINT("outliers: %d (%f%%)", outliers, 100*outliers/(float)(n));
			PRINT("load factor: %f%%", LOAD_FACTOR());
		#endif
		// Do other fake sequences appear frequently? 
		// Ideally, these would all have 0 frequency hits.
		// Due to the probabilistic nature, however, we allow at most 1 hit.
		char temp[KMER_SIZE * 2 + 1];
		memset(temp, 0, KMER_SIZE * 2 + 1);
		for (int i = 0; i < KMER_SIZE * 2; i++) {
			temp[i] = 'A' + i;
			ASSERT(COUNT(temp) <= 1);
		}
	});
})

// Reads in a file comprising reads of all As with one errant T.
// The test will try to correct the sequence.
// These are flags for the second test. 
// Setting these to true/false will result in more output for human verification
#define CUTOFF 1
#define CORRECT_SEQUENCE "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
TEST(error_test2(), {
	USING(MINSKETCH, minsketch_new(MINSKETCH_WIDTH, MINSKETCH_HEIGHT), {
		// Populate the histogram
		READ();
		// Correct the *reads* of the file
		fastq *f = fastq_new(FASTQ_FILE); 
		ASSERT(f != NULL);
		while(fastq_read(f)) {
			if (ERROR_CORRECT(f->sequence, CUTOFF)) {
				ASSERT(strcmp(f->sequence, CORRECT_SEQUENCE) == 0);
			}
		}
		fastq_free(f);
	});
});

// Driver for error tests
TEST(error_test(), {
	ASSERT(error_test1() == true);
	ASSERT(error_test2() == true);
})