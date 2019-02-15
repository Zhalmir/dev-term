workspace "dev-term"
	objdir "bin-int"
	architecture "x64"
	configurations { "debug", "release" }

project "dev-term"
	kind "ConsoleApp"
	language "C"

	files { "src/**.c" }

	filter "configurations:debug"
		symbols "On"
		targetdir ("bin/" .. os.host() .. "/debug")

	filter "configurations:release"
		optimize "On"
		targetdir ("bin/" .. os.host() .. "/release")