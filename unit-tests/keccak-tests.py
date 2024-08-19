
import unittest           # python testing framework
from ctypes import *      # python library for working with C
from pathlib import Path  # object oriented filesystem paths
import sys                # modifying import path
import secrets            # for generating cryptographically secure random numbers


submodule_dir_path = Path(__file__).resolve().parent.parent / 'keccak'  # Keccak submodule directory path - python implementation
submodule_dir_path = submodule_dir_path.resolve().absolute()            # Converts submodule dir path to absolute path
sys.path.append(str(submodule_dir_path))                                # Adds submodule dir to pythons import search paths
import keccak as keccak_python                                          # Imports keccak.py from submodule dir


base_path = Path(__file__).resolve().parent # base directory path
so_file = base_path / '../keccak.so'        # Keccak shared object - direct file path
keccak_c = CDLL(str(so_file))               # Loads shared object file using ctypes


def random_state_generator():
  state_matrix = []
  for x in range(5):
    row = []
    for y in range(5):
      random_int = secrets.randbits(64) # generates a random 64-bit int - for each lane
      row.append(random_int)  
    state_matrix.append(row) 
  return state_matrix 


class TestKeccakMethods(unittest.TestCase):

  # check functions accessible from keccak.c
  def test_ctypes(self): 
    keccak_c.add.restype = c_int            # returns int
    keccak_c.add.argtypes = [c_int, c_int]  # takes two ints as args

    # Call keccak add function
    result = keccak_c.add(6, 4)
    self.assertEqual(result, 10, "keccak_c.add(6, 4) should return 10")

    result = keccak_c.add(-2, 5)
    self.assertEqual(result, 3, "keccak_c.add(-2, 5) should return 3")

    result = keccak_c.add(0, 0)
    self.assertEqual(result, 0, "keccak_c.add(0, 0) should return 0")


  # check functions accessible from keccak.py
  def test_python_submodule(self):

    result = keccak_python.bits2bytes(15)
    self.assertEqual(result, 2.75, "Keccak_python.bits2bytes(15) should return 2.75")

    result = keccak_python.bits2bytes(8)
    self.assertEqual(result, 1.875, "Keccak_python.bits2bytes(15) should return 1.875")

    result = keccak_python.bits2bytes(0)
    self.assertEqual(result, 0.875, "Keccak_python.bits2bytes(15) should return 0.875")


if __name__== '__main__':
  unittest.main()