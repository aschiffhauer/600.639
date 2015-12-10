#include <stdbool.h>
#include <stdio.h>

#include "bloomfilter.h"
#include "bloomfiltertests.h"
#include "tests.h"
#include "hash.h"

#define USING(m, k, expressions) bloomfilter *b = bloomfilter_new(m, k); ASSERT(b != NULL); {expressions}; bloomfilter_free(b);
#define ADD(str) bloomfilter_add(b, str)
#define GET(str) bloomfilter_get(b, str)

TEST(bloomfilter_test1(), {
	USING(256, 4, {
		ASSERT(GET("AGTAGTAGTA") == false);
		ASSERT(GET("TGATGATGAT") == false);
		ASSERT(GET("GTAGTAGTAG") == false);

		ASSERT(ADD("AGTAGTAGTA") == true);
		ASSERT(ADD("TGATGATGAT") == true);
		ASSERT(ADD("GTAGTAGTAG") == true);

		ASSERT(GET("AGTAGTAGTA") == true);
		ASSERT(GET("TGATGATGAT") == true);
		ASSERT(GET("GTAGTAGTAG") == true);
		
		ASSERT(GET("CATCATCATC") == false);
		ASSERT(GET("TAGTAGTAGT") == false);
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