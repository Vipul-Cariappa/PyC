import unittest
from PyC import LoadCpp

cppModule = LoadCpp("tests/libcppmodule.so", "tests/c/module.hpp", cpp=True)
cModule = LoadCpp("tests/libcmodule.so", "tests/c/module.h")


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
        self.assertEqual(cppModule.add(24, 46), 70)
        self.assertEqual(cppModule.add("Vipul", 2), "VipulVipul")
        self.assertAlmostEqual(cppModule.pi(1_000_000), 3.14159, 4)
        self.assertEqual(
            cppModule.concat(
                "Vipul ", "Cariappa"
            ),
            "Vipul Cariappa"
        )

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
