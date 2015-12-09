#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdbool.h>

#include "histogram.h"

bool error_detect(histogram *h, char *sequence, int k, int cutoff);
char *error_correct(histogram *h, char *sequence, int k, int cutoff);

#endif