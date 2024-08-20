
/*
 * Laura Fortune D21126682
 * Keccak, SHA-3 256 Bit Variant Implementation
 */


// main file where keccak algorithm will be implemented
#include "keccak.h"
#include "round_functions.h"

// test function
int add(int x, int y) {
  return x + y;
}

// SHA-3-256 Setup
#define RATE 1088
#define CAPACITY 512
#define OUTPUT_LENGTH (256 / 8) // 256-bits / 8-bits = 32 bytes
#define BLOCKSIZE 136           // rate / 8-bits = 136-bytes
#define KECCAK_ROUNDS 24        // 24 rounds for keccak-f[1600]

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
void keccak_f(uint64_t state[5][5]) {
  for (int round = 0; round < KECCAK_ROUNDS; round++) {
    theta(state);
    rho_and_pi(state);
    chi(state);
    iota(state, round);
  }
}


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