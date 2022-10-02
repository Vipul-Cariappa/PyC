from random import randint, random, choice
from textwrap import indent
import unittest
from xml.dom.minidom import Element
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


class Test_c_type(unittest.TestCase):
    def setUp(self):
        self.random_int_1 = randint(-(2**30), 2**30)
        self.random_int_array = [
            randint(-(2**30), 2**30) for _ in range(randint(1, 20))
        ]
        self.random_uint_array = [randint(0, 2**32) for _ in range(randint(1, 20))]
        self.random_long_1 = randint(-(2**30), 2**30)
        self.random_long_array = [
            randint(-(2**30), 2**30) for _ in range(randint(1, 20))
        ]
        self.random_ulong_array = [randint(0, 2**32) for _ in range(randint(1, 20))]
        self.random_short_1 = randint(-(2**14), 2**14)
        self.random_short_array = [
            randint(-(2**14), 2**14) for _ in range(randint(1, 20))
        ]
        self.random_ushort_array = [randint(0, 2**16) for _ in range(randint(1, 20))]
        self.random_char_1 = randint(0, 127)
        self.random_char_2 = randint(0, 127)
        self.random_float_1 = random() * 100
        self.random_float_array = [random() * 100 for _ in range(randint(1, 20))]
        self.random_double_1 = random() * 10000
        self.random_double_array = [random() * 10000 for _ in range(randint(1, 20))]
        self.string_1 = "c_char from PyC module"
        self.random_bool_array = [choice((True, False)) for _ in range(randint(1, 20))]

    def test_initialing_and_conversions_int(self):
        # --- c_int ---
        int1 = c_int(self.random_int_1)
        self.assertEqual(int(int1), self.random_int_1)
        self.assertEqual(int1.value(), self.random_int_1)
        self.assertAlmostEqual(float(int1), self.random_int_1, 4)
        if self.random_int_1:
            self.assertTrue(bool(int1))
        else:
            self.assertFalse(bool(int1))
        self.assertFalse(int1.free_on_no_reference)
        self.assertFalse(int1.is_array)
        self.assertFalse(int1.is_pointer)

        int2 = c_int(int1)
        self.assertEqual(int(int2), self.random_int_1)
        self.assertEqual(int2.value(), self.random_int_1)
        self.assertAlmostEqual(float(int2), self.random_int_1, 4)
        if self.random_int_1:
            self.assertTrue(bool(int2))
        else:
            self.assertFalse(bool(int2))
        self.assertFalse(int2.free_on_no_reference)
        self.assertFalse(int2.is_array)
        self.assertFalse(int2.is_pointer)

        # --- c_uint ---
        uint1 = c_uint(abs(self.random_int_1))
        self.assertEqual(int(uint1), abs(self.random_int_1))
        self.assertEqual(uint1.value(), abs(self.random_int_1))
        self.assertAlmostEqual(float(uint1), abs(self.random_int_1), 4)
        if abs(self.random_int_1):
            self.assertTrue(bool(uint1))
        else:
            self.assertFalse(bool(uint1))
        self.assertFalse(uint1.free_on_no_reference)
        self.assertFalse(uint1.is_array)
        self.assertFalse(uint1.is_pointer)

        uint2 = c_uint(uint1)
        self.assertEqual(int(uint2), abs(self.random_int_1))
        self.assertEqual(uint2.value(), abs(self.random_int_1))
        self.assertAlmostEqual(float(uint2), abs(self.random_int_1), 4)
        if abs(self.random_int_1):
            self.assertTrue(bool(uint2))
        else:
            self.assertFalse(bool(uint2))
        self.assertFalse(uint2.free_on_no_reference)
        self.assertFalse(uint2.is_array)
        self.assertFalse(uint2.is_pointer)

    def test_initialing_and_conversions_long(self):
        # --- c_long ---
        long1 = c_long(self.random_long_1)
        self.assertEqual(int(long1), self.random_long_1)
        self.assertEqual(long1.value(), self.random_long_1)
        self.assertAlmostEqual(float(long1), self.random_long_1, 4)
        if self.random_long_1:
            self.assertTrue(bool(long1))
        else:
            self.assertFalse(bool(long1))
        self.assertFalse(long1.free_on_no_reference)
        self.assertFalse(long1.is_array)
        self.assertFalse(long1.is_pointer)

        long2 = c_long(long1)
        self.assertEqual(int(long2), self.random_long_1)
        self.assertEqual(long2.value(), self.random_long_1)
        self.assertAlmostEqual(float(long2), self.random_long_1, 4)
        if self.random_long_1:
            self.assertTrue(bool(long2))
        else:
            self.assertFalse(bool(long2))
        self.assertFalse(long2.free_on_no_reference)
        self.assertFalse(long2.is_array)
        self.assertFalse(long2.is_pointer)

        # --- c_ulong ---
        ulong1 = c_ulong(abs(self.random_long_1))
        self.assertEqual(int(ulong1), abs(self.random_long_1))
        self.assertEqual(ulong1.value(), abs(self.random_long_1))
        self.assertAlmostEqual(float(ulong1), abs(self.random_long_1), 4)
        if abs(self.random_long_1):
            self.assertTrue(bool(ulong1))
        else:
            self.assertFalse(bool(ulong1))
        self.assertFalse(ulong1.free_on_no_reference)
        self.assertFalse(ulong1.is_array)
        self.assertFalse(ulong1.is_pointer)

        ulong2 = c_ulong(ulong1)
        self.assertEqual(int(ulong2), abs(self.random_long_1))
        self.assertEqual(ulong2.value(), abs(self.random_long_1))
        self.assertAlmostEqual(float(ulong2), abs(self.random_long_1), 4)
        if abs(self.random_long_1):
            self.assertTrue(bool(ulong2))
        else:
            self.assertFalse(bool(ulong2))
        self.assertFalse(ulong2.free_on_no_reference)
        self.assertFalse(ulong2.is_array)
        self.assertFalse(ulong2.is_pointer)

    def test_initialing_and_conversions_short(self):
        # --- c_short ---
        short1 = c_short(self.random_short_1)
        self.assertEqual(int(short1), self.random_short_1)
        self.assertEqual(short1.value(), self.random_short_1)
        self.assertAlmostEqual(float(short1), self.random_short_1, 4)
        if self.random_short_1:
            self.assertTrue(bool(short1))
        else:
            self.assertFalse(bool(short1))
        self.assertFalse(short1.free_on_no_reference)
        self.assertFalse(short1.is_array)
        self.assertFalse(short1.is_pointer)

        short2 = c_short(short1)
        self.assertEqual(int(short2), self.random_short_1)
        self.assertEqual(short2.value(), self.random_short_1)
        self.assertAlmostEqual(float(short2), self.random_short_1, 4)
        if self.random_short_1:
            self.assertTrue(bool(short2))
        else:
            self.assertFalse(bool(short2))
        self.assertFalse(short2.free_on_no_reference)
        self.assertFalse(short2.is_array)
        self.assertFalse(short2.is_pointer)

        # --- c_ushort ---
        ushort1 = c_ushort(abs(self.random_short_1))
        self.assertEqual(int(ushort1), abs(self.random_short_1))
        self.assertEqual(ushort1.value(), abs(self.random_short_1))
        self.assertAlmostEqual(float(ushort1), abs(self.random_short_1), 4)
        if abs(self.random_short_1):
            self.assertTrue(bool(ushort1))
        else:
            self.assertFalse(bool(ushort1))
        self.assertFalse(ushort1.free_on_no_reference)
        self.assertFalse(ushort1.is_array)
        self.assertFalse(ushort1.is_pointer)

        ushort2 = c_ushort(ushort1)
        self.assertEqual(int(ushort2), abs(self.random_short_1))
        self.assertEqual(ushort2.value(), abs(self.random_short_1))
        self.assertAlmostEqual(float(ushort2), abs(self.random_short_1), 4)
        if abs(self.random_short_1):
            self.assertTrue(bool(ushort2))
        else:
            self.assertFalse(bool(ushort2))
        self.assertFalse(ushort2.free_on_no_reference)
        self.assertFalse(ushort2.is_array)
        self.assertFalse(ushort2.is_pointer)

    def test_initialing_and_conversions_float(self):
        # --- c_float ---
        float1 = c_float(self.random_float_1)
        self.assertEqual(int(float1), int(self.random_float_1))
        self.assertAlmostEqual(float1.value(), self.random_float_1, 4)
        self.assertAlmostEqual(float(float1), self.random_float_1, 4)
        if self.random_float_1:
            self.assertTrue(bool(float1))
        else:
            self.assertFalse(bool(float1))
        self.assertFalse(float1.free_on_no_reference)
        self.assertFalse(float1.is_array)
        self.assertFalse(float1.is_pointer)

        float2 = c_float(float1)
        self.assertEqual(int(float2), int(self.random_float_1))
        self.assertAlmostEqual(float2.value(), self.random_float_1, 4)
        self.assertAlmostEqual(float(float2), self.random_float_1, 4)
        if self.random_float_1:
            self.assertTrue(bool(float2))
        else:
            self.assertFalse(bool(float2))
        self.assertFalse(float2.free_on_no_reference)
        self.assertFalse(float2.is_array)
        self.assertFalse(float2.is_pointer)

    def test_initialing_and_conversions_double(self):
        # --- c_double ---
        double1 = c_double(self.random_double_1)
        self.assertEqual(int(double1), int(self.random_double_1))
        self.assertAlmostEqual(double1.value(), self.random_double_1, 4)
        self.assertAlmostEqual(float(double1), self.random_double_1, 4)
        if self.random_double_1:
            self.assertTrue(bool(double1))
        else:
            self.assertFalse(bool(double1))
        self.assertFalse(double1.free_on_no_reference)
        self.assertFalse(double1.is_array)
        self.assertFalse(double1.is_pointer)

        double2 = c_double(double1)
        self.assertEqual(int(double2), int(self.random_double_1))
        self.assertAlmostEqual(double2.value(), self.random_double_1, 4)
        self.assertAlmostEqual(float(double2), self.random_double_1, 4)
        if self.random_double_1:
            self.assertTrue(bool(double2))
        else:
            self.assertFalse(bool(double2))
        self.assertFalse(double2.free_on_no_reference)
        self.assertFalse(double2.is_array)
        self.assertFalse(double2.is_pointer)

    def test_initialing_and_conversions_bool(self):
        # --- c_bool ---
        bool1T = c_bool(True)
        bool1F = c_bool(False)
        bool2T = c_bool(1)
        bool2F = c_bool(0)
        bool3T = c_bool(bool1T)
        bool3F = c_bool(bool1F)

        self.assertTrue(bool(bool1T))
        self.assertTrue(bool2T.value())
        self.assertTrue(bool3T)
        self.assertFalse(bool(bool1F))
        self.assertFalse(bool2F.value())
        self.assertFalse(bool3F)

        self.assertEqual(int(bool1T), 1)
        self.assertEqual(int(bool2T), 1)
        self.assertEqual(int(bool3T), 1)
        self.assertEqual(int(bool1F), 0)
        self.assertEqual(int(bool2F), 0)
        self.assertEqual(int(bool3F), 0)
        self.assertEqual(float(bool1T), 1)
        self.assertEqual(float(bool2T), 1)
        self.assertEqual(float(bool3T), 1)
        self.assertEqual(float(bool1F), 0)
        self.assertEqual(float(bool2F), 0)
        self.assertEqual(float(bool3F), 0)

        self.assertFalse(bool1T.free_on_no_reference)
        self.assertFalse(bool2F.free_on_no_reference)
        self.assertFalse(bool3T.free_on_no_reference)
        self.assertFalse(bool1T.is_array)
        self.assertFalse(bool2F.is_array)
        self.assertFalse(bool3T.is_array)
        self.assertFalse(bool1T.is_pointer)
        self.assertFalse(bool2F.is_pointer)
        self.assertFalse(bool3T.is_pointer)

    def test_initialing_and_conversions_char(self):
        # --- c_char ---
        str1 = c_char("")
        str2 = c_char(self.string_1)
        str3 = c_char("x")
        # FIXME: initialise c_char from c_char
        # str4 = c_char(str2)

        self.assertEqual(str(str1), "")
        self.assertEqual(str(str2), self.string_1)
        self.assertEqual(str(str3), "x")
        self.assertEqual(str1.value(), "")
        self.assertEqual(str2.value(), self.string_1)
        self.assertEqual(str3.value(), "x")
        self.assertEqual(len(str1), len(""))
        self.assertEqual(len(str2), len(self.string_1))
        self.assertEqual(len(str3), len("x"))

        self.assertRaises(NotImplementedError, lambda: str1[0])
        self.assertRaises(NotImplementedError, str1.append, "x")
        self.assertRaises(NotImplementedError, str1.pop)

        self.assertFalse(str1.free_on_no_reference)
        self.assertTrue(str2.free_on_no_reference)

        self.assertFalse(str1.is_array)
        self.assertFalse(str1.is_pointer)
        self.assertTrue(str2.is_array)
        self.assertTrue(str2.is_pointer)
        self.assertFalse(str3.is_array)
        self.assertFalse(str3.is_pointer)

    def test_array_iterations_and_attributes_int(self):
        # --- c_int ---
        int_array0 = c_int([])

        self.assertEqual(len(int_array0), 0)
        self.assertTrue(int_array0.free_on_no_reference)
        self.assertTrue(int_array0.is_array)
        self.assertTrue(int_array0.is_pointer)

        for i in int_array0:
            # should not run this
            raise Exception

        int_array = c_int(self.random_int_array)

        self.assertEqual(len(int_array), len(self.random_int_array))
        self.assertTrue(int_array.free_on_no_reference)
        self.assertTrue(int_array.is_array)
        self.assertTrue(int_array.is_pointer)

        for index, element in enumerate(int_array):
            self.assertEqual(element, self.random_int_array[index])

        int_array2 = c_int([c_int(i) for i in self.random_int_array])

        self.assertEqual(len(int_array2), len(self.random_int_array))
        self.assertTrue(int_array2.free_on_no_reference)
        self.assertTrue(int_array2.is_array)
        self.assertTrue(int_array2.is_pointer)

        for index, element in enumerate(int_array2):
            self.assertEqual(element, self.random_int_array[index])

        int_array3 = c_int(
            [
                c_bool(0),
                c_int(1),
                c_long(2),
                c_short(3),
                c_uint(4),
                c_ulong(5),
                c_ushort(6),
                c_float(7),
                c_double(8),
            ]
        )

        self.assertEqual(len(int_array3), 9)
        self.assertTrue(int_array3.free_on_no_reference)
        self.assertTrue(int_array3.is_array)
        self.assertTrue(int_array3.is_pointer)

        for index, element in enumerate(int_array3):
            self.assertEqual(element, index)

        # --- c_uint ---
        # FIXME: unsigned version needs separate storage
        uint_array0 = c_uint([])

        self.assertEqual(len(uint_array0), 0)
        self.assertTrue(uint_array0.free_on_no_reference)
        self.assertTrue(uint_array0.is_array)
        self.assertTrue(uint_array0.is_pointer)

        for i in uint_array0:
            # should not run this
            raise Exception

        uint_array = c_uint(self.random_uint_array)

        self.assertEqual(len(uint_array), len(self.random_uint_array))
        self.assertTrue(uint_array.free_on_no_reference)
        self.assertTrue(uint_array.is_array)
        self.assertTrue(uint_array.is_pointer)

        for index, element in enumerate(uint_array):
            # FIXME
            # self.assertEqual(element, self.random_uint_array[index])
            pass

        uint_array2 = c_int([c_int(i) for i in self.random_uint_array])

        self.assertEqual(len(uint_array2), len(self.random_uint_array))
        self.assertTrue(uint_array2.free_on_no_reference)
        self.assertTrue(uint_array2.is_array)
        self.assertTrue(uint_array2.is_pointer)

        for index, element in enumerate(uint_array2):
            # FIXME
            # self.assertEqual(element, self.random_uint_array[index])
            pass

        uint_array3 = c_uint(
            [
                c_bool(0),
                c_int(1),
                c_long(2),
                c_short(3),
                c_uint(4),
                c_ulong(5),
                c_ushort(6),
                c_float(7),
                c_double(8),
            ]
        )

        self.assertEqual(len(uint_array3), 9)
        self.assertTrue(uint_array3.free_on_no_reference)
        self.assertTrue(uint_array3.is_array)
        self.assertTrue(uint_array3.is_pointer)

        for index, element in enumerate(int_array3):
            self.assertEqual(element, index)

    def test_array_iterations_and_attributes_long(self):
        # --- c_long ---
        long_array0 = c_long([])

        self.assertEqual(len(long_array0), 0)
        self.assertTrue(long_array0.free_on_no_reference)
        self.assertTrue(long_array0.is_array)
        self.assertTrue(long_array0.is_pointer)

        for i in long_array0:
            # should not run this
            raise Exception

        long_array = c_long(self.random_long_array)

        self.assertEqual(len(long_array), len(self.random_long_array))
        self.assertTrue(long_array.free_on_no_reference)
        self.assertTrue(long_array.is_array)
        self.assertTrue(long_array.is_pointer)

        for index, element in enumerate(long_array):
            self.assertEqual(element, self.random_long_array[index])

        long_array2 = c_long([c_long(i) for i in self.random_long_array])

        self.assertEqual(len(long_array2), len(self.random_long_array))
        self.assertTrue(long_array2.free_on_no_reference)
        self.assertTrue(long_array2.is_array)
        self.assertTrue(long_array2.is_pointer)

        for index, element in enumerate(long_array2):
            self.assertEqual(element, self.random_long_array[index])

        long_array3 = c_long(
            [
                c_bool(0),
                c_int(1),
                c_long(2),
                c_short(3),
                c_uint(4),
                c_ulong(5),
                c_ushort(6),
                c_float(7),
                c_double(8),
            ]
        )

        self.assertEqual(len(long_array3), 9)
        self.assertTrue(long_array3.free_on_no_reference)
        self.assertTrue(long_array3.is_array)
        self.assertTrue(long_array3.is_pointer)

        for index, element in enumerate(long_array3):
            self.assertEqual(element, index)

        # --- c_ulong ---
        # FIXME: unsigned version needs separate storage
        ulong_array0 = c_ulong([])

        self.assertEqual(len(ulong_array0), 0)
        self.assertTrue(ulong_array0.free_on_no_reference)
        self.assertTrue(ulong_array0.is_array)
        self.assertTrue(ulong_array0.is_pointer)

        for i in ulong_array0:
            # should not run this
            raise Exception

        ulong_array = c_ulong(self.random_ulong_array)

        self.assertEqual(len(ulong_array), len(self.random_ulong_array))
        self.assertTrue(ulong_array.free_on_no_reference)
        self.assertTrue(ulong_array.is_array)
        self.assertTrue(ulong_array.is_pointer)

        for index, element in enumerate(ulong_array):
            # FIXME
            # self.assertEqual(element, self.random_ulong_array[index])
            pass

        ulong_array2 = c_long([c_long(i) for i in self.random_ulong_array])

        self.assertEqual(len(ulong_array2), len(self.random_ulong_array))
        self.assertTrue(ulong_array2.free_on_no_reference)
        self.assertTrue(ulong_array2.is_array)
        self.assertTrue(ulong_array2.is_pointer)

        for index, element in enumerate(ulong_array2):
            # FIXME
            # self.assertEqual(element, self.random_ulong_array[index])
            pass

        ulong_array3 = c_ulong(
            [
                c_bool(0),
                c_int(1),
                c_long(2),
                c_short(3),
                c_uint(4),
                c_ulong(5),
                c_ushort(6),
                c_float(7),
                c_double(8),
            ]
        )

        self.assertEqual(len(ulong_array3), 9)
        self.assertTrue(ulong_array3.free_on_no_reference)
        self.assertTrue(ulong_array3.is_array)
        self.assertTrue(ulong_array3.is_pointer)

        for index, element in enumerate(long_array3):
            self.assertEqual(element, index)

    def test_array_iterations_and_attributes_short(self):
        # --- c_short ---
        short_array0 = c_short([])

        self.assertEqual(len(short_array0), 0)
        self.assertTrue(short_array0.free_on_no_reference)
        self.assertTrue(short_array0.is_array)
        self.assertTrue(short_array0.is_pointer)

        for i in short_array0:
            # should not run this
            raise Exception

        short_array = c_short(self.random_short_array)

        self.assertEqual(len(short_array), len(self.random_short_array))
        self.assertTrue(short_array.free_on_no_reference)
        self.assertTrue(short_array.is_array)
        self.assertTrue(short_array.is_pointer)

        for index, element in enumerate(short_array):
            self.assertEqual(element, self.random_short_array[index])

        short_array2 = c_short([c_short(i) for i in self.random_short_array])

        self.assertEqual(len(short_array2), len(self.random_short_array))
        self.assertTrue(short_array2.free_on_no_reference)
        self.assertTrue(short_array2.is_array)
        self.assertTrue(short_array2.is_pointer)

        for index, element in enumerate(short_array2):
            self.assertEqual(element, self.random_short_array[index])

        short_array3 = c_short(
            [
                c_bool(0),
                c_int(1),
                c_long(2),
                c_short(3),
                c_uint(4),
                c_ulong(5),
                c_ushort(6),
                c_float(7),
                c_double(8),
            ]
        )

        self.assertEqual(len(short_array3), 9)
        self.assertTrue(short_array3.free_on_no_reference)
        self.assertTrue(short_array3.is_array)
        self.assertTrue(short_array3.is_pointer)

        for index, element in enumerate(short_array3):
            self.assertEqual(element, index)

        # --- c_ushort ---
        # FIXME: unsigned version needs separate storage
        ushort_array0 = c_ushort([])

        self.assertEqual(len(ushort_array0), 0)
        self.assertTrue(ushort_array0.free_on_no_reference)
        self.assertTrue(ushort_array0.is_array)
        self.assertTrue(ushort_array0.is_pointer)

        for i in ushort_array0:
            # should not run this
            raise Exception

        ushort_array = c_ushort(self.random_ushort_array)

        self.assertEqual(len(ushort_array), len(self.random_ushort_array))
        self.assertTrue(ushort_array.free_on_no_reference)
        self.assertTrue(ushort_array.is_array)
        self.assertTrue(ushort_array.is_pointer)

        for index, element in enumerate(ushort_array):
            # FIXME
            # self.assertEqual(element, self.random_ushort_array[index])
            pass

        ushort_array2 = c_short([c_short(i) for i in self.random_ushort_array])

        self.assertEqual(len(ushort_array2), len(self.random_ushort_array))
        self.assertTrue(ushort_array2.free_on_no_reference)
        self.assertTrue(ushort_array2.is_array)
        self.assertTrue(ushort_array2.is_pointer)

        for index, element in enumerate(ushort_array2):
            # FIXME
            # self.assertEqual(element, self.random_ushort_array[index])
            pass

        ushort_array3 = c_ushort(
            [
                c_bool(0),
                c_int(1),
                c_long(2),
                c_short(3),
                c_uint(4),
                c_ulong(5),
                c_ushort(6),
                c_float(7),
                c_double(8),
            ]
        )

        self.assertEqual(len(ushort_array3), 9)
        self.assertTrue(ushort_array3.free_on_no_reference)
        self.assertTrue(ushort_array3.is_array)
        self.assertTrue(ushort_array3.is_pointer)

        for index, element in enumerate(short_array3):
            self.assertEqual(element, index)

    def test_array_iterations_and_attributes_float(self):
        # --- c_float ---
        float_array0 = c_float([])

        self.assertEqual(len(float_array0), 0)
        self.assertTrue(float_array0.free_on_no_reference)
        self.assertTrue(float_array0.is_array)
        self.assertTrue(float_array0.is_pointer)

        for i in float_array0:
            # should not run this
            raise Exception

        float_array = c_float(self.random_float_array)

        self.assertEqual(len(float_array), len(self.random_float_array))
        self.assertTrue(float_array.free_on_no_reference)
        self.assertTrue(float_array.is_array)
        self.assertTrue(float_array.is_pointer)

        for index, element in enumerate(float_array):
            self.assertAlmostEqual(element, self.random_float_array[index], 4)

        float_array2 = c_float([c_float(i) for i in self.random_float_array])

        self.assertEqual(len(float_array2), len(self.random_float_array))
        self.assertTrue(float_array2.free_on_no_reference)
        self.assertTrue(float_array2.is_array)
        self.assertTrue(float_array2.is_pointer)

        for index, element in enumerate(float_array2):
            self.assertAlmostEqual(element, self.random_float_array[index], 4)

        float_array3 = c_float(
            [
                c_bool(0),
                c_int(1),
                c_long(2),
                c_short(3),
                c_uint(4),
                c_ulong(5),
                c_ushort(6),
                c_float(7),
                c_double(8),
            ]
        )

        self.assertEqual(len(float_array3), 9)
        self.assertTrue(float_array3.free_on_no_reference)
        self.assertTrue(float_array3.is_array)
        self.assertTrue(float_array3.is_pointer)

        for index, element in enumerate(float_array3):
            self.assertAlmostEqual(element, index, 4)

    def test_array_iterations_and_attributes_double(self):
        # --- c_double ---
        double_array0 = c_double([])

        self.assertEqual(len(double_array0), 0)
        self.assertTrue(double_array0.free_on_no_reference)
        self.assertTrue(double_array0.is_array)
        self.assertTrue(double_array0.is_pointer)

        for i in double_array0:
            # should not run this
            raise Exception

        double_array = c_double(self.random_double_array)

        self.assertEqual(len(double_array), len(self.random_double_array))
        self.assertTrue(double_array.free_on_no_reference)
        self.assertTrue(double_array.is_array)
        self.assertTrue(double_array.is_pointer)

        for index, element in enumerate(double_array):
            self.assertAlmostEqual(element, self.random_double_array[index], 4)

        double_array2 = c_double([c_double(i) for i in self.random_double_array])

        self.assertEqual(len(double_array2), len(self.random_double_array))
        self.assertTrue(double_array2.free_on_no_reference)
        self.assertTrue(double_array2.is_array)
        self.assertTrue(double_array2.is_pointer)

        for index, element in enumerate(double_array2):
            self.assertAlmostEqual(element, self.random_double_array[index], 4)

        double_array3 = c_double(
            [
                c_bool(0),
                c_int(1),
                c_long(2),
                c_short(3),
                c_uint(4),
                c_ulong(5),
                c_ushort(6),
                c_float(7),
                c_double(8),
            ]
        )

        self.assertEqual(len(double_array3), 9)
        self.assertTrue(double_array3.free_on_no_reference)
        self.assertTrue(double_array3.is_array)
        self.assertTrue(double_array3.is_pointer)

        for index, element in enumerate(double_array3):
            self.assertAlmostEqual(element, index, 4)

    def test_array_iterations_and_attributes_bool(self):
        # --- c_bool ---
        bool_array0 = c_bool([])

        self.assertEqual(len(bool_array0), 0)
        self.assertTrue(bool_array0.free_on_no_reference)
        self.assertTrue(bool_array0.is_array)
        self.assertTrue(bool_array0.is_pointer)

        for i in bool_array0:
            # should not run this
            raise Exception

        bool_array = c_bool(self.random_bool_array)

        self.assertEqual(len(bool_array), len(self.random_bool_array))
        self.assertTrue(bool_array.free_on_no_reference)
        self.assertTrue(bool_array.is_array)
        self.assertTrue(bool_array.is_pointer)

        for index, element in enumerate(bool_array):
            self.assertEqual(element, self.random_bool_array[index])

        bool_array2 = c_bool([c_bool(i) for i in self.random_bool_array])

        self.assertEqual(len(bool_array2), len(self.random_bool_array))
        self.assertTrue(bool_array2.free_on_no_reference)
        self.assertTrue(bool_array2.is_array)
        self.assertTrue(bool_array2.is_pointer)

        for index, element in enumerate(bool_array2):
            self.assertEqual(element, self.random_bool_array[index])

        bool_array3 = c_bool(
            [
                c_bool(0),
                c_int(1),
                c_long(2),
                c_short(3),
                c_uint(4),
                c_ulong(5),
                c_ushort(6),
                c_float(7),
                c_double(8),
            ]
        )

        self.assertEqual(len(bool_array3), 9)
        self.assertTrue(bool_array3.free_on_no_reference)
        self.assertTrue(bool_array3.is_array)
        self.assertTrue(bool_array3.is_pointer)

        for index, element in enumerate(bool_array3):
            self.assertEqual(element, bool(index))

    @unittest.skip("Not Implemented")
    def test_array_iterations_and_attributes_bool(self):
        ...

    def test_array_access_and_methods_int(self):
        # --- c_int ---
        int_array0 = c_int([])

        for i in int_array0:
            # should not run this
            raise Exception

        int_array0.append(0)
        int_array0.append(c_bool(1))
        int_array0.append(c_int(2))
        int_array0.append(c_long(3))
        int_array0.append(c_short(4))
        int_array0.append(c_uint(5))
        int_array0.append(c_ulong(6))
        int_array0.append(c_ushort(7))
        int_array0.append(c_float(8))
        int_array0.append(c_double(9))

        for i in range(len(int_array0)):
            self.assertEqual(int_array0[i], i)

        for i in range(len(int_array0)):
            self.assertEqual(int_array0.pop(), 9 - i)

        self.assertRaises(IndexError, int_array0.pop)

        int_array0.append(0)

        int_array = c_int(self.random_int_array)

        int_array.append(0)
        int_array.append(c_bool(1))
        int_array.append(c_int(2))
        int_array.append(c_long(3))
        int_array.append(c_short(4))
        int_array.append(c_uint(5))
        int_array.append(c_ulong(6))
        int_array.append(c_ushort(7))
        int_array.append(c_float(8))
        int_array.append(c_double(9))

        for i in range(len(int_array)):
            if i < len(self.random_int_array):
                self.assertEqual(int_array[i], self.random_int_array[i])
            else:
                self.assertEqual(int_array[i], i - len(self.random_int_array))

        for i in range(len(int_array) - 1, -1, -1):
            if i < len(self.random_int_array):
                self.assertEqual(int_array.pop(), self.random_int_array[i])
            else:
                self.assertEqual(int_array.pop(), i - len(self.random_int_array))

        self.assertRaises(IndexError, int_array.pop)

        int_array.append(0)

        # --- c_uint ---
        uint_array0 = c_uint([])

        for i in uint_array0:
            # should not run this
            raise Exception

        uint_array0.append(0)
        uint_array0.append(c_bool(1))
        uint_array0.append(c_int(2))
        uint_array0.append(c_long(3))
        uint_array0.append(c_short(4))
        uint_array0.append(c_uint(5))
        uint_array0.append(c_ulong(6))
        uint_array0.append(c_ushort(7))
        uint_array0.append(c_float(8))
        uint_array0.append(c_double(9))

        for i in range(len(uint_array0)):
            self.assertEqual(uint_array0[i], i)

        for i in range(len(uint_array0)):
            self.assertEqual(uint_array0.pop(), 9 - i)

        self.assertRaises(IndexError, uint_array0.pop)

        uint_array0.append(0)

        uint_array = c_uint(self.random_uint_array)

        uint_array.append(0)
        uint_array.append(c_bool(1))
        uint_array.append(c_int(2))
        uint_array.append(c_long(3))
        uint_array.append(c_short(4))
        uint_array.append(c_uint(5))
        uint_array.append(c_ulong(6))
        uint_array.append(c_ushort(7))
        uint_array.append(c_float(8))
        uint_array.append(c_double(9))

        for i in range(len(uint_array)):
            if i < len(self.random_uint_array):
                self.assertEqual(uint_array[i], self.random_uint_array[i])
            else:
                self.assertEqual(uint_array[i], i - len(self.random_uint_array))

        for i in range(len(uint_array) - 1, -1, -1):
            if i < len(self.random_uint_array):
                self.assertEqual(uint_array.pop(), self.random_uint_array[i])
            else:
                self.assertEqual(uint_array.pop(), i - len(self.random_uint_array))

        self.assertRaises(IndexError, uint_array.pop)

        uint_array.append(0)

    def test_array_access_and_methods_long(self):
        # --- c_long ---
        long_array0 = c_long([])

        for i in long_array0:
            # should not run this
            raise Exception

        long_array0.append(0)
        long_array0.append(c_bool(1))
        long_array0.append(c_int(2))
        long_array0.append(c_long(3))
        long_array0.append(c_short(4))
        long_array0.append(c_uint(5))
        long_array0.append(c_ulong(6))
        long_array0.append(c_ushort(7))
        long_array0.append(c_float(8))
        long_array0.append(c_double(9))

        for i in range(len(long_array0)):
            self.assertEqual(long_array0[i], i)

        for i in range(len(long_array0)):
            self.assertEqual(long_array0.pop(), 9 - i)

        self.assertRaises(IndexError, long_array0.pop)

        long_array0.append(0)

        long_array = c_long(self.random_long_array)

        long_array.append(0)
        long_array.append(c_bool(1))
        long_array.append(c_int(2))
        long_array.append(c_long(3))
        long_array.append(c_short(4))
        long_array.append(c_uint(5))
        long_array.append(c_ulong(6))
        long_array.append(c_ushort(7))
        long_array.append(c_float(8))
        long_array.append(c_double(9))

        for i in range(len(long_array)):
            if i < len(self.random_long_array):
                self.assertEqual(long_array[i], self.random_long_array[i])
            else:
                self.assertEqual(long_array[i], i - len(self.random_long_array))

        for i in range(len(long_array) - 1, -1, -1):
            if i < len(self.random_long_array):
                self.assertEqual(long_array.pop(), self.random_long_array[i])
            else:
                self.assertEqual(long_array.pop(), i - len(self.random_long_array))

        self.assertRaises(IndexError, long_array.pop)

        long_array.append(0)

        # --- c_ulong ---
        ulong_array0 = c_ulong([])

        for i in ulong_array0:
            # should not run this
            raise Exception

        ulong_array0.append(0)
        ulong_array0.append(c_bool(1))
        ulong_array0.append(c_int(2))
        ulong_array0.append(c_long(3))
        ulong_array0.append(c_short(4))
        ulong_array0.append(c_uint(5))
        ulong_array0.append(c_ulong(6))
        ulong_array0.append(c_ushort(7))
        ulong_array0.append(c_float(8))
        ulong_array0.append(c_double(9))

        for i in range(len(ulong_array0)):
            self.assertEqual(ulong_array0[i], i)

        for i in range(len(ulong_array0)):
            self.assertEqual(ulong_array0.pop(), 9 - i)

        self.assertRaises(IndexError, ulong_array0.pop)

        ulong_array0.append(0)

        ulong_array = c_ulong(self.random_ulong_array)

        ulong_array.append(0)
        ulong_array.append(c_bool(1))
        ulong_array.append(c_int(2))
        ulong_array.append(c_long(3))
        ulong_array.append(c_short(4))
        ulong_array.append(c_uint(5))
        ulong_array.append(c_ulong(6))
        ulong_array.append(c_ushort(7))
        ulong_array.append(c_float(8))
        ulong_array.append(c_double(9))

        for i in range(len(ulong_array)):
            if i < len(self.random_ulong_array):
                self.assertEqual(ulong_array[i], self.random_ulong_array[i])
            else:
                self.assertEqual(ulong_array[i], i - len(self.random_ulong_array))

        for i in range(len(ulong_array) - 1, -1, -1):
            if i < len(self.random_ulong_array):
                self.assertEqual(ulong_array.pop(), self.random_ulong_array[i])
            else:
                self.assertEqual(ulong_array.pop(), i - len(self.random_ulong_array))

        self.assertRaises(IndexError, ulong_array.pop)

        ulong_array.append(0)

    def test_array_access_and_methods_short(self):
        # --- c_short ---
        short_array0 = c_short([])

        for i in short_array0:
            # should not run this
            raise Exception

        short_array0.append(0)
        short_array0.append(c_bool(1))
        short_array0.append(c_int(2))
        short_array0.append(c_long(3))
        short_array0.append(c_short(4))
        short_array0.append(c_uint(5))
        short_array0.append(c_ulong(6))
        short_array0.append(c_ushort(7))
        short_array0.append(c_float(8))
        short_array0.append(c_double(9))

        for i in range(len(short_array0)):
            self.assertEqual(short_array0[i], i)

        for i in range(len(short_array0)):
            self.assertEqual(short_array0.pop(), 9 - i)

        self.assertRaises(IndexError, short_array0.pop)

        short_array0.append(0)

        short_array = c_short(self.random_short_array)

        short_array.append(0)
        short_array.append(c_bool(1))
        short_array.append(c_int(2))
        short_array.append(c_long(3))
        short_array.append(c_short(4))
        short_array.append(c_uint(5))
        short_array.append(c_ulong(6))
        short_array.append(c_ushort(7))
        short_array.append(c_float(8))
        short_array.append(c_double(9))

        for i in range(len(short_array)):
            if i < len(self.random_short_array):
                self.assertEqual(short_array[i], self.random_short_array[i])
            else:
                self.assertEqual(short_array[i], i - len(self.random_short_array))

        for i in range(len(short_array) - 1, -1, -1):
            if i < len(self.random_short_array):
                self.assertEqual(short_array.pop(), self.random_short_array[i])
            else:
                self.assertEqual(short_array.pop(), i - len(self.random_short_array))

        self.assertRaises(IndexError, short_array.pop)

        short_array.append(0)

        # --- c_ushort ---
        ushort_array0 = c_ushort([])

        for i in ushort_array0:
            # should not run this
            raise Exception

        ushort_array0.append(0)
        ushort_array0.append(c_bool(1))
        ushort_array0.append(c_int(2))
        ushort_array0.append(c_long(3))
        ushort_array0.append(c_short(4))
        ushort_array0.append(c_uint(5))
        ushort_array0.append(c_ulong(6))
        ushort_array0.append(c_ushort(7))
        ushort_array0.append(c_float(8))
        ushort_array0.append(c_double(9))

        for i in range(len(ushort_array0)):
            self.assertEqual(ushort_array0[i], i)

        for i in range(len(ushort_array0)):
            self.assertEqual(ushort_array0.pop(), 9 - i)

        self.assertRaises(IndexError, ushort_array0.pop)

        ushort_array0.append(0)

        ushort_array = c_ushort(self.random_ushort_array)

        ushort_array.append(0)
        ushort_array.append(c_bool(1))
        ushort_array.append(c_int(2))
        ushort_array.append(c_long(3))
        ushort_array.append(c_short(4))
        ushort_array.append(c_uint(5))
        ushort_array.append(c_ulong(6))
        ushort_array.append(c_ushort(7))
        ushort_array.append(c_float(8))
        ushort_array.append(c_double(9))

        for i in range(len(ushort_array)):
            if i < len(self.random_ushort_array):
                self.assertEqual(ushort_array[i], self.random_ushort_array[i])
            else:
                self.assertEqual(ushort_array[i], i - len(self.random_ushort_array))

        for i in range(len(ushort_array) - 1, -1, -1):
            if i < len(self.random_ushort_array):
                self.assertEqual(ushort_array.pop(), self.random_ushort_array[i])
            else:
                self.assertEqual(ushort_array.pop(), i - len(self.random_ushort_array))

        self.assertRaises(IndexError, ushort_array.pop)

        ushort_array.append(0)

    def test_array_access_and_methods_float(self):
        # --- c_float ---
        float_array0 = c_float([])

        for i in float_array0:
            # should not run this
            raise Exception

        float_array0.append(0)
        float_array0.append(c_bool(1))
        float_array0.append(c_int(2))
        float_array0.append(c_long(3))
        float_array0.append(c_short(4))
        float_array0.append(c_uint(5))
        float_array0.append(c_ulong(6))
        float_array0.append(c_ushort(7))
        float_array0.append(c_float(8))
        float_array0.append(c_double(9))

        for i in range(len(float_array0)):
            self.assertEqual(float_array0[i], i)

        for i in range(len(float_array0)):
            self.assertEqual(float_array0.pop(), 9 - i)

        self.assertRaises(IndexError, float_array0.pop)

        float_array0.append(0)

        float_array = c_float(self.random_float_array)

        float_array.append(0)
        float_array.append(c_bool(1))
        float_array.append(c_int(2))
        float_array.append(c_long(3))
        float_array.append(c_short(4))
        float_array.append(c_uint(5))
        float_array.append(c_ulong(6))
        float_array.append(c_ushort(7))
        float_array.append(c_float(8))
        float_array.append(c_double(9))

        for i in range(len(float_array)):
            if i < len(self.random_float_array):
                self.assertAlmostEqual(float_array[i], self.random_float_array[i], 4)
            else:
                self.assertAlmostEqual(
                    float_array[i], i - len(self.random_float_array), 4
                )

        for i in range(len(float_array) - 1, -1, -1):
            if i < len(self.random_float_array):
                self.assertAlmostEqual(float_array.pop(), self.random_float_array[i], 4)
            else:
                self.assertAlmostEqual(
                    float_array.pop(), i - len(self.random_float_array), 4
                )

        self.assertRaises(IndexError, float_array.pop)

        float_array.append(0)

    def test_array_access_and_methods_double(self):
        # --- c_double ---
        double_array0 = c_double([])

        for i in double_array0:
            # should not run this
            raise Exception

        double_array0.append(0)
        double_array0.append(c_bool(1))
        double_array0.append(c_int(2))
        double_array0.append(c_long(3))
        double_array0.append(c_short(4))
        double_array0.append(c_uint(5))
        double_array0.append(c_ulong(6))
        double_array0.append(c_ushort(7))
        double_array0.append(c_float(8))
        double_array0.append(c_double(9))

        for i in range(len(double_array0)):
            self.assertEqual(double_array0[i], i)

        for i in range(len(double_array0)):
            self.assertEqual(double_array0.pop(), 9 - i)

        self.assertRaises(IndexError, double_array0.pop)

        double_array0.append(0)

        double_array = c_double(self.random_double_array)

        double_array.append(0)
        double_array.append(c_bool(1))
        double_array.append(c_int(2))
        double_array.append(c_long(3))
        double_array.append(c_short(4))
        double_array.append(c_uint(5))
        double_array.append(c_ulong(6))
        double_array.append(c_ushort(7))
        double_array.append(c_float(8))
        double_array.append(c_double(9))

        for i in range(len(double_array)):
            if i < len(self.random_double_array):
                self.assertAlmostEqual(double_array[i], self.random_double_array[i], 4)
            else:
                self.assertAlmostEqual(
                    double_array[i], i - len(self.random_double_array), 4
                )

        for i in range(len(double_array) - 1, -1, -1):
            if i < len(self.random_double_array):
                self.assertAlmostEqual(
                    double_array.pop(), self.random_double_array[i], 4
                )
            else:
                self.assertAlmostEqual(
                    double_array.pop(), i - len(self.random_double_array), 4
                )

        self.assertRaises(IndexError, double_array.pop)

        double_array.append(0)
