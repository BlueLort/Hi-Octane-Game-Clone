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
libDir["BulletDebug"] = "GXSystem/Deps/lib/Bullet/x64/Debug"
libDir["AssimpDebug"] = "GXSystem/Deps/lib/Assimp/x64/Debug"
libDir["BulletRelease"] = "GXSystem/Deps/lib/Bullet/x64/Release"
libDir["AssimpRelease"] = "GXSystem/Deps/lib/Assimp/x64/Release"

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
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.cc"
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
        "SDL2main",
        "assimp-vc140-mt",
        "BulletCollision",
        "BulletDynamics",
        "LinearMath"

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
        "%{libDir.SDL_Windows}",
        "%{libDir.BulletDebug}",
        "%{libDir.AssimpDebug}"
    }
    runtime "Debug"
    libdirs
    {
        "%{libDir.SDL_Windows}",
        "%{libDir.BulletRelease}",
        "%{libDir.AssimpRelease}"
    }
    runtime "Release"
    


filter "system:linux"

    libdirs
    {
        "%{libDir.SDL_Linux}",
        "%{libDir.Bullet}",
        "%{libDir.Assimp}"
    }
