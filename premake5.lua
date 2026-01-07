workspace " Resurge"
    architecture "x64"
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }
    -- 添加全局编译选项
    flags { "MultiProcessorCompile" }
    -- 设置字符集为 UTF-8
    characterset "Unicode"
    -- 添加 /utf-8 编译选项
    buildoptions { "/utf-8" }


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--include glfw
IncludeDir = {}
IncludeDir["GLFW"] = "Resurge/vendor/GLFW/include"
IncludeDir["Glad"] = "Resurge/vendor/Glad/include"

include "Resurge/vendor/GLFW"
include "Resurge/vendor/Glad"

--resurge
project "Resurge"
    location "Resurge"
    kind "SharedLib"
    language "C++"

    targetdir("bin/"..outputdir.."/%{prj.name}")
    objdir("bin-int/"..outputdir.."/%{prj.name}")

    pchheader "rgpch.h"
    pchsource "Resurge/src/rgpch.cpp"

    files
    { 
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}"        
    }

    links
    {
        "GLFW",
        "Glad",
        "opengl32.lib"
    }

    filter"system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "RG_PLATFORM_WINDOWS",
            "RG_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{MKDIR} ../bin/"..outputdir.."/Sandbox"),
            ("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/"..outputdir.."/Sandbox")
        }
    filter "configurations:Debug"
        defines "RG_DEBUG"
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "RG_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "RG_Dist"
        buildoptions "/MD"
        optimize "On"

--sandbos
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir("bin/"..outputdir.."/%{prj.name}")
    objdir("bin-int/"..outputdir.."/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Resurge/vendor/spdlog/include",
        "Resurge/src"
    }

    links
    {
        "Resurge"
    }
    filter"system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "RG_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "RG_DEBUG"
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "RG_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "RG_Dist"
        buildoptions "/MD"
        optimize "On"