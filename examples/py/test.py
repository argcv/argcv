import unittest

from examples.py import lib

class TestLibs(unittest.TestCase):

  def test_fib(self):
    self.assertEqual(lib.Fib(5), 8)

  def test_abs(self):
    self.assertEqual(lib.Abs(-1), 1)


if __name__ == '__main__':
  unittest.main()
