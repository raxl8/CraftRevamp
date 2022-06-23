return {
    include = function()
        includedirs { "lib/includes", "../../include" }
		defines { 'ssize_t=__int64', '_SSIZE_T_DEFINED=1', '_U_=', 'NGHTTP2_STATICLIB' }
    end,
    project = function()
        kind "StaticLib"
        language "C"
 
        files "lib/*.c"
    end
}
