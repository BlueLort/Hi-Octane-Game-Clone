workspace "Octane"

architecture "x64"
systemversion "latest"
startproject "GXSystem"

outputDir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

includeDir = {}
includeDir["Deps"]  = "GXSystem/Deps/include"
includeDir["thirdparty"] = "GXSystem/Third-Party"

libDir = {}
libDir["SDL_Windows"] = "GXSystem/Deps/lib/SDL2-2.0.16/Windows/x64"
libDir["SDL_Linux"] = ""

configurations
{
    "Debug",
    "Release"
}

flags
{
    "MultiProcessorCompile"
}

filter "system:windows"

    defines
    {
        "GX_PLATFORM_WINDOWS"
    }

filter "system:linux"

    defines
    {
        "GX_PLATFORM_LINUX"
    }

filter "system:macos"

    defines
    {
        "GX_PLATFORM_MACOS"
    }

filter "configurations:Debug"

    defines
    {
        "GX_CONFIG_DEBUG"
    }

    runtime "Debug"
    symbols "on"

filter "configurations:Release"

    defines
    {
        "GX_CONFIG_RELEASE"
    }

    runtime "Release"
    optimize "on"

project "GXSystem"

    location "GXSystem"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{includeDir.Deps}",
        "%{includeDir.thirdparty}"
    }

    links
    {
        "SDL2",
        "SDL2main"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

filter "system:windows"

    libdirs
    {
        "%{libDir.SDL_Windows}"
    }

filter "system:linux"

    libdirs
    {
        "%{libDir.SDL_Linux}"
    }
