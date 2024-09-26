// Entry point for running the program

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keccak.h"

// helper print function
void print_32byte_digest(unsigned char *digest, size_t length) {

  for (size_t i = 0; i < length; i++) {
    printf("%02x", digest[i]); // prints each byte as 2 digit hex
  
    // adds space every 8 bytes for readability
    if ((i + 1) % 8 == 0) printf(" ");   

 }
  printf("\n"); 
}


int main() {

    if (argc < 2 ) {
    printf("Error: Please provide an input string to be hashed \n");
    printf("Usage: %s <input string> \n", argv[0]);
    return 1;
  }

  if (argc > 2) {
    printf("Error: Too many arguments - Please provide only one input string \n");
    printf("Usage: %s <input string> \n", argv[0]);
    return 1;
  }
  
  // test input
  unsigned char *input = (unsigned char *)argv[1];
  size_t input_length = strlen(argv[1]);

  // call keccak hash fuction on test data
  unsigned char *hash_digest = keccak_hash(input, input_length);

   printf("############ INPUT DATA ###########\n");
   printf("%s\n", input);

   printf("############ HASH DIGEST ###########\n");
   print_32byte_digest(hash_digest, 32); // sha-3-256 will always return 32 byte hash

   free(hash_digest);

   return 0;

}
