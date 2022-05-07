workspace "PyC"
    configurations { "Debug", "Release" }

project "PyC"
    kind "SharedLib"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    files { "src/**.h", "src/**.c", "src/**.hpp", "src/**.cpp" }
    excludes { "test/**" }

    includedirs { "/usr/include/python3.9", "/usr/local/include", "/usr/lib/llvm-11/include/" }
    libdirs { "/usr/local/lib", "usr/lib/x86_64-linux-gnu/" }
    links { "python3.9", "ffi", "clang-11" }

    filter "configurations:Debug"
        buildoptions { "-ftest-coverage", "-fprofile-arcs" }
        linkoptions { "-lgcov" }
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

