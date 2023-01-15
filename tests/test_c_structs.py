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
from PyC import BindingError, CppError

# TODO: update

cModule = loader("tests/libcmodule.so", "tests/c/module.h")
cppModule = loader("tests/libcppmodule.so", "tests/c/module.hpp", cpp=True)


class Test_c_struct(unittest.TestCase):
    def test_initialing_and_conversions(self):
        r1 = cModule.RECT(1, 2)
        self.assertEqual(r1.x, 1)
        self.assertEqual(r1.y, 2)

        r1.x = 2
        r1.y = 3
        self.assertEqual(r1.x, 2)
        self.assertEqual(r1.y, 3)

        self.assertFalse(r1.is_array)
        self.assertTrue(r1.free_on_no_reference)

        r2 = cModule.RECT()
        r2.x = 2
        r2.y = 3
        self.assertEqual(r2.x, 2)
        self.assertEqual(r2.y, 3)

        self.assertFalse(r2.is_array)
        self.assertTrue(r2.free_on_no_reference)

        self.assertRaises(BindingError, cModule.RECT, 12)
        self.assertRaises(BindingError, cModule.RECT, 12, 123, 1234)
        self.assertRaises(Exception, cModule.RECT, "vipul", "cariappa")

    def test_array_iterations_and_access(self):
        ra0 = cModule.RECT([])

        for i in ra0:
            # this should not run
            raise Exception

        ra = cModule.RECT(
            [
                cModule.RECT(1, 2),
                cModule.RECT(2, 3),
                cModule.RECT(3, 4),
                cModule.RECT(4, 5),
            ]
        )

        for index, element in enumerate(ra):
            # self.assertTrue(ra[index] is element)
            self.assertEqual(element.x, index + 1)
            self.assertEqual(element.y, index + 2)

    def test_array_attributes_and_methods(self):
        ra0 = cModule.RECT([])

        self.assertEqual(len(ra0), 0)

        self.assertRaises(IndexError, ra0.pop)
        ra0.append(cModule.RECT(12, 24))

        self.assertTrue(ra0.is_array)
        self.assertTrue(ra0.free_on_no_reference)

        self.assertEqual(len(ra0), 1)

        ra = cModule.RECT([cModule.RECT(1, 2), cModule.RECT(2, 3)])
        self.assertTrue(ra.is_array)
        self.assertTrue(ra.free_on_no_reference)

        self.assertEqual(len(ra), 2)

        ra.append(cModule.RECT(3, 4))
        ra.append(cModule.RECT(4, 5))

        self.assertEqual(len(ra), 4)

        for index, element in enumerate(ra):
            # self.assertTrue(ra[index] is element)
            self.assertEqual(element.x, index + 1)
            self.assertEqual(element.y, index + 2)

        for i in range(len(ra)):
            element = ra.pop()
            self.assertEqual(element.x, 4 - i)
            self.assertEqual(element.y, 5 - i)

        self.assertRaises(IndexError, ra.pop)

        ra.append(cModule.RECT(12, 24))
