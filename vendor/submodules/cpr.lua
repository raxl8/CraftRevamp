return {
    include = function()
        includedirs { "include", "../../include" }
    end,
    project = function()
        kind "StaticLib"
        language "C++"

        files "cpr/*.cpp"
		
        submodules "curl"
    end
}
