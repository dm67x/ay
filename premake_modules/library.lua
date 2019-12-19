json = require 'json'
require 'os'
require 'io'

local Library = {}
Library.__index = Library

local jsons = {
    "sdl2.json",
    "glad.json",
    "glm.json",
    "stb.json"
}

local contents = {}

print("Number of libraries loaded: " .. #jsons)

function loading(text)
    local result = text
    local size = #result
    for i = size, 100 do
        result = result .. "."
    end
    return result
end

-- read json files
for index, value in ipairs(jsons) do
    local filename = value
    local content = ""

    print(loading("Loading " .. filename))
    for line in io.lines(filename) do
        content = content .. line
    end

    content = json.decode(content)
    table.insert(contents, content)
end

function Library:include()
    for index, value in ipairs(contents) do
        includedirs { value["include"] }
    end
end

function Library:link()
    filter { "system:windows" }
    for index, value in ipairs(contents) do
        filter { "configurations:Debug" }
            libdirs { value["x64"]["vs"]["debug"]["bin"] }

            for i, val in ipairs(value["x64"]["vs"]["debug"]["libs"]) do
                links { val }
            end

        filter { "configurations:Release" }
            libdirs { value["x64"]["vs"]["release"]["bin"] }

            for i, val in ipairs(value["x64"]["vs"]["release"]["libs"]) do
                links { val }
            end
    end
    filter {}
end

return Library
