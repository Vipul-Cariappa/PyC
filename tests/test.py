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
        self.assertEqual(cModule.add(24, 46), 70)
        self.assertAlmostEqual(cModule.pi(1_000_000), 3.14159, 4)
        self.assertEqual(
            cModule.concat(
                "Vipul ", "Cariappa"
            ),
            "Vipul Cariappa"
        )

        # cpp
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
        n1 = PyC.c_int(1)
        n2 = PyC.c_int(10)
        cModule.copy_int(n1, n2)
        self.assertEqual(n1.value(), 10)
        self.assertEqual(n2.value(), 10)

        d1 = PyC.c_double(20.20)
        d2 = PyC.c_double(3.14)
        cModule.copy_double(d1, d2)
        self.assertAlmostEqual(d1.value(), 3.14, 4)
        self.assertAlmostEqual(d2.value(), 3.14, 4)

    def test_function_with_c_types(self):
        self.assertEqual(cModule.add(PyC.c_int(24), PyC.c_int(46)), 70)
        self.assertAlmostEqual(cModule.copy_double(
            PyC.c_double(3.14), PyC.c_double(3.14)), 3.14, 4)

    # def test_structs(self):
    #     r = cModule.RECT
    #     print(r)

        # print(r.x)
        # print(r.y)

        # r.x = 99
        # r.y = 100

        # print(r.x)
        # print(r.y)


if __name__ == "__main__":
    unittest.main()
