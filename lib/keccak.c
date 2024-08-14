
/*
 * Laura Fortune D21126682
 * Keccak, SHA-3 256 Bit Variant Implementation
 */


// main file where keccak algorithm will be implemented
#include "keccak.h"


// test function
int add(int x, int y) {
  return x + y;
}



/*
 * Keccak-f[b] - Permutation Functions
 * - b=1600, w=64, n=24, z=output, A=state array, d=256/8 = 32 bytes (output length) 
 * - M=input: string of bytes -Mbytes, followed by Mbits(0-7)
 * - Applies round functions (theta, rho, pi, chi, iota) to state array
 * - State array is transformed 24 times
 * 
 * Pseudo-code example,
 * - Keccak-f[b](A) {
 *     for i in 0…n-1
 *        A = Round[b](A, RC[i])
 *      return A
 *   }
 */



/*
 * Keccak  Sponge Functions
 */

// Padding

// Initialisation 

// Absorbing Phase

// Squeezing Phase


/**
 * Implementation of main hashing function declared in header file
 */
unsigned char *keccak_hash(unsigned char *data, size_t length) {

  //TODO:
  // Implement Padding
  // Implement Initialisation
  // Implement Absorbing phase
  // Implement Squeezing phase

  // return hashed input
  unsigned char *digest = (unsigned char *)malloc(32);

  // place holder value 
  for (int i = 0; i < 32; i++) {
    digest[i] = 0x00; 
  }
  return digest;

}