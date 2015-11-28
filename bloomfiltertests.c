#include <stdio.h>

#include "bloomfilter.h"
#include "bloomfiltertests.h"
#include "tests.h"

#define NEW(x,y) bloomfilter *b = bloomfilter_new(x,y); ASSERT(b != NULL);
#define ADD(x) bloomfilter_add(b, x)
#define GET(x) bloomfilter_get(b, x)

static bool bloomfilter_test1() {
	NEW(100, 100);

	ASSERT(GET("A") == false);
	ASSERT(GET("AB") == false);
	ASSERT(GET("ABC") == false);

	ASSERT(ADD("A") == true);
	ASSERT(ADD("AB") == true);
	ASSERT(ADD("ABC") == true);

	ASSERT(GET("A") == true);
	ASSERT(GET("AB") == true);
	ASSERT(GET("ABC") == true);
	
	ASSERT(GET("ACB") == false);
	ASSERT(GET("ABCD") == false);

	return true;
}

static bool bloomfilter_test2() {
	NEW(2, 2);

	ASSERT(GET("A") == false);
	ASSERT(GET("AB") == false);
	ASSERT(GET("ABC") == false);
	ASSERT(GET("ABCD") == false);
	ASSERT(GET("ABCDE") == false);
	ASSERT(GET("ABCDEF") == false);

	ASSERT(ADD("A") == true);
	ASSERT(ADD("AB") == true);
	ASSERT(ADD("ABC") == true);
	ASSERT(ADD("ABCD") == true);
	ASSERT(ADD("ABCDE") == true);
	ASSERT(ADD("ABCDEF") == true);

	ASSERT(GET("A") == true);
	ASSERT(GET("AB") == true);
	ASSERT(GET("ABC") == true);
	ASSERT(GET("ABCD") == true);
	ASSERT(GET("ABCDE") == true);
	ASSERT(GET("ABCDEF") == true);

	return true;
}

bool bloomfilter_test() {
	ASSERT(bloomfilter_test1() == true);
	ASSERT(bloomfilter_test2() == true);
	PRINT("bloomfilter tests passed");
	return true;
}