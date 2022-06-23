return {
    include = function()
        includedirs "include"
    end,
    project = function()
        language "C++"
		kind "StaticLib"

        files { "src/**.cc" }
        removefiles "src/fmt.cc"
    end
}
