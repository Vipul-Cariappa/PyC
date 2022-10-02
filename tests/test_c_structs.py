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


class Test_c_struct(unittest.TestCase):
    def test_initialing_and_conversions(self):
        ra = cModule.RECT([cModule.RECT(1, 2), cModule.RECT(2, 3)])
        ra.append(cModule.RECT(3, 4))
        ra.append(cModule.RECT(3, 4))
        r = ra.pop()

        for _i, i in enumerate(ra):
            assert ra[_i] is i

        self.assertTrue(ra.is_array)

        for index, element in enumerate(ra):
            self.assertEqual(element.x, index + 1)
            self.assertEqual(element.y, index + 2)
            self.assertEqual(ra[index].x, index + 1)
            self.assertEqual(ra[index].y, index + 2)

        self.assertRaises(IndexError, lambda: ra[3])

        ra[2] = cModule.RECT(20, 40)
        for _i, i in enumerate(ra):
            assert ra[_i] is i

    def test_array_and_iterations(self):
        ...

    def test_array_and_access(self):
        ...

    def test_array_method_and_attributes(self):
        ...
