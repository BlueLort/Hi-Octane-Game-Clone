#pragma once

#ifdef GX_ENABLE_ASSERTS
#define GX_ASSERT(x, ...) { if(!(x)) { GX_FERROR("Assertion Failed: {0}\n{1}", __VA_ARGS__); __debugbreak(); } }
#define GXE_ASSERT(x, ...) { if(!(x)) { GXE_FERROR("Assertion Failed: {0}\n{1}", __VA_ARGS__); __debugbreak(); } }
#else
#define GX_ASSERT(x, ...)
#define GXE_ASSERT(x, ...)
#endif

#define USING_GLM_MATHS

//TODO ADD THIS TO USE VULKAN API 
//#define USING_VULKAN_API