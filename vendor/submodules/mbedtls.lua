return {
    include = function()
        includedirs "include"
    end,
    project = function()
        kind "StaticLib"
        language "C"

        includedirs "library"
        files { "library/*.h", "library/*.c" }

        if os.istarget("windows") then
            defines "WINDOWS_BUILD=1"
        end

        -- TODO: linux building
    end
}
