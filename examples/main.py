from PyC import LoadCpp, print_CppModule

main = LoadCpp("./libmain.so", "main.h", cpp=False)
print_CppModule(main)

pi = main.pi(1000)

rect = main.Rectangle(12, 24)
print(f"rect: {rect.x = } {rect.y = }") # rect: rect.x = 12 rect.y = 24

area = main.get_area(rect)
perimeter = main.get_perimeter(rect)

print(f"{pi = }, {area = }, {perimeter = }") # pi = 3.140592653839794, area = 288, perimeter = 72
