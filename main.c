#include <stdio.h>

#include "minsketchtests.h"
#include "bloomfiltertests.h"

int main (void) {
	bloomfilter_test();
	minsketch_test();
}