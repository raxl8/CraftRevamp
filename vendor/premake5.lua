local submodules_list = {}
for _, path in pairs(os.matchfiles("submodules/*.lua")) do
    local submodule = dofile(path)
    submodule.name = string.sub(path, string.find(path, "/[^/]*$") + 1, string.find(path, ".[^.]*$") - 1) -- extract filename
    print("Found submodule: " .. submodule.name)
    table.insert(submodules_list, submodule)
end

local vendor_cwd = os.getcwd()

function submodules(list)
    if type(list) ~= "table" then
        list = { list }
    end

    for _, submodule in pairs(submodules_list) do
        for _, submodule_name in pairs(list) do
            if submodule.name == submodule_name then
                local old_cwd = os.getcwd()
                os.chdir(vendor_cwd .. "/submodules/" .. submodule.name)

                submodule.include()
                links(submodule.name)

                os.chdir(old_cwd)
                break
            end
        end
    end
end

os.chdir("submodules") -- tidy up a bit generated files

group "vendor"

for _, submodule in pairs(submodules_list) do
    project(submodule.name)

    local old_cwd = os.getcwd()
    os.chdir(submodule.name)
    
    submodule.include()
    submodule.project()

    os.chdir(old_cwd)
end

group ""

os.chdir(vendor_cwd)
