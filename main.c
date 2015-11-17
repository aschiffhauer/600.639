#include <stdio.h>

#include "minsketch.h"

#define NEW(x,y) minsketch *m = minsketch_new(x,y); if (m == 0) return -1;
#define ADD(x) printf("m.add(\"%s\")\n", x); minsketch_add(m, #x);
#define GET(x) printf("m.get(\"%s\") = %d\n", x, minsketch_get(m, #x));

int main (void) {
	NEW(2,2);

	GET("foo");
	GET("fo");
	GET("f");

	ADD("foo");
	ADD("fo");
	ADD("f");

	GET("foo");
	GET("fo");
	GET("f");
}