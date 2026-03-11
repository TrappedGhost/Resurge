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
IncludeDir["ImGui"] = "Resurge/vendor/imgui"
IncludeDir["glm"] = "Resurge/vendor/glm"
IncludeDir["stb_image"] = "Resurge/vendor/stb_image"
IncludeDir["entt"] = "Resurge/vendor/entt/include"
IncludeDir["yaml_cpp"] = "Resurge/vendor/yaml-cpp/include"


include "Resurge/vendor/GLFW"
include "Resurge/vendor/Glad"
include "Resurge/vendor/imgui"
include "Resurge/vendor/yaml-cpp"


--resurge
project "Resurge"
    location "Resurge"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    staticruntime "on"

    targetdir("bin/"..outputdir.."/%{prj.name}")
    objdir("bin-int/"..outputdir.."/%{prj.name}")

    pchheader "rgpch.h"
    pchsource "Resurge/src/rgpch.cpp"

    files
    { 
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "yaml-cpp",
        "opengl32.lib"
    }


    filter"system:windows"
        systemversion "latest"

        defines
        {
            "RG_PLATFORM_WINDOWS",
            "RG_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
            "YAML_CPP_STATIC_DEFINE"
        }

    filter "configurations:Debug"
        defines "RG_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "RG_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "RG_Dist"
        runtime "Release"
        optimize "On"

--sandbos
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "on"


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
        "Resurge/src",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}"
    }

    links
    {
        "Resurge"
    }
    filter"system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "RG_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "RG_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "RG_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "RG_Dist"
        runtime "Release"
        optimize "On"


--Resurge-Editor
project "Resurge-Editor"
    location "Resurge-Editor"
    kind "ConsoleApp"
    language "C++"
    staticruntime "on"


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
        "Resurge/src",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}"

    }

    links
    {
        "Resurge"
    }
    filter"system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "RG_PLATFORM_WINDOWS",
            "YAML_CPP_STATIC_DEFINE"
        }

    filter "configurations:Debug"
        defines "RG_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "RG_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "RG_Dist"
        runtime "Release"
        optimize "On"