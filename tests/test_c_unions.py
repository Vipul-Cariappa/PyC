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


class Test_c_union(unittest.TestCase):
    def test_initialing_and_conversions(self):
        r1 = cModule.Number()

        r1.i = 2
        self.assertEqual(r1.i, 2)
        r1.l = 3
        self.assertEqual(r1.l, 3)

        self.assertFalse(r1.is_array)
        self.assertTrue(r1.free_on_no_reference)

    def test_array_iterations_and_access(self):
        ra0 = cModule.Number([])

        for i in ra0:
            # this should not run
            raise Exception

        ra = cModule.Number(
            [
                cModule.Number(),
                cModule.Number(),
                cModule.Number(),
                cModule.Number(),
            ]
        )

        for index, element in enumerate(ra):
            self.assertTrue(ra[index] is element)
            ra[index].l = index

        for index, element in enumerate(ra):
            self.assertTrue(ra[index] is element)
            self.assertEqual(element.l, index)

    def test_array_attributes_and_methods(self):
        ra0 = cModule.Number([])

        self.assertEqual(len(ra0), 0)

        self.assertRaises(IndexError, ra0.pop)
        ra0.append(cModule.Number())

        self.assertTrue(ra0.is_array)
        self.assertTrue(ra0.free_on_no_reference)

        self.assertEqual(len(ra0), 1)

        ra = cModule.Number([cModule.Number(), cModule.Number()])
        self.assertTrue(ra.is_array)
        self.assertTrue(ra.free_on_no_reference)

        self.assertEqual(len(ra), 2)

        ra.append(cModule.Number())
        ra.append(cModule.Number())

        self.assertEqual(len(ra), 4)

        for index, element in enumerate(ra):
            self.assertTrue(ra[index] is element)
            ra[index].d = index

        for index, element in enumerate(ra):
            self.assertTrue(ra[index] is element)
            self.assertAlmostEqual(element.d, index)

        for i in range(len(ra)):
            element = ra.pop()
            self.assertAlmostEqual(element.d, 3 - i)

        self.assertRaises(IndexError, ra.pop)

        ra.append(cModule.Number())
