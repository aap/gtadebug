workspace "iii_debug"
	configurations { "Release", "Debug" }
	location "build"
   
	files { "src/*.*" }
   

      
project "iii_debug"
	kind "SharedLib"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".dll"
	characterset ("MBCS")
	includedirs { os.getenv("RWSDK33") }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		debugdir "C:/Users/aap/games/gta3"
		debugcommand "C:/Users/aap/games/gta3/gta3.exe"
		postbuildcommands "copy /y \"$(TargetPath)\" \"C:\\Users\\aap\\games\\gta3\\dlls\\iii_debug.dll\""

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		flags { "StaticRuntime" }
		debugdir "C:/Users/aap/games/gta3"
		debugcommand "C:/Users/aap/games/gta3/gta3.exe"
		postbuildcommands "copy /y \"$(TargetPath)\" \"C:\\Users\\aap\\games\\gta3\\dlls\\iii_debug.dll\""
