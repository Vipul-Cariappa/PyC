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
        self.random_double_1 = random() * 1000
        self.random_double_2 = random() * 1000

    def test_simple_functions(self):
        self.assertEqual(
            cModule.add_int(self.random_int_1, self.random_int_2),
            self.random_int_1 + self.random_int_2,
        )
        self.assertEqual(
            cModule.mul_long(self.random_char_1, self.random_char_2),
            self.random_char_1 * self.random_char_2,
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
            cModule.mul_long(c_long(self.random_char_1), c_long(self.random_char_2)),
            self.random_char_1 * self.random_char_2,
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
        self.assertEqual(
            cModule.div_char(self.random_char_1, self.random_char_2),
            self.random_char_1 // self.random_char_2,
        )

        self.assertFalse(cModule.invert(True))
        self.assertTrue(cModule.invert(0))
        self.assertFalse(cModule.invert(c_bool(True)))
        self.assertTrue(cModule.invert(c_bool(0)))

        self.assertEqual(cModule.invert_enum(1), 0)
        self.assertEqual(cModule.invert_enum(0), 1)
        self.assertEqual(cModule.invert_enum(cModule.TRUE), 0)
        self.assertEqual(cModule.invert_enum(cModule.FALSE), 1)

        self.assertEqual(
            cModule.invert_bit(abs(self.random_long_1), abs(self.random_char_1 % 31)),
            abs(self.random_long_1) ^ (1 << abs(self.random_char_1 % 31)),
        )

        self.assertAlmostEqual(cModule.pi(1_000_000), 3.14159, 4)
        self.assertAlmostEqual(cModule.e(20), 2.71828, 4)

    def test_functions_with_pointers(self):
        n1 = c_int(self.random_int_1)
        n2 = c_int(self.random_int_2)
        cModule.copy_int(n1, n2)
        self.assertEqual(n1.value(), self.random_int_2)
        self.assertEqual(n2.value(), self.random_int_2)

        d1 = c_double(self.random_double_1)
        d2 = c_double(self.random_double_2)
        cModule.copy_double(d1, d2)
        self.assertAlmostEqual(d1.value(), self.random_double_2, 4)
        self.assertAlmostEqual(d2.value(), self.random_double_2, 4)

        ## --- char tests with python str ---
        # full_name = cModule.string_concat("Vipul ", "Cariappa")
        full_name = cModule.string_concat(c_char("Vipul "), c_char("Cariappa"))
        full_name.free_on_no_reference = True
        self.assertEqual(str(full_name), "Vipul Cariappa")

        ## --- char tests with c_char ---
        full_name = cModule.string_concat(c_char("Vipul "), c_char("Cariappa"))
        full_name.free_on_no_reference = True
        self.assertEqual(str(full_name), "Vipul Cariappa")

    def test_functions_with_arrays(self):
        bool_list = c_bool([0, 0, 0])
        cModule.init_bool_list_with_true(bool_list, len(bool_list))
        for i in bool_list:
            self.assertTrue(i)

        list_power_2 = cModule.powers_of_two_list(20)  # TODO: change a pointer to array
        list_power_2.free_on_no_reference = True

    # def test_functions_with_structs(self):
    #     # TODO: update to use randomly generated number
    #     r = cModule.RECT

    #     r1 = r()
    #     r1.x = 200
    #     r1.y = 100

    #     r2 = r()
    #     r2.x = 250
    #     r2.y = 140

    #     self.assertEqual(cModule.get_area(r1), 200 * 100)
    #     self.assertEqual(cModule.get_perimeter(r1), 2 * (200 + 100))

    #     self.assertEqual(cModule.get_area(r2), 250 * 140)
    #     self.assertEqual(cModule.get_perimeter(r2), 2 * (250 + 140))

    #     self.assertFalse(cModule.same_rects(r1, r2))
    #     self.assertTrue(cModule.same_rects(r1, r1))

    #     t = cModule.create_rect(1, 2)
    #     f = cModule.rect_add_from_ptr(r1, r2)
    #     f.free_on_no_reference = True

    #     self.assertEqual(t.x, 1)
    #     self.assertEqual(t.y, 2)
    #     self.assertEqual(f.x, 200 + 250)
    #     self.assertEqual(f.y, 100 + 140)

    #     c = cModule.Cuboid()
    #     c.z = 20
    #     c.r = r1
    #     self.assertEqual(c.r.x, 200)
    #     self.assertEqual(c.r.y, 100)
    #     self.assertEqual(c.z, 20)

    #     c = cModule.Cuboid_p()
    #     c_int_ = c_int(20)
    #     c.z = c_int_
    #     c.r = r1
    #     self.assertEqual(c.z.value(), 20)
    #     self.assertEqual(c.r.x, 200)
    #     self.assertEqual(c.r.y, 100)

    #     num = cModule.Number()
    #     num.d = 3.1415
    #     snum = cModule.sNumber()
    #     snum.x = 10
    #     snum.y = 20
    #     snum.num = num
    #     self.assertAlmostEqual(snum.num.d, 3.1415, 4)
    #     self.assertEqual(snum.x, 10)
    #     self.assertEqual(snum.y, 20)

    #     snump = cModule.sNumber_ptr()
    #     n1 = c_int(10)
    #     n2 = c_int(20)
    #     snump.x = n1
    #     snump.y = n2
    #     snump.num = num
    #     self.assertAlmostEqual(snump.num.d, 3.1415, 4)
    #     self.assertEqual(snump.x.value(), 10)
    #     self.assertEqual(snump.y.value(), 20)

    # def test_functions_with_unions(self):
    #     num = cModule.Number()
    #     num.c = 10
    #     self.assertEqual(num.c, 10)
    #     num.i = 200
    #     self.assertEqual(num.i, 200)
    #     num.l = 3000
    #     self.assertEqual(num.l, 3000)
    #     num.f = 3.14
    #     self.assertAlmostEqual(num.f, 3.14, 4)

    #     num.d = cModule.pi(50000)
    #     self.assertAlmostEqual(num.d, 3.1415, 3)

    #     unionOfStructs_ = cModule.unionOfStructs()
    #     r1 = cModule.RECT()
    #     r1.x = 920
    #     r1.y = 640
    #     unionOfStructs_.r = r1
    #     self.assertEqual(unionOfStructs_.r.x, 920)
    #     self.assertEqual(unionOfStructs_.r.y, 640)
    #     unionOfStructs_.x = 300
    #     self.assertEqual(unionOfStructs_.x, 300)

    #     unionOfUnions_ = cModule.unionOfUnions()
    #     num.l = 5600
    #     unionOfUnions_.n = num
    #     self.assertEqual(unionOfUnions_.n.l, 5600)
    #     unionOfUnions_.uos = unionOfStructs_
    #     self.assertEqual(unionOfUnions_.uos.x, 300)

    #     unionOfPtr_ = cModule.unionOfPtr()
    #     unionOfPtr_.n = num
    #     self.assertEqual(unionOfPtr_.n.l, 5600)
    #     unionOfPtr_.r = r1
    #     self.assertEqual(unionOfPtr_.r.x, 920)
    #     self.assertEqual(unionOfPtr_.r.y, 640)
    #     n = c_int(12)
    #     unionOfPtr_.x = n
    #     self.assertEqual(unionOfPtr_.x.value(), 12)

    #     num.l = 3600
    #     self.assertEqual(cModule.get_number(num), 3600)
    #     self.assertEqual(cModule.get_number(num), 3600)

    #     num1 = cModule.creat_number_with_int(4800)
    #     self.assertEqual(num1.i, 4800)

    #     num2 = cModule.creat_number_ptr_with_int(4800)
    #     num2.free_on_no_reference = True
    #     self.assertEqual(num2.i, 4800)


class TestFunctions_CPP(unittest.TestCase):
    # TODO: update to use randomly generated numbers

    def test_simple_functions(self):
        self.assertEqual(cppModule.product_int(2, 4), 8)
        # self.assertEqual(cppModule.add_long_long(23, 40), 63) # TODO: support long long type
        self.assertEqual(cppModule.add_short(2, 4), 6)
        self.assertEqual(cppModule.increment_1(12), 13)
        self.assertEqual(cppModule.add(24, 46), 70)

        string = cppModule.add("Vipul", 2)
        # string = cppModule.add(c_char("Vipul"), 2) # FIXME: match_ffi_type_to_defination things it is add(int, int) because PyNumber_Check is true on c_char
        string.free_on_no_reference = True
        self.assertEqual(str(string), "VipulVipul")

        self.assertAlmostEqual(cppModule.pi(1_000_000), 3.14159, 4)

        string = cppModule.concat("Vipul ", "Cariappa")
        string.free_on_no_reference = True
        self.assertEqual(str(string), "Vipul Cariappa")

    @unittest.skip("Not Implemented")
    def test_functions_with_pointers(self):
        ...

    @unittest.skip("Not Implemented")
    def test_functions_with_arrays(self):
        ...

    @unittest.skip("Not Implemented")
    def test_functions_with_structs(self):
        ...

    @unittest.skip("Not Implemented")
    def test_functions_with_unions(self):
        ...
