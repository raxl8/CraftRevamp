return {
    include = function()
        defines "USING_UV_SHARED"
        includedirs "include"
    end,
    project = function()
        kind "SharedLib"
        language "C"

        removedefines "USING_UV_SHARED"
        defines "BUILDING_UV_SHARED"

        includedirs "src"
        files { "src/*.h", "src/*.c" }

        if os.istarget("windows") then
            files "src/win/*"
            
            links { "iphlpapi", "psapi", "ws2_32", "userenv" }
        end

        -- TODO: linux building
    end
}
