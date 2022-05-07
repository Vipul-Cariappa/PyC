import unittest
from PyC import LoadCpp

cppModule = LoadCpp("tests/libcmodule.so", "tests/c/module.hpp")


class TestBasic(unittest.TestCase):
    def test_global_variables(self):
        self.assertEqual(cppModule.MAX, 5_000_000)
        self.assertAlmostEqual(cppModule.f, 3.14, 4)
        self.assertEqual(cppModule.PROGRAM, "C/C++")

    def test_simple_functions(self):
        self.assertEqual(cppModule.add(24, 46), 70)
        self.assertAlmostEqual(cppModule.pi(1_000_000), 3.14159, 4)
        self.assertEqual(cppModule.concat(
            "Vipul ", "Cariappa"), "Vipul Cariappa")


if __name__ == "__main__":
    unittest.main()
