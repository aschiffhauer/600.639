#include <stdbool.h>
#include <stdio.h>

#include "minsketch.h"
#include "minsketchtests.h"
#include "tests.h"

#define USING(x,y,z) minsketch *m = minsketch_new(x,y); ASSERT(m != NULL); z; minsketch_free(m);

#define ADD(x) minsketch_add(m, x)
#define GET(x) minsketch_get(m, x)

TEST(minsketch_test1(), {
	USING(100, 100, {
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

TEST(minsketch_test(), {
	ASSERT(minsketch_test1() == true);
	ASSERT(minsketch_test2() == true);
})