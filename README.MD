## Keccak SHA-3-256 Implementation

---

Implementation of Keccak SHA-3 hashing algorithm (version 256) as a C Library. This version was created purely for learning purposes and is not optimised or intended for production use. 

The Keccak algorithm was designed by Guido Bertoni, Joan Daemen(co-author of [AES](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard)), Michaël Peeters and Gilles Van Assche and is based on a broader framework known as ‘sponge construction’, formally defined in their 2007 paper ‘[Sponge Functions](https://keccak.team/files/SpongeFunctions.pdf)’. On Oct 2, 2012, Keccak was selected as the successor to SHA-2 by the NIST. This implementation follows the original [Keccak SHA-3 submission](https://keccak.team/files/Keccak-submission-3.pdf) prior to its [standardisation by the NIST](https://nvlpubs.nist.gov/nistpubs/fips/nist.fips.202.pdf). 

### Documentation

For a more detailed explanation of Keccak, SHA-3-256 and Sponge Construction, please refer to the [keccak-sha-3-256.pdf](https://github.com/lauraFortune/sha-3-256/tree/main/documentation/keccak-sha-3-256.pdf) in the documentation directory

### Features

*   **Complete SHA-3-256 Implementation**
*   **Adheres to the Original Keccak Padding Rule:**
    *   If only one byte of padding is required, byte value 0x86 is applied
    *   Else padding begins with 0x06, is filled with zeros 0x00, and ends with 0x80 - signifying the end of the sequence
*   **Keccak\_hash:** Core function Keccak\_hash takes a user input ‘data’ and returns a 256-bit(32-byte) hash digest
*   **Submodule Validation:** Tested against submodule [ctz/keccak](https://github.com/ctz/keccak) (Python implementation) for validation. Unit tests are written in Python using the Ctypes library to interface with the C implementation, comparing its output against the referenced submodule.
*   **Github Actions:** Tests run automatically on each commit
*   **Learning Focus:** Written for educational purposes with a focus on clarity over security

### Keccak Hash Function

The core function Keccak\_hash is the main entry point to the keccak/sha-3-256 C library:

```c

/**
 * @brief Main entry point to the keccak/SHA-3-256 C library
 
 * - Takes user input (param data) of length (param length)
 * - Returns the Keccak 32-byte(256-bit) hash digest of the input data
 * 
 * @param data - pointer to an array of bytes (the input to be hashed)
 * @param length - length of the input (data)
 * @return unsigned char* - pointer to the array containing the keccak hash digest
 */

unsigned char *keccak_hash(unsigned char *data, size_t length);
```

## Running the C Library

1.  Clone project:
    
    ```plaintext
    git clone --recurse-submodules https://github.com/lauraFortune/sha-3-256
    ```
    
2.  Navigate to the project directory's root and run the make file to build the program:
    
    ```plaintext
    cd sha-3-256
    make
    ```
    
3.  The C Library takes a single string input as a command line argument. Run the newly created main executable along with your input string to hash:
    
    ```c
    ./main <input string>
    
    - example:
    	./main "Hello, World!"
    ```
    

## Testing the Implementation

 To execute the Python unit tests, run:

```plaintext
make tests
```

*   These tests validate the C implementation against the Python submodule.

### Cleaning Up

To remove compiled files, run: 

```plaintext
make clean
```

## Acknowledgments

*   [Keccak Team Official Website](https://keccak.team/)
*   [NIST SHA-3 Specification](https://nvlpubs.nist.gov/nistpubs/fips/nist.fips.202.pdf)
*   [ctz/keccak](https://github.com/ctz/keccak) as a submodule for validation
   
