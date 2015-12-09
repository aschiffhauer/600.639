#ifndef _TESTS_H_
#define _TESTS_H_

#include <stdio.h>

#define PRINT(...) { fprintf(stdout, __VA_ARGS__); fprintf(stdout, "\n"); }
#define ERROR(...) { fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); }
#define DEBUG { ERROR("%s: %s: %d", __FILE__, __FUNCTION__, __LINE__); }
#define PANIC(...) { ERROR(__VA_ARGS__); DEBUG; for(;;) { ; } ; }
#define ASSERT(x, ...) { if (!(x)) {DEBUG; ERROR("    fail: %s", #x); __VA_ARGS__; return false; } }
#define TEST(x, y) bool x { y; PRINT("%s passed", #x); return true; }
#define lambda(t, f) ({ t _f_ f; _f_; })

#endif
