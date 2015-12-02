#include <stdbool.h>
#include <stdio.h>

#include "bloomfilter.h"
#include "bloomfiltertests.h"
#include "tests.h"

#define USING(x,y,z) bloomfilter *b = bloomfilter_new(x,y); ASSERT(b != NULL); z; bloomfilter_free(b);

#define ADD(x) bloomfilter_add(b, x)
#define GET(x) bloomfilter_get(b, x)

TEST(bloomfilter_test1(), {
	USING(100, 100, {
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
	});	
})

TEST(bloomfilter_test2(), {
	USING(2, 2, {
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
	});
})

TEST(bloomfilter_test(), {
	ASSERT(bloomfilter_test1() == true);
	ASSERT(bloomfilter_test2() == true);
})