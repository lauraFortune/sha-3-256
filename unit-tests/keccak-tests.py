
import unittest           # python testing framework
import sys                # modifying import paths
from ctypes import *      # python library for working with C
from pathlib import Path  # object oriented filesystem paths
import secrets            # for generating cryptographically secure random numbers
import functools          # Python 2/3 compatibility - 'reduce'
import six                # Python 2/3 compatibility - xrange -> range

# C Path Setup               
base_path = Path(__file__).resolve().parent  # base directory path
so_file = base_path / '../keccak.so'         # Keccak shared object - direct file path
keccak_c = CDLL(str(so_file))                # Loads shared object file using ctypes

# Python Path Setup
submodule_dir_path = Path(__file__).resolve().parent.parent / 'keccak'  # Keccak submodule directory path - python implementation
submodule_dir_path = submodule_dir_path.resolve().absolute()            # Converts submodule dir path to absolute path
sys.path.append(str(submodule_dir_path))                                # Adds submodule dir to pythons import search paths
import keccak as keccak_py                                              # Imports keccak.py from submodule dir

# Python 2/3 compatibility patches
keccak_py.xrange = six.moves.range                      # Xrange not comptabible with Python 3
keccak_py.reduce = functools.reduce                     # Python 3 no longer supports native 'reduce'
keccak_py.bits2bytes = lambda x: (int(x) + 7) // 8      # Patch for '/' - to ensure result is always int '//'
keccak_py.KeccakState.str2bytes = lambda ss: list(ss)   # Patch for string/byte conversion 


def generate_random_buffers(num=3, max_length=1024):
  buffers = []
  for i in range(num):
    length = secrets.randbelow(max_length) + 1
    buffers.append(secrets.token_bytes(length))
  return buffers

# Create Array of random states for testing
def generate_random_states(num=3):
  states = []
  for i in range(num):
    # create array of 25 random 64-bit ints to populate state
    state = [secrets.randbits(64) for _ in range(25)]
    states.append(state)
  return states


# Transforms array of 25 values to 5*5 C matrix
def transform_to_c_state(values):
  row = c_uint64 * 5      # Define row type (5 elements of uint64) using lib c_types  
  c_state = (row * 5)()   # Instantiate 5*5 matrix with c_row - all elements are set to zero  
  
  for x in range(5):      # Populate c_state with random values
    for y in range(5):
      c_state[x][y] = values[x * 5 + y]
  return c_state 


# Transforms array of 25 values to 5*5 Python matrix 
def transform_to_py_state(values):
  # Initialise state with 'keccakState' object from keccak.py file
  keccak_state = keccak_py.KeccakState(1088, 1600) # bitrate 1088, b=1600

  # Populate keccak_state.s matrix values
  for x in range(5):
    for y in range(5):
      keccak_state.s[x][y] = values[x * 5 + y]
  return keccak_state

def c_state_to_py_list(c_state):
  result = []
  for x in range(5):
    row = []
    for y in range(5):
      row.append(c_state[x][y])
    result.append(row)
  return result



class TestKeccakMethods(unittest.TestCase):

  '''
  Keccf_f Unit Test
  '''
  def test_keccak_f(self):
    states = generate_random_states(3) # generates 3 random states for testing
    test_results = []

    for state in states:

      # Instantiate states
      c_state = transform_to_c_state(state) 
      py_state = transform_to_py_state(state)

      # Call keccak_f
      keccak_c.keccak_f(c_state)
      keccak_py.keccak_f(py_state)

      # Extract state 's' from keccak_py 
      py_result = py_state.s

      # Convert c_state to python list for fair comparison
      c_result = c_state_to_py_list(c_state)

      # Print Results
      print(f"C Result: \n{c_result}")
      print(f"Python Result: \n{py_result}")

      # Tests
      test_results.append((c_result == py_result))
      self.assertEqual(c_result, py_result, "C and Python results should be the same")
    
    print("================================ Keccak-f: Test Results ", test_results)


  '''
  Padding Unit Test
  '''
  def test_padding(self):
    buffers = generate_random_buffers(3)
    test_results = []
    
    for buffer in buffers: 

      # Setup params for Python & C implementation
      blocksize = 136                                               # Blocks size for SHA-3-256
      message_length = len(buffer)                                  # length of input message in bytes
      padding_length = blocksize - (message_length % blocksize)     # required padding length
      padded_message_length = message_length + padding_length       # total length of padded message

      # Instantiate buffers
      c_buffer = create_string_buffer(buffer, padded_message_length)
      py_buffer = list(buffer)


      # Call padding functions
      keccak_c.padding(c_buffer, c_size_t(message_length), c_size_t(padding_length))  # updates c_buffer
      py_padding = keccak_py.multirate_padding(message_length % blocksize, blocksize) # returns buffer padding 


      # Padded message results
      c_result = list(c_buffer.raw)       # C padded message
      py_result = py_buffer + py_padding  # Py padded message

      # Print Results
      print(f"C Result: \n{c_result}")
      print(f"Python Result: \n{py_result}")

      # Tests
      test_results.append((c_result == py_result))
      self.assertEqual(c_result, py_result, "C and Python results should be the same")
    
    print("================================ Padding: Test Results ", test_results)

      
  '''
  Initialise State Unit Test
  '''
  def test_initialise_state(self):

    test_results = []

    # Instantiate Python state
    py_state = keccak_py.KeccakState.zero()
    py_result = py_state

    # Create C state
    c_state = (c_uint64 * 5 * 5)()
    # Initialise C state
    keccak_c.initialise_state(c_state)
    # Convert c_state to python list for fair comparison
    c_result = c_state_to_py_list(c_state)

    # Print Results
    print(f"C Result: \n{c_result}")
    print(f"Python Result: \n{py_result}")

    # Tests
    test_results.append((c_result == py_result))
    self.assertEqual(c_result, py_result, "C and Python results should be the same")
    
    print("================================ Initialise State: Test Results ", test_results)
    
  '''
  Absorbing Unit Test
  '''
  def test_absorbing(self):
    buffers = generate_random_buffers(3)
    test_results = []

    for buffer in buffers:
      print(f"Buffer to absorb: \n{buffer}")
      print(f"Buffer Length: \n{len(buffer)}")
      # Initialise Python state
      py_sponge = keccak_py.KeccakSponge(1088, 1600, keccak_py.multirate_padding, keccak_py.keccak_f)
      
      print(f"Py State before absorbing: \n{py_sponge.state.s}")
      
      py_sponge.absorb(buffer)
      py_result = py_sponge.state.s

      # Initialise C state
      c_state = (c_uint64 * 5 * 5)()
      keccak_c.initialise_state(c_state)

      print(f"C State before absorbing: \n{c_state_to_py_list(c_state)}")

      # Convert buffer to the correct C type
      c_buffer = (c_uint8 * len(buffer))(*buffer)

      # Call C absorbing function
      keccak_c.absorbing(c_state, c_buffer, len(c_buffer))

      c_result = c_state_to_py_list(c_state)

      # Print Results
      print(f"C Result: \n{c_result}")
      print(f"Python Result: \n{py_result}")

      # Tests
      test_results.append((c_result == py_result))
      self.assertEqual(c_result, py_result, "C and Python results should be the same")

    print("================================ Absorbing: Test Results ", test_results)


  '''
  Squeezing Unit Test
  '''
  def test_squeezing(self):
    states = generate_random_states(3)
    test_results = []

    for state in states:
      
      # Instantiate states
      c_state = transform_to_c_state(state)
      py_state = transform_to_py_state(state)

      # Call C Squeezing function
      output = (c_uint8 * 32)()               # allocates buffer of 32 bytes for output
      keccak_c.squeezing(c_state, output)     # pass c_state and output to squeezing function
      c_result = list(output)                 # convert output to py list

      # Call Python squeezing function
      py_sponge = keccak_py.KeccakSponge(1088, 1600, keccak_py.multirate_padding, keccak_py.keccak_f)
      print(f"py_state.s: {py_state.s}")
      py_sponge.state.s = py_state.s          # set sponge state to py_state
      py_result = list(py_sponge.squeeze(32)) # 32-byte output
    

      # Print Results
      print(f"C Result: \n{c_result}")
      print(f"Python Result: \n{py_result}")

      # Tests
      test_results.append((c_result == py_result))
      self.assertEqual(c_result, py_result, "C and Python results should be the same")

    print("================================ Squeezing: Test Results ", test_results)



  '''
  Keccak Hash Function Unit Test
  '''
  def test_keccak_hash(self):
    buffers = generate_random_buffers(3)
    test_results = []

    for buffer in buffers:

      # Python implementation
      keccak_py_hash = keccak_py.Keccak256()
      keccak_py_hash.update(buffer)
      py_digest_str = keccak_py_hash.digest()                               # Gets digest as string of byte values
      py_digest_hex = py_digest_str.encode('latin1').hex()                  # Convert str (to bytes) to hex
      py_result = py_digest_hex


      # C implementation
      c_input = (c_uint8 * len(buffer))(*buffer)                            # Create C array from input string
      keccak_c.keccak_hash.restype = POINTER(c_uint8 * 32)                  # Define return type
      keccak_c_hash_ptr = keccak_c.keccak_hash(c_input, len(buffer)) 
      c_hash_array = keccak_c_hash_ptr.contents                             # Gets c types array of byte values
      c_hash_hex = bytes(c_hash_array).hex()                                # Convert ctypes array (to bytes) to hex
      c_result = c_hash_hex

      # Print Results
      print(f"C Result: \n{c_result}")
      print(f"Python Result: \n{py_result}")

      # Tests
      test_results.append((c_result == py_result))
      self.assertEqual(c_result, py_result, "C and Python results should be the same")

      print("================================ Keccak Hash Test Results ", test_results)

if __name__== '__main__':
  unittest.main()
