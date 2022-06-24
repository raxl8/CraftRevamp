return {
    include = function()
        includedirs "."
    end,
    project = function()
        kind "SharedLib"
        language "C"

        files "*.c"

        if os.istarget('windows') then
            files "win32/zlib.def"
        end
    end
}
