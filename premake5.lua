workspace "CraftRevamp"
    configurations { "Debug", "Release" }
    platforms "x64"

project "CraftRevamp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}"

    files { "src/**.h", "src/**.cpp" }

    filter "configurations:Debug"
        defines "_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "NDEBUG"
        optimize "On"
