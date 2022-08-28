workspace "PyC"
    configurations { "Debug", "TestCoverage", "Release", "MemoryTest" }

project "PyC"
    kind "SharedLib"
    language "C"
    targetdir "bin/%{cfg.buildcfg}"
    files { "src/**.h", "src/**.c", "src/**.hpp", "src/**.cpp" }
    excludes { "test/**" }

    
    filter "configurations:Debug"
        includedirs { "/usr/include/python3.8", "/usr/include", "/usr/lib/llvm-15/include/", "/usr/include/qlibc/" }
        libdirs { "/usr/lib", "/usr/lib/llvm-15/lib/" }
        links { "python3.8", "ffi", "clang-15", "qlibc" }
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:TestCoverage"
        includedirs { "/usr/include/python3.8", "/usr/include", "/usr/lib/llvm-15/include/", "/usr/include/qlibc/" }
        libdirs { "/usr/lib", "/usr/lib/llvm-15/lib/" }
        links { "python3.8", "ffi", "clang-15", "qlibc" }
        buildoptions { "-ftest-coverage", "-fprofile-arcs" }
        linkoptions { "-lgcov" }
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        includedirs { "/usr/include/python3.8", "/usr/include", "/usr/lib/llvm-15/include/", "/usr/include/qlibc/" }
        libdirs { "/usr/lib", "/usr/lib/llvm-15/lib/" }
        links { "python3.8", "ffi", "clang-15", "qlibc" }
        defines { "NDEBUG" }
        optimize "On"

        filter "configurations:MemoryTest"
            includedirs { "/usr/local/include/python3.9", "/usr/include", "/usr/lib/llvm-15/include/", "/usr/include/qlibc/" }
            libdirs { "/usr/lib", "/usr/lib/llvm-15/lib/" }
            links { "python3.9d", "ffi", "clang-15" }
            buildoptions { "-fsanitize=address" }
            defines { "DEBUG", "Py_DEBUG" }
            symbols "On"
