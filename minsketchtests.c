#include <stdbool.h>
#include <stdio.h>

#include "minsketch.h"
#include "minsketchtests.h"
#include "tests.h"

// Macros that wrap common minsketch uses for tests
#define USING(w, d, expressions) minsketch *m = minsketch_new(w, d); ASSERT(m != NULL); expressions; minsketch_free(m);
#define ADD(str) minsketch_add(m, str)
#define GET(str) minsketch_get(m, str)

// A sufficiently large minsketch shouldn't give false positives
TEST(minsketch_test1(), {
	USING(256, 4, {
		ASSERT(GET("A") == 0);
		ASSERT(GET("AA") == 0);
		ASSERT(GET("AAA") == 0);

		ASSERT(ADD("A") == true);
		ASSERT(ADD("AA") == true);
		ASSERT(ADD("AAA") == true);

		ASSERT(GET("A") >= 1);
		ASSERT(GET("AA") >= 1);
		ASSERT(GET("AAA") >= 1);
		
		ASSERT(GET("AAAA") == 0);
		ASSERT(GET("AAAAA") == 0);
	});
});

// A sufficiently small minsketch should give false positives
TEST(minsketch_test2(), {
	USING(2, 2, {
		ASSERT(GET("A") == 0);
		ASSERT(GET("AA") == 0);
		ASSERT(GET("AAA") == 0);
		ASSERT(GET("AAAA") == 0);
		ASSERT(GET("AAAAA") == 0);
		ASSERT(GET("AAAAAA") == 0);

		ASSERT(ADD("A") == true);
		ASSERT(ADD("AA") == true);
		ASSERT(ADD("AAA") == true);
		ASSERT(ADD("AAAA") == true);
		ASSERT(ADD("AAAAA") == true);
		ASSERT(ADD("AAAAAA") == true);

		int count = 0;
		ASSERT((count += GET("A")) >= 1);
		ASSERT((count += GET("AA")) >= 1);
		ASSERT((count += GET("AAA")) >= 1);
		ASSERT((count += GET("AAAA")) >= 1);
		ASSERT((count += GET("AAAAA")) >= 1);
		ASSERT((count += GET("AAAAAA")) >= 1);
		ASSERT(count > 6);
	});
});

// Driver for minsketch tests
TEST(minsketch_test(), {
	ASSERT(minsketch_test1() == true);
	ASSERT(minsketch_test2() == true);
})