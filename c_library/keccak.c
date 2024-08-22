
/*
 * Laura Fortune D21126682
 * Keccak, SHA-3 256 Bit Variant Implementation
 */

// main file where keccak algorithm will be implemented
#include <stdlib.h>
#include <string.h>
#include "keccak.h"
#include "round_functions.h"

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
void padding(unsigned char *buffer, size_t length, size_t padding_length) {

  if (padding_length == 1) {
    buffer[length] = 0x81;  // Pad with 0x81 if only one byte of padding is needed
  } else {
    buffer[length] = 0x01; // Padding starts at 0x01
    memset(buffer + length + 1, 0x00, padding_length - 2); // zero filled padding
    buffer[length + padding_length - 1] = 0x80; // End padding with 0x80
  }
}


// Initialisation 
void initialise_state(uint64_t state[5][5]) {
  for(int x = 0; x < 5; x++) {
    for(int y = 0; y < 5; y++) {
      state[x][y] = 0;
    }
  }
}

// Absorbing Phase

// Squeezing Phase


/**
 * Implementation of main hashing function declared in header file
 */
unsigned char *keccak_hash(unsigned char *data, size_t length) {
  
  unsigned char *digest = (unsigned char *)malloc(32); // Allocate memory for output - hashed input
  if (!digest) return NULL;

  // #### Step 1. Padding
  // Calculate padded message length
  size_t padding_length = BLOCKSIZE - (length % BLOCKSIZE); 
  size_t padded_message_length = length + padding_length; 

  // Allocate memory for the padded message buffer
  unsigned char *buffer = (unsigned char *)malloc(padded_message_length); 
  if (!buffer) {
    free(digest);
    return NULL;
  }

  // Copies message input (data) into the start of the buffer
  memcpy(buffer, data, length); // Buffer now contains [data][unitialised padding space]

  // Apply the padding to buffer from the end of 'data'
  padding(buffer, length, padding_length); // Buffer now contains [data][padding bytes]

  // Step 2. Initialisation
  uint64_t state[5][5];
  initialise_state(state);

  //TODO:
  // Implement Absorbing phase
  // Implement Squeezing phase


  

  // place holder value 
  for (int i = 0; i < 32; i++) {
    digest[i] = 0x00; 
  }

  return digest;

}


