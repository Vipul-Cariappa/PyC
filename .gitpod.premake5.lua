workspace "PyC"
    configurations { "Debug", "TestCoverage", "Release" }

project "PyC"
    kind "SharedLib"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    files { "src/**.h", "src/**.c", "src/**.hpp", "src/**.cpp" }
    excludes { "test/**" }

    includedirs { "/usr/include/python3.8", "/usr/include", "/usr/lib/llvm-15/include/", "/usr/include/qlibc/" }
    libdirs { "/usr/lib", "/usr/lib/llvm-15/lib/" }
    links { "python3.8", "ffi", "clang-15", "qlibc" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:TestCoverage"
        buildoptions { "-ftest-coverage", "-fprofile-arcs" }
        linkoptions { "-lgcov" }
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
