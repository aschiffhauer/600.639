#ifndef _HASH_H
#define _HASH_H

#define A 0x4  // 0b100
#define C 0x5  // 0b101
#define G 0x6  // 0b110
#define T 0x7  // 0b111

#define BITS_PER_CHAR (3)
#define MAX_HASH_SIZE ((sizeof(unsigned long long) * 8)/BITS_PER_CHAR)
#define REC_HASH_SIZE ((sizeof(unsigned int) * 8)/BITS_PER_CHAR)

unsigned long long hash(const char *str, int seed);

#endif