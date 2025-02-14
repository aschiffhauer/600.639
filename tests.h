#ifndef _TESTS_H_
#define _TESTS_H_

#include <stdio.h>

// This file is a test suite for frequently used macros 
#define PRINT(...) { fprintf(stdout, __VA_ARGS__); fprintf(stdout, "\n"); fflush(stdout); }
#define ERROR(...) { fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr); }
#define LOCAL(...) { ERROR("%s: %s: %d", __FILE__, __FUNCTION__, __LINE__); }
#define PANIC(...) { ERROR(__VA_ARGS__); LOCAL(); for(;;) { ; } ; }
#define ASSERT(x, ...) { if (!(x)) {LOCAL(); ERROR("    fail: %s", #x); __VA_ARGS__; return false; } }
#define TEST(x, y) bool x { y; PRINT("%s passed", #x); return true; }

#endif
