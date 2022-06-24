workspace "CraftRevamp"
    configurations { "Debug", "Release" }
    platforms "x64"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}/%{prj.name}"

    filter "configurations:Debug"
        defines "_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "NDEBUG"
        optimize "On"

include "vendor"

project "CraftRevamp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    includedirs "src" -- fix intellisense not finding pch.h
    files { "src/**.h", "src/**.cpp" }

    submodules { "uvw", "fmt", "mbedtls", "curl", "cpr", "json" }
