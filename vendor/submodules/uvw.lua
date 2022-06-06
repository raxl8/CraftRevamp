return {
    include = function()
        includedirs "src"
        submodules "libuv"
    end,
    project = function()
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"

        defines "UVW_AS_LIB"
        files { "src/**.h", "src/**.cpp" }

        if os.istarget("windows") then
            links "ws2_32"
        end
        
        -- TODO: linux building
    end
}
