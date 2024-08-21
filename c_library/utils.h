
/*
 * Laura Fortune D21126682
 * Keccak, SHA-3 256 Bit Variant Implementation
 */

// Header file - declarations for utils.c

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h> // for uint64_t

// declare global consts
extern const uint64_t RC[24];
extern const int ROTATIONS[5][5];

// declare helper functions
uint64_t ROTL64(uint64_t val, uint64_t n);

#endif