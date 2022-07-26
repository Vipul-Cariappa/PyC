import unittest
import PyC
from PyC import LoadCpp

cModule = LoadCpp("tests/libcmodule.so", "tests/c/module.h")
cppModule = LoadCpp("tests/libcppmodule.so", "tests/c/module.hpp", cpp=True)


class TestBasic(unittest.TestCase):
    def test_global_variables(self):
        # c
        self.assertEqual(cModule.MAX, 5_000_000)
        self.assertAlmostEqual(cModule.f, 3.14, 4)
        self.assertEqual(cModule.PROGRAM, "C/C++")

        # cpp
        self.assertEqual(cppModule.MAX, 5_000_000)
        self.assertAlmostEqual(cppModule.f, 3.14, 4)
        self.assertEqual(cppModule.PROGRAM, "C/C++")

    def test_simple_functions(self):
        # c
        self.assertEqual(cModule.product_int(2, 4), 8)
        self.assertEqual(cModule. add_long_long(23, 40), 63)
        self.assertEqual(cModule.add_short(2, 4), 6)
        self.assertEqual(cModule.increment_1(12), 13)
        self.assertEqual(cModule.add(24, 46), 70)
        self.assertTrue(cModule.invert(0))
        self.assertFalse(cModule.invert(True))
        self.assertAlmostEqual(cModule.pi(1_000_000), 3.14159, 4)
        self.assertEqual(
            cModule.concat(
                "Vipul ", "Cariappa"
            ),
            "Vipul Cariappa"
        )

        # cpp
        self.assertEqual(cppModule.product_int(2, 4), 8)
        self.assertEqual(cppModule. add_long_long(23, 40), 63)
        self.assertEqual(cppModule.add_short(2, 4), 6)
        self.assertEqual(cppModule.increment_1(12), 13)
        self.assertEqual(cppModule.add(24, 46), 70)
        self.assertEqual(cppModule.add("Vipul", 2), "VipulVipul")
        self.assertAlmostEqual(cppModule.pi(1_000_000), 3.14159, 4)
        self.assertEqual(
            cppModule.concat(
                "Vipul ", "Cariappa"
            ),
            "Vipul Cariappa"
        )

    def test_pointer_args_with_functions(self):
        # c_int
        n1 = PyC.c_int(1)
        n2 = PyC.c_int(10)
        cModule.copy_int(n1, n2)
        self.assertEqual(n1.value(), 10)
        self.assertEqual(n2.value(), 10)

        c_int_array = PyC.c_int((1, 2, 3))
        cModule.write_int(c_int_array, 100)
        c_int_array[1] = 200
        c_int_array[2] = 300

        self.assertEqual(c_int_array[0], 100)
        self.assertEqual(c_int_array[1], 200)
        self.assertEqual(c_int_array[2], 300)

        self.assertFalse(n1.is_pointer)
        self.assertFalse(n1.is_array)
        
        self.assertTrue(c_int_array.is_pointer)
        self.assertTrue(c_int_array.is_array)
        self.assertEqual(len(c_int_array), 3)

        # c_double
        d1 = PyC.c_double(20.20)
        d2 = PyC.c_double(3.14)
        cModule.copy_double(d1, d2)
        self.assertAlmostEqual(d1.value(), 3.14, 4)
        self.assertAlmostEqual(d2.value(), 3.14, 4)
        
        # c_float
        f1 = PyC.c_float(20.20)
        f2 = PyC.c_float(3.14)
        cModule.copy_double(f1, f2)
        self.assertAlmostEqual(f1.value(), 3.14, 4)
        self.assertAlmostEqual(f2.value(), 3.14, 4)

        # c_long
        l1 = PyC.c_long(1)
        l2 = PyC.c_long(4)
        cModule.copy_int(l1, l2)
        self.assertEqual(l1.value(), 4)
        self.assertEqual(l2.value(), 4)
        self.assertEqual(cModule.invert_bit(l1, 2), 0)
        
        # c_short
        s1 = PyC.c_short(1)
        s2 = PyC.c_short(10)
        cModule.copy_int(s1, s2)
        self.assertEqual(s1.value(), 10)
        self.assertEqual(s2.value(), 10)

        # c_bool
        b_true = PyC.c_bool(True)
        b_false = PyC.c_bool(0)
        self.assertTrue(bool(b_true))
        self.assertFalse(bool(b_false))
        
        # c_char
        string = "vipul "
        c_string_empty = PyC.c_char("")
        c_string_char = PyC.c_char("#")
        c_string = PyC.c_char(string)

        self.assertEqual(len(c_string_empty), 0)
        self.assertEqual(len(c_string_char), 1)
        self.assertEqual(len(c_string), len(string))

        self.assertEqual(str(c_string_empty), "")
        self.assertEqual(str(c_string_char), "#")
        self.assertEqual(str(c_string), string)

        self.assertEqual(cppModule.add(string, 2), string+string)
        self.assertEqual(cppModule.repeat_char(c_string_char, 2), "##")

    def test_pointer_return_funcs(self):
        self.assertEqual(cModule.returns_pointer().value(), 465826769270)

    def test_function_with_c_types(self):
        self.assertEqual(cModule.add(PyC.c_int(24), PyC.c_int(46)), 70)
        self.assertAlmostEqual(cModule.copy_double(
            PyC.c_double(3.14), PyC.c_double(3.14)), 3.14, 4)

    def test_structs(self):
        r = cModule.RECT
        
        r1 = r()
        r1.x = 200
        r1.y = 100
        
        r2 = r()
        r2.x = 250
        r2.y = 140

        self.assertEqual(r1.x, 200)
        self.assertEqual(r1.y, 100)
        self.assertEqual(r2.x, 250)
        self.assertEqual(r2.y, 140)

        self.assertEqual(cModule.get_area(r1), 200*100)
        self.assertEqual(cModule.get_perimeter(r1), 2*(200+100))
        
        self.assertEqual(cModule.get_area(r2), 250*140)
        self.assertEqual(cModule.get_perimeter(r2), 2*(250+140))

        self.assertFalse(cModule.same_rects(r1, r2))
        self.assertTrue(cModule.same_rects(r1, r1))
        
        t = cModule.get_rect(1, 2)
        f = cModule.rect_add(r1, r2)
        
        self.assertEqual(t.x, 1)
        self.assertEqual(t.y, 2)
        self.assertEqual(f.x, 200+250)
        self.assertEqual(f.y, 100+140)

        c = cModule.Cuboid()
        c.z = 20
        c.r = r1
        self.assertEqual(c.r.x, 200)
        self.assertEqual(c.r.y, 100)
        self.assertEqual(c.z, 20)
        
        c = cModule.Cuboid_p()
        c_int_ = PyC.c_int(20)
        c.z = c_int_
        c.r = r1
        self.assertEqual(c.z.value(), 20)
        self.assertEqual(c.r.x, 200)
        self.assertEqual(c.r.y, 100)

        num = cModule.Number()
        num.d = 3.1415
        snum = cModule.sNumber()
        snum.x = 10
        snum.y = 20
        snum.num = num
        self.assertAlmostEqual(snum.num.d, 3.1415, 4)
        self.assertEqual(snum.x, 10)
        self.assertEqual(snum.y, 20)
        
        snump = cModule.sNumber_ptr()
        n1 = PyC.c_int(10)
        n2 = PyC.c_int(20)
        snump.x = n1
        snump.y = n2
        snump.num = num
        self.assertAlmostEqual(snump.num.d, 3.1415, 4)
        self.assertEqual(snump.x.value(), 10)
        self.assertEqual(snump.y.value(), 20)

        
    def test_unions(self):
        num = cModule.Number()
        
        num.c = 10
        self.assertEqual(num.c, 10)
        
        num.i = 200
        self.assertEqual(num.i, 200)
        
        num.l = 3000
        self.assertEqual(num.l, 3000)
        
        num.f = 3.14
        self.assertAlmostEqual(num.f, 3.14, 4)
        
        num.d = cModule.pi(50000)
        self.assertAlmostEqual(num.d, 3.1415, 3)


        


if __name__ == "__main__":
    unittest.main()
