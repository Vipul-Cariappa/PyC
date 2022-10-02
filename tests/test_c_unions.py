import unittest
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

# TODO: update

cModule = loader("tests/libcmodule.so", "tests/c/module.h")
cppModule = loader("tests/libcppmodule.so", "tests/c/module.hpp", cpp=True)


class Test_c_union(unittest.TestCase):
    def test_initialing_and_conversions(self):
        na = cModule.Number([cModule.Number(), cModule.Number()])
        na.append(cModule.Number())
        na.append(cModule.Number())

        na[0].i = 1
        na[1].i = 2
        na[2].i = 3
        na[3].i = 4

        self.assertEqual(na.pop().i, 4)

        for _i, i in enumerate(na):
            assert na[_i] is i

        self.assertTrue(na.is_array)

        for index, element in enumerate(na):
            self.assertEqual(element.i, index + 1)
            self.assertEqual(na[index].i, index + 1)

        self.assertRaises(IndexError, lambda: na[3])

        na[2] = cModule.Number()
        na[2].i = 120
        for _i, i in enumerate(na):
            assert na[_i] is i

    def test_array_and_iterations(self):
        ...

    def test_array_and_access(self):
        ...

    def test_array_method_and_attributes(self):
        ...
