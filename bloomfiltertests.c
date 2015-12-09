#include <stdbool.h>
#include <stdio.h>

#include "bloomfilter.h"
#include "bloomfiltertests.h"
#include "tests.h"
#include "hash.h"

#define USING(x,y,z) bloomfilter *b = bloomfilter_new(x,y); ASSERT(b != NULL); z; bloomfilter_free(b);

#define ADD(x) bloomfilter_add(b, x)
#define GET(x) bloomfilter_get(b, x)

TEST(bloomfilter_test1(), {
	USING(100, 100, {
		ASSERT(GET("AGTAGTAGTA") == false);
		ASSERT(GET("TGATGATGAT") == false);
		ASSERT(GET("GTAGTAGTAG") == false);

		ASSERT(ADD("AGTAGTAGTA") == true);
		ASSERT(ADD("TGATGATGAT") == true);
		ASSERT(ADD("GTAGTAGTAG") == true);

		ASSERT(GET("AGTAGTAGTA") == true);
		ASSERT(GET("TGATGATGAT") == true);
		ASSERT(GET("GTAGTAGTAG") == true);
		
		ASSERT(GET("CATCATCATC") == false, ERROR("    COUNT(CATCATCATC) = %d, h(CATCATCATC) = %llu", GET("CATCATCATC"), hash("CATCATCATC", 0)));
		ASSERT(GET("TAGTAGTAGT") == false, ERROR("    COUNT(TAGTAGTAGT) = %d, h(TAGTAGTAGT) = %llu", GET("TAGTAGTAGT"), hash("TAGTAGTAGT", 0)));
	});	
})

TEST(bloomfilter_test2(), {
	USING(2, 2, {
		ASSERT(GET("AGTAGTAGTA") == false);
		ASSERT(GET("TGATGATGAT") == false);
		ASSERT(GET("GTAGTAGTAG") == false);
		ASSERT(GET("CATCATCATC") == false);
		ASSERT(GET("TAGTAGTAGT") == false);

		ASSERT(ADD("AGTAGTAGTA") == true);
		ASSERT(ADD("TGATGATGAT") == true);
		ASSERT(ADD("GTAGTAGTAG") == true);
		ASSERT(ADD("CATCATCATC") == true);
		ASSERT(ADD("TAGTAGTAGT") == true);

		ASSERT(GET("AGTAGTAGTA") == true);
		ASSERT(GET("TGATGATGAT") == true);
		ASSERT(GET("GTAGTAGTAG") == true);
		ASSERT(GET("CATCATCATC") == true);
		ASSERT(GET("TAGTAGTAGT") == true);
	});
})

TEST(bloomfilter_test(), {
	ASSERT(bloomfilter_test1() == true);
	ASSERT(bloomfilter_test2() == true);
})