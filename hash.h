#ifndef _HASH_H
#define _HASH_H

#define BITS_PER_CHAR (2) // Number of bits used per nucleotide
#define MAX_KMER_SIZE (((sizeof(unsigned int) * 8)/BITS_PER_CHAR) - 1) // Recommended max kmer size (15)
#define A 0x0  // 0b00
#define C 0x1  // 0b01
#define G 0x2  // 0b10
#define T 0x3  // 0b11

// Hashes a kmer; h(x) = ((djb2(x) + binary(x)) ^ seed)
unsigned int hash(const char *str, unsigned int seed);

#endif