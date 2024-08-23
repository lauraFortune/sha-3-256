
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
void absorbing(uint64_t state[5][5], unsigned char *buffer, size_t buffer_length) {
  
  size_t num_blocks = buffer_length / BLOCKSIZE;  //  Total block count - including padded block

  // Iterate through each block of the padded message (buffer)
  for(size_t i = 0; i < num_blocks; i++) {
    // Process each 8-byte segment of a block across 17 lanes (blocksize / 8-bytes = 17 lanes)
    // One lane = 64-bits or 8-bytes of the state array
    for(size_t j = 0; j < BLOCKSIZE; j += 8) { 
      uint64_t lane_value = 0;  // lane 8-bytes/64-bits

      // Iterate through all 8-bytes of the current 8-byte message segment
      for (size_t byte = 0; byte < 8; byte++) {
        uint64_t byteValue = (uint64_t)buffer[i * BLOCKSIZE + j + byte]; // Extract current byte value from buffer (padded message)
        lane_value |= byteValue << (byte * 8);                           // Bitwise left shift byte value into lane value - 0n, 8n, 16n, 24n, 32n, 40n, 48n, 56n
      }

      size_t x = (j / 8) % 5;      // Caluclates current X coordinate of state array
      size_t y = (j / 8) / 5;      // Calculates current Y coordinate of state array
      state[x][y] ^= lane_value;   // XOR 8-bytes from the buffer(padded message) into the current lane(x,y) of the state array
    }
    // Call the permutation functions on state after absorbing each block
    keccak_f(state);
  }

}


// Squeezing Phase


/**
 * Implementation of main hashing function declared in header file
 * data: message, length: data length, buffer = data + padding, buffer_length: length of padded message
 */
unsigned char *keccak_hash(unsigned char *data, size_t length) {
  
  unsigned char *digest = (unsigned char *)malloc(32);  // Allocate memory for output - hashed input
  if (!digest) return NULL;

  // Step 1: Padding
  size_t padding_length = BLOCKSIZE - (length % BLOCKSIZE); 
  size_t buffer_length = length + padding_length;                   // Calculate padded message length
  unsigned char *buffer = (unsigned char *)malloc(buffer_length);   // Allocate memory for the padded message buffer
  if (!buffer) {
    free(digest);
    return NULL;
  }

  memcpy(buffer, data, length); // Copies input into start of buffer [data][unitialised padding space]
  padding(buffer, length, padding_length); // Apply the padding to buffer [data][padding bytes]

  // Step 2: Initialisation
  uint64_t state[5][5];
  initialise_state(state);

  // Step 3: Absorbing phase
  absorbing(state, buffer, buffer_length);

  //TODO:
  // Implement Squeezing phase


  

  // place holder value 
  for (int i = 0; i < 32; i++) {
    digest[i] = 0x00; 
  }

  return digest;

}


