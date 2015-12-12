#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include <stdbool.h>

bool algorithm_correct(const char *input_file_name, const char *output_file_name, int kmer_size, int minsketch_width, int minsketch_height, int frequency_cutoff);

#endif