#include <stdbool.h>

#include "errortests.h"
#include "tests.h"

TEST(error_test1(), {

})

TEST(error_test(), {
	ASSERT(error_test1() == true);
})