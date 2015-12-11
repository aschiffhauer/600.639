#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdbool.h>

#include "histogram.h"

// Given a pre-populated histogram, a sequence, the kmer length for the sequence, and a 'cutoff' frequency: 
// returns and modifies the sequence if an error is detected;
// otherwise, returns NULL and does not modify the sequence.
char *error_correct(histogram *h, char *sequence, int k, int cutoff);

#endif