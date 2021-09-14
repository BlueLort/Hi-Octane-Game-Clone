@echo off
%VULKAN_SDK%/bin/glslc -fshader-stage=vert triangle.vert.glsl -o Compiled/triangle.vert.spv
%VULKAN_SDK%/bin/glslc -fshader-stage=frag triangle.frag.glsl -o Compiled/triangle.frag.spv
pause