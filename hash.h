#ifndef _HASH_H
#define _HASH_H

#define A 0x0  // 0b00
#define C 0x1  // 0b01
#define G 0x2  // 0b10
#define T 0x3  // 0b11

#define BITS_PER_CHAR (2)
#define MAX_HASH_SIZE ((sizeof(unsigned long long) * 8)/BITS_PER_CHAR) - 1
#define REC_HASH_SIZE ((sizeof(unsigned int) * 8)/BITS_PER_CHAR) - 1

unsigned long long hash(const char *str, int seed);

#endif