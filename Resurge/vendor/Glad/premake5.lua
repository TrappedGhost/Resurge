project "Glad"
	kind "StaticLib"
	language "C"
	staticruntime "off"
	warnings "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	includedirs
	{
		"include"
	}
	
	filter "system:windows"
		systemversion "latest"
		

	filter { "system:windows", "configurations:Debug-AS" }	
		runtime "Debug"
		symbols "on"
		sanitize { "Address" }
		flags { "NoRuntimeChecks", "NoIncrementalLink" }

