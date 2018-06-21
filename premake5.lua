workspace "gtadebug"
	configurations { "Release", "Debug" }
	location "build"
	files { "src/*.*" }
	includedirs { "src" }

project "iii_debug"
	kind "SharedLib"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".dll"
	characterset ("MBCS")
	includedirs { os.getenv("RWSDK33") }
	includedirs { "src_iii" }
	files { "src_iii/*.*" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		debugdir "C:/Users/aap/games/gta3"
		debugcommand "C:/Users/aap/games/gta3/gta3.exe"
		postbuildcommands "copy /y \"$(TargetPath)\" \"C:\\Users\\aap\\games\\gta3\\plugins\\iii_debug.dll\""

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		flags { "StaticRuntime" }
		debugdir "C:/Users/aap/games/gta3"
		debugcommand "C:/Users/aap/games/gta3/gta3.exe"
		postbuildcommands "copy /y \"$(TargetPath)\" \"C:\\Users\\aap\\games\\gta3\\plugins\\iii_debug.dll\""

project "vc_debug"
	kind "SharedLib"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".dll"
	characterset ("MBCS")
	includedirs { os.getenv("RWSDK34") }
	includedirs { "src_vc" }
	files { "src_vc/*.*" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		debugdir "C:/Users/aap/games/gtavc"
		debugcommand "C:/Users/aap/games/gtavc/gta_vc.exe"
		postbuildcommands "copy /y \"$(TargetPath)\" \"C:\\Users\\aap\\games\\gtavc\\plugins\\vc_debug.dll\""

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		flags { "StaticRuntime" }
		debugdir "C:/Users/aap/games/gtavc"
		debugcommand "C:/Users/aap/games/gtavc/gta_vc.exe"
		postbuildcommands "copy /y \"$(TargetPath)\" \"C:\\Users\\aap\\games\\gtavc\\plugins\\vc_debug.dll\""

project "sa_debug"
	kind "SharedLib"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".dll"
	characterset ("MBCS")
	includedirs { os.getenv("RWSDK36") }
	includedirs { "src_sa" }
	files { "src_sa/*.*" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		debugdir "C:/Users/aap/games/gtasa"
		debugcommand "C:/Users/aap/games/gtasa/gta_sa.exe"
		postbuildcommands "copy /y \"$(TargetPath)\" \"C:\\Users\\aap\\games\\gtasa\\plugins\\sa_debug.dll\""

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		flags { "StaticRuntime" }
		debugdir "C:/Users/aap/games/gtasa"
		debugcommand "C:/Users/aap/games/gtasa/gta_sa.exe"
		postbuildcommands "copy /y \"$(TargetPath)\" \"C:\\Users\\aap\\games\\gtasa\\plugins\\sa_debug.dll\""


project "3d_debug"
	kind "SharedLib"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".dll"
	characterset ("MBCS")
	includedirs { os.getenv("RWSDK33") }
	includedirs { "src_iii" }
	files { "src_iii/*.*" }
	defines { "GTA3D" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		debugdir "C:/Users/aap/games/\gta3d_latest"
		debugcommand "C:/Users/aap/games/\gta3d_latest/gta3.exe"
		postbuildcommands "copy /y \"$(TargetPath)\" \"C:\\Users\\aap\\games\\\gta3d_latest\\plugins\\iii_debug.dll\""

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		flags { "StaticRuntime" }
		debugdir "C:/Users/aap/games/\gta3d_latest"
		debugcommand "C:/Users/aap/games/\gta3d_latest/gta3.exe"
		postbuildcommands "copy /y \"$(TargetPath)\" \"C:\\Users\\aap\\games\\\gta3d_latest\\plugins\\iii_debug.dll\""
