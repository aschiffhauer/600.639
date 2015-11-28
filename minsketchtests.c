#include <stdio.h>

#include "minsketch.h"
#include "minsketchtests.h"
#include "tests.h"

#define NEW(x,y) minsketch *m = minsketch_new(x,y); ASSERT(m != NULL);
#define ADD(x) minsketch_add(m, x)
#define GET(x) minsketch_get(m, x)

static bool minsketch_test1() {
	NEW(100, 100);

	ASSERT(GET("A") == 0);
	ASSERT(GET("AB") == 0);
	ASSERT(GET("ABC") == 0);

	ASSERT(ADD("A") == true);
	ASSERT(ADD("AB") == true);
	ASSERT(ADD("ABC") == true);

	ASSERT(GET("A") >= 1);
	ASSERT(GET("AB") >= 1);
	ASSERT(GET("ABC") >= 1);
	
	ASSERT(GET("ACB") == 0);
	ASSERT(GET("ABCD") == 0);


	return true;
}

static bool minsketch_test2() {
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
	ASSERT((count += GET("A")) >= 1);
	ASSERT((count += GET("AB")) >= 1);
	ASSERT((count += GET("ABC")) >= 1);
	ASSERT((count += GET("ABCD")) >= 1);
	ASSERT((count += GET("ABCDE")) >= 1);
	ASSERT((count += GET("ABCDEF")) >= 1);
	ASSERT(count > 6);

	return true;
}

bool minsketch_test() {
	ASSERT(minsketch_test1() == true);
	ASSERT(minsketch_test2() == true);
	PRINT("minsketch tests passed");
	return true;
}