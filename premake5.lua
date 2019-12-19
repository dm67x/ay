local library = require 'premake_modules/library'

workspace "Ay"
    configurations { "Debug", "Release" }
    location "build"
    architecture "x64"

project "Ay"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

    sysincludedirs { "code" }
    files { "code/**.hpp", "code/**.cpp" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter {}

    library:include()
    library:link()

    filter { "system:windows" }
		links { "OpenGL32" }
		
	filter { "system:not windows" }
		links { "GL" }