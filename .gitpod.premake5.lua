workspace "PyC"
    configurations { "Debug", "TestCoverage", "Release", "MemoryTest" }

project "PyC"
    kind "SharedLib"
    language "C"
    targetdir "bin/%{cfg.buildcfg}"
    files { "src/**.h", "src/**.c", "src/**.hpp", "src/**.cpp" }
    excludes { "test/**" }

    
    filter "configurations:Debug"
        includedirs { "/usr/local/include/python3.11d", "/usr/include", "/usr/lib/llvm-12/include/" }
        libdirs { "/usr/lib", "/usr/lib/llvm-12/lib/" }
        links { "python3.11d", "ffi", "clang-12" }
        defines { "DEBUG" }
        symbols "On"
    
    filter "configurations:TestCoverage"
        includedirs { "/usr/local/include/python3.11d", "/usr/include", "/usr/lib/llvm-12/include/" }
        libdirs { "/usr/lib", "/usr/lib/llvm-12/lib/" }
        links { "python3.11d", "ffi", "clang-12" }
        buildoptions { "-ftest-coverage", "-fprofile-arcs" }
        linkoptions { "-lgcov" }
        defines { "DEBUG" }
        symbols "On"
    
    filter "configurations:Release"
        includedirs { "/usr/include/python3.8", "/usr/include", "/usr/lib/llvm-12/include/" }
        libdirs { "/usr/lib", "/usr/lib/llvm-12/lib/" }
        links { "python3.8", "ffi", "clang-12" }
        defines { "NDEBUG" }
        optimize "On"
    
    filter "configurations:MemoryTest"
        includedirs { "/usr/local/include/python3.11d", "/usr/include", "/usr/lib/llvm-12/include/" }
        libdirs { "/usr/lib", "/usr/lib/llvm-12/lib/", "/usr/local/lib" }
        links { "python3.11d", "ffi", "clang-12" }
        buildoptions { "-fsanitize=address" }
        defines { "DEBUG" }
        symbols "On"
