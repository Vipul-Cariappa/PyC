import unittest
from helper import loader
from PyC import (
    c_int,
    c_long,
    c_short,
    c_char,
    c_uint,
    c_ulong,
    c_ushort,
    c_float,
    c_double,
    c_bool,
)


cModule = loader("tests/libcmodule.so", "tests/c/module.h")
cppModule = loader("tests/libcppmodule.so", "tests/c/module.hpp", cpp=True)


class TestGlobals_C(unittest.TestCase):
    def test_simple_datatypes(self):
        self.assertEqual(cModule.MAX, 5_000_000)
        self.assertAlmostEqual(cModule.f, 3.14, 4)
        self.assertEqual(str(cModule.PROGRAM), "C/C++")

        self.assertEqual(cModule.global_flag, 0)
        cModule.set_global_flag(10)
        self.assertEqual(cModule.global_flag, 10)
        self.assertEqual(cModule.get_global_flag(), 10)

    @unittest.skip("Not Implemented")
    def test_pointers(self):
        ...

    @unittest.skip("Not Implemented")
    def test_arrays(self):
        ...

    @unittest.skip("Not Implemented")
    def test_structs(self):
        ...

    @unittest.skip("Not Implemented")
    def test_unions(self):
        ...


class TestGlobals_CPP(unittest.TestCase):
    def test_simple_datatypes(self):
        self.assertEqual(cppModule.MAX, 5_000_000)
        self.assertAlmostEqual(cppModule.f, 3.14, 4)
        self.assertEqual(str(cppModule.PROGRAM), "C/C++")

    @unittest.skip("Not Implemented")
    def test_pointers(self):
        ...

    @unittest.skip("Not Implemented")
    def test_arrays(self):
        ...

    @unittest.skip("Not Implemented")
    def test_structs(self):
        ...

    @unittest.skip("Not Implemented")
    def test_unions(self):
        ...
