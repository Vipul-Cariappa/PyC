import unittest
from random import randint, random
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


class TestFunctions_C(unittest.TestCase):
    def setUp(self):
        self.random_int_1 = randint(-(2**30), 2**30)
        self.random_int_2 = randint(-(2**30), 2**30)
        self.random_long_1 = randint(-(2**30), 2**30)
        self.random_long_2 = randint(-(2**30), 2**30)
        self.random_short_1 = randint(-(2**14), 2**14)
        self.random_short_2 = randint(-(2**14), 2**14)
        self.random_char_1 = randint(0, 127)
        self.random_char_2 = randint(0, 127)
        self.random_float_1 = random() * 100
        self.random_float_2 = random() * 100

    def test_simple_functions(self):
        self.assertEqual(
            cModule.add_int(self.random_int_1, self.random_int_2),
            self.random_int_1 + self.random_int_2,
        )
        self.assertEqual(
            cModule.mul_long(self.random_long_1, self.random_long_2),
            self.random_long_1 * self.random_long_2,
        )
        self.assertEqual(
            cModule.sub_short(self.random_short_1, self.random_short_2),
            self.random_short_1 - self.random_short_2,
        )
        self.assertEqual(
            cModule.div_char(self.random_char_1, self.random_char_2),
            self.random_char_1 // self.random_char_2,
        )

        self.assertEqual(
            cModule.add_uint(abs(self.random_int_1), abs(self.random_int_2)),
            abs(self.random_int_1) + abs(self.random_int_2),
        )
        self.assertEqual(
            cModule.mul_ulong(abs(self.random_long_1), abs(self.random_long_2)),
            abs(self.random_long_1) * abs(self.random_long_2),
        )
        self.assertEqual(
            cModule.sub_ushort(abs(self.random_short_1), abs(self.random_short_2)),
            abs(abs(self.random_short_1) - abs(self.random_short_2)),
        )
        # self.assertEqual(
        #     cModule.div_char(self.random_char_1, self.random_char_2),
        #     self.random_char_1 // self.random_char_2,
        # )

        ## --- Running tests with c_type ---
        self.assertEqual(
            cModule.add_int(c_int(self.random_int_1), self.random_int_2),
            self.random_int_1 + self.random_int_2,
        )
        self.assertEqual(
            cModule.mul_long(c_long(self.random_long_1), c_long(self.random_long_2)),
            self.random_long_1 * self.random_long_2,
        )
        self.assertEqual(
            cModule.sub_short(self.random_short_1, c_short(self.random_short_2)),
            self.random_short_1 - self.random_short_2,
        )
        # FIXME: init c_char with int
        # self.assertEqual(
        #     cModule.div_char(self.random_char_1, c_char(self.random_char_2)),
        #     self.random_char_1 // self.random_char_2,
        # )

        self.assertEqual(
            cModule.add_uint(c_uint(abs(self.random_int_1)), abs(self.random_int_2)),
            abs(self.random_int_1) + abs(self.random_int_2),
        )
        self.assertEqual(
            cModule.mul_ulong(
                c_ulong(abs(self.random_long_1)), c_ulong(abs(self.random_long_2))
            ),
            abs(self.random_long_1) * abs(self.random_long_2),
        )
        self.assertEqual(
            cModule.sub_ushort(
                abs(self.random_short_1), c_ushort(abs(self.random_short_2))
            ),
            abs(abs(self.random_short_1) - abs(self.random_short_2)),
        )
        # FIXME: unsigned char not supported
        # self.assertEqual(
        #     cModule.div_char(self.random_char_1, self.random_char_2),
        #     self.random_char_1 // self.random_char_2,
        # )

        self.assertFalse(cModule.invert(True))
        self.assertTrue(cModule.invert(0))
        self.assertFalse(cModule.invert(c_bool(True)))
        self.assertTrue(cModule.invert(c_bool(0)))

        self.assertEqual(
            cModule.invert_bit(abs(self.random_long_1), abs(self.random_char_1 % 31)),
            abs(self.random_long_1) ^ (1 << abs(self.random_char_1 % 31)),
        )

        self.assertAlmostEqual(cModule.pi(1_000_000), 3.14159, 4)
        self.assertAlmostEqual(cModule.e(20), 2.71828, 4)

        full_name = cModule.string_concat("Vipul ", "Cariappa")
        full_name.free_on_no_reference = True
        self.assertEqual(str(full_name), "Vipul Cariappa")

        ## --- Running tests with c_type ---
        full_name = cModule.string_concat(c_char("Vipul "), c_char("Cariappa"))
        full_name.free_on_no_reference = True
        self.assertEqual(str(full_name), "Vipul Cariappa")

    def test_functions_with_arg_pointers(self):
        ...

    def test_functions_with_arrays(self):
        ...

    def test_functions_with_return_pointers(self):
        ...

    def test_functions_with_structs(self):
        ...

    def test_functions_with_unions(self):
        ...

    def test_functions_with_typedefs(self):
        ...


class TestFunctions_CPP(unittest.TestCase):
    def test_simple_functions(self):
        self.assertEqual(cppModule.product_int(2, 4), 8)
        self.assertEqual(cppModule.add_long_long(23, 40), 63)
        self.assertEqual(cppModule.add_short(2, 4), 6)
        self.assertEqual(cppModule.increment_1(12), 13)
        self.assertEqual(cppModule.add(24, 46), 70)

        string = cppModule.add("Vipul", 2)
        string.free_on_no_reference = True
        self.assertEqual(str(string), "VipulVipul")

        self.assertAlmostEqual(cppModule.pi(1_000_000), 3.14159, 4)

        string = cppModule.concat("Vipul ", "Cariappa")
        string.free_on_no_reference = True
        self.assertEqual(str(string), "Vipul Cariappa")

    def test_functions_with_arg_pointers(self):
        ...

    def test_functions_with_arrays(self):
        ...

    def test_functions_with_return_pointers(self):
        ...

    def test_functions_with_structs(self):
        ...

    def test_functions_with_unions(self):
        ...

    def test_functions_with_typedefs(self):
        ...
