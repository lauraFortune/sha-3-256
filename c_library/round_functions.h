
/*
 * Laura Fortune D21126682
 * Keccak, SHA-3 256 Bit Variant Implementation
 */

// Header file - declarations for round_functions.c

#ifndef ROUND_FUNCTIONS_H
#define ROUND_FUNCTIONS_H

#include <stdint.h>

/**
 * Keccak-f[b] Round Functions 
 * - comprised of 5 steps
 * - Each function takes input state array and returns transformed state array
 */

// θ (Theta): transforms each column of state array 
void theta(uint64_t state[5][5]);

// ρ (Rho): transforms each lane of state array
// π (Pi): Permutation of lanes of state array
void rho_and_pi(uint64_t state[5][5]);

// χ (Chi): transforms rows of state array
void chi(uint64_t state[5][5]);

// ι Iota: adds round constant to Lane[0][0] of state array
void iota(uint64_t state[5][5], int roundConst);

#endif
