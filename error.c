#include <stdbool.h>
#include <stdlib.h>

#include "error.h"

bool error_detect(histogram *h, char *sequence) {
	return h && sequence;
}

char *error_correct(histogram *h, char *sequence) {
	return (h && sequence) ? NULL : NULL;
}