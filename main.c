#include <stdio.h>

#include "minsketch.h"
#include "bloomfilter.h"

#define M_NEW(x,y) minsketch *m = minsketch_new(x,y); if (!m) return -1;
#define M_ADD(x) printf("m.add(\"%s\")\n", x); minsketch_add(m, x);
#define M_GET(x) printf("m.get(\"%s\") = %d\n", x, minsketch_get(m, x));

#define B_NEW(m,k) bloomfilter *b = bloomfilter_new(m,k); if (!b) return -1;
#define B_ADD(x) printf("b.add(\"%s\")\n", x); bloomfilter_add(b, x);
#define B_GET(x) printf("b.get(\"%s\") = %d\n", x, bloomfilter_get(b, x));

int main (void) {
	M_NEW(100, 100);

	M_GET("foo");
	M_GET("fo");
	M_GET("f");

	M_ADD("foo");
	M_ADD("fo");
	M_ADD("f");

	M_GET("foo");
	M_GET("fo");
	M_GET("f");
}