
/*
 * Laura Fortune D21126682
 * Keccak, SHA-3 256 Bit Variant Implementation
 */
#include "round_functions.h"
#include "utils.h"


// Contains the permutation functions (theta, rho, pi, chi, iota)

/**
 *  θ step
 *  C[x] = A[x,0] xor A[x,1] xor A[x,2] xor A[x,3] xor A[x,4],   for x in 0…4
 *  D[x] = C[x-1] xor rot(C[x+1],1),     for x in 0…4
 *  A[x,y] = A[x,y] xor D[x]
 */
void theta(uint64_t state[5][5]) {
 
  uint64_t C[5] = {0, 0, 0, 0, 0};       // Initialise column parity 'C' array to all zeroes
  uint64_t D[5] = {0, 0, 0, 0, 0};       // initialise D array

  
  // Calculate column parity 'C'
  for (int x = 0; x < 5; x++) { 
    for (int y = 0; y < 5; y++) { 
      C[x] ^= state[x][y];               // XOR each lane value of current column
    }
  }
                                         
  // Calculate D - Disfused column parity
  for (int x = 0; x < 5; x++) {
    // D[x] = parity of col to left, XORed with parity of col to right(rotated left 1-bit)
    D[x] = C[(x + 4) % 5] ^ ROTL64(C[(x + 1) % 5], 1); // modulo 5 ensures wrapping 
  }

  // Apply diffused column parity 'D' to state array
  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 5; y++) {
      state[x][y] ^= D[x];                // XOR each element in current column with corresponding D value 
    }
  }
}


/**
 * ρ and π steps
 * B[y,2*x+3*y] = rot(A[x,y], r[x,y]),     for (x,y) in (0…4,0…4)
 */
void rho_and_pi(uint64_t state[5][5]) {
  // Initialisation
  int x = 1, y = 0; // Initial coordinates
  uint_fast64_t lane = state[x][y];        // value stored at lane(x, y)

  for (int t = 0; t < 24; t++) {
    // calculate rotation offset
    int rotationOffset = ((t + 1) * (t + 2) / 2) % 64; 

    // Update coordinates
    int newX = y;                          // x is now equal to y
    int newY = (2 * x + 3 * y) % 5;        // y is now equal to new y position
    uint64_t tempLane = state[newX][newY]; // Temporarily store the value at new coordinates

    // Rotate the current lane and update state at new coordinates
    uint64_t rotatedBits = ROTL64(lane, rotationOffset);
    state[newX][newY] = rotatedBits;       // Update state at position newX, newY

    // Update lane and x,y coordinates for next iteration
    lane = tempLane;
    x = newX;
    y = newY;

  }
}

/**
 *  χ step
 *  A[x,y] = B[x,y] xor ((not B[x+1,y]) and B[x+2,y]),     for (x,y) in (0…4,0…4)
 */
void chi(uint64_t state[5][5]) {
  
  uint64_t C[5]; // Initialise state array to store planes (rows)

  // Processes each of the 5 planes/rows in state array
  for (int y = 0; y < 5; y++) {

    // first inner loop
    for (int x = 0; x < 5; x++) {
      C[x] = state[x][y]; // store value of current plane/row from state array - updates to State should not effect computation of current row
    }

    // second inner loop - updates each of the 5 elements of each row
    for (int x = 0; x < 5; x++) {
      state[x][y] = (C[x] ^ ((~C[(x+1) % 5]) & C[(x+2) % 5]));
    }

  }

}

/*
 * ι step
 *  A[0,0] = A[0,0] xor RC
 */
void iota(uint64_t state[5][5], int round) {
  
  state[0][0] ^= RC[round]; // XOR the current value at state[0][0] with the round constant
}