
/*
 * Laura Fortune D21126682
 * Keccak, SHA-3 256 Bit Variant Implementation
 */

// Header file - declarations for keccak.c
#ifndef KECCAK_H
#define KECCAK_H

// SHA-3-256 Setup
#define RATE 1088
#define CAPACITY 512
#define OUTPUT_LENGTH (256 / 8) // 256-bits / 8-bits = 32 bytes
#define BLOCKSIZE 136           // rate / 8-bits = 136-bytes
#define KECCAK_ROUNDS 24        // 24 rounds for keccak-f[1600]


/**
 * @brief Main entry point to the keccak/SHA-3-256 C library
 * 
 * Keccak was selected by the NIST as the hashing algorithm for SHA-3 
 * This implememtation covers the SHA-3 256 bit variant
 * 
 * - Takes user input (param data) of length (param length)
 * - Returns the Keccak 32-byte(256-bit) hash digest of the input data
 * 
 * @param data - pointer to an array of bytes (the input to be hashed)
 * @param length - length of the input (data)
 * @return unsigned char* - pointer to the array containing the keccak hash digest
 */

unsigned char *keccak_hash(unsigned char *data, size_t length);


#endif
