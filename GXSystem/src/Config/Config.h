#ifndef CONFIG_H_
#define CONFIG_H_

#define GX_ENABLE_ASSERTS 1

#if GX_ENABLE_ASSERTS

#include "Logging/Logger.h"

#if _MSC_VER

#include <intrin.h>
#define GXDebugBreak() __debugbreak()

#else

#define GXDebugBreak() __builtin_trap()

#endif

#define GX_ASSERT(x, ...) { if(!(x)) { GX_FERROR("Assertion Failed: {0}\n{1}", __VA_ARGS__); GXDebugBreak(); } }
#define GXE_ASSERT(x, ...) { if(!(x)) { GXE_FERROR("Assertion Failed: {0}\n{1}", __VA_ARGS__); GXDebugBreak(); } }

#else
#define GX_ASSERT(x, ...)
#define GXE_ASSERT(x, ...)
#endif

#define USING_GLM_MATHS

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

#define GX_PLATFORM_WINDOWS 1
#define GX_RENDERER_API_OPENGL
#define GX_RENDERER_API_VULKAN
#define GX_RENDERER_API_DIRECTX

#ifndef _WIN64
#error "64-bit is required."
#endif

#elif defined(__linux__) || defined(__gnu_linux__)

#define GX_PLATFORM_LINUX 1
#define GX_RENDERER_API_OPENGL
#define GX_RENDERER_API_VULKAN

#elif defined(__ANDROID__)

#define GX_PLATFORM_ANDROID 1

#elif defined(__unix__)

#define GX_PLATFORM_UNIX 1

#elif defined(_POSIX_VERSION)

#define GX_PLATFORM_POSIX 1

#elif __APPLE__ || defined(__MACH__)

#define GX_PLATFORM_APPLE 1
#include <TargetConditionals.h>

#if TARGET_IPHONE_SIMULATOR

#define GX_PLATFORM_IOS 1
#define GX_PLATFORM_IOS_SIMULATOR 1

#elif TARGET_OS_IPHONE

#define GX_PLATFORM_IOS 1

#elif TARGET_OS_MAC

#define GX_PLATFORM_MACOS

#endif

#else

#error "unknown platform."

#endif

#endif