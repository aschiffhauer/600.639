#include <stdio.h>

#include "bloomfilter.h"
#include "bloomfiltertests.h"
#include "tests.h"

#define NEW(x,y) bloomfilter *b = bloomfilter_new(x,y); ASSERT(b != NULL);
#define ADD(x) bloomfilter_add(b, x)
#define GET(x) bloomfilter_get(b, x)

static bool bloomfilter_test1() {
	NEW(100, 100);

	ASSERT(GET("A") == 0);
	ASSERT(GET("AB") == 0);
	ASSERT(GET("ABC") == 0);

	ASSERT(ADD("A") == true);
	ASSERT(ADD("AB") == true);
	ASSERT(ADD("ABC") == true);

	ASSERT(GET("A") == 1);
	ASSERT(GET("AB") == 1);
	ASSERT(GET("ABC") == 1);

	return true;
}

static bool bloomfilter_test2() {
	NEW(2, 2);

	ASSERT(GET("A") == 0);
	ASSERT(GET("AB") == 0);
	ASSERT(GET("ABC") == 0);
	ASSERT(GET("ABCD") == 0);
	ASSERT(GET("ABCDE") == 0);
	ASSERT(GET("ABCDEF") == 0);

	ASSERT(ADD("A") == true);
	ASSERT(ADD("AB") == true);
	ASSERT(ADD("ABC") == true);
	ASSERT(ADD("ABCD") == true);
	ASSERT(ADD("ABCDE") == true);
	ASSERT(ADD("ABCDEF") == true);

	int count = 0;
	ASSERT((count += GET("A")) == 1);
	ASSERT((count += GET("AB")) == 1);
	ASSERT((count += GET("ABC")) == 1);
	ASSERT((count += GET("ABCD")) == 1);
	ASSERT((count += GET("ABCDE")) == 1);
	ASSERT((count += GET("ABCDEF")) == 1);
	ASSERT(count == 6);

	return true;
}

bool bloomfilter_test() {
	ASSERT(bloomfilter_test1() == true);
	ASSERT(bloomfilter_test2() == true);
	PRINT("bloomfilter tests passed");
	return true;
}