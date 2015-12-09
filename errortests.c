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

#define KMER_SIZE 10
#define FASTQ_FILE "reads.fastq"
#define FAKE_KMER "ATATATATAT"
#define WIDTH 100
#define HEIGHT 1

// error_test1 flags
#define DEBUG_STATS true
#define DEBUG_KMERS false
#define DEBUG_LIERS false
#define DEBUG_FAKES false
#define DEBUG_FAKER false

// error_test2 flags
#define DEBUG_NSEQS true
#define DEBUG_SEQS  true

#define USING(x,y,z) histogram *h = histogram_new(x, y); ASSERT(h != NULL); z; histogram_free(h);
#define READ(...) histogram_read(h, FASTQ_FILE, KMER_SIZE)
#define COUNT(x) histogram_count(h, x)
#define FOR_EACH_KMER(x, y) fastq_for_each_kmer(FASTQ_FILE, KMER_SIZE, x, y);
#define LOAD_FACTOR(...) histogram_load_factor(h)
#define ERROR_CORRECT(sequence, cutoff) error_correct(h, sequence, KMER_SIZE, cutoff)

TEST(error_test1(), {
	USING(MINSKETCH, minsketch_new(WIDTH, HEIGHT), {
		READ();
		int n = 0;
		float mean = 0.0;
		FOR_EACH_KMER(kmer, {
			n++;
			mean += COUNT(kmer);
		});
		mean /= (float)n;
		float variance = 0.0f;
		FOR_EACH_KMER(kmer, {
			variance += powf(COUNT(kmer) - mean, 2);
		});
		variance /= n - 1;
		float stddev = sqrtf(variance);
		int outliers = 0;
		FOR_EACH_KMER(kmer, {
			int count = COUNT(kmer);
			#if DEBUG_KMERS
				PRINT("%s", kmer);
			#endif
			if (count <= ceil(mean - 2 * stddev)) {
				#if DEBUG_LIERS && DEBUG_KMERS == false
					PRINT("%s: %u", kmer, count);
				#endif
				outliers++;
			}
		});
		#if DEBUG_STATS
			PRINT("len: %d", n);
			PRINT("mean: %f", mean);
			PRINT("stddev: %f", stddev);
			PRINT("outliers: %d (%f%%)", outliers, 100*outliers/(float)(n));
			PRINT("load factor: %f%%", LOAD_FACTOR());
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
		#if DEBUG_FAKER
			PRINT("%s: %d", FAKE_KMER, COUNT(FAKE_KMER));
		#endif
	});
})

TEST(error_test2(), {
	USING(MINSKETCH, minsketch_new(WIDTH, HEIGHT), {
		char previous[MAX_READ_LENGTH + 1];
		READ();
		fastq *f = fastq_new(FASTQ_FILE); 
		int n = 0;
		while(fastq_read_line(f)) {
			strcpy(previous, f->sequence);
			if (ERROR_CORRECT(f->sequence, 10)) {
				n++;
				#if DEBUG_SEQS
					PRINT("corrected: %s -> %s", previous, f->sequence);
				#endif
			}
		}
		#if DEBUG_NSEQS
			PRINT("errant sequences: %d", n);
		#endif
	});
});

TEST(error_test(), {
	ASSERT(error_test1() == true);
	ASSERT(error_test2() == true);
})