#ifndef GX_VULKAN_SHADER_H_
#define GX_VULKAN_SHADER_H_

#include "GXVulkanTypes.h"

#include <string>

namespace gx {
    bool VulkanLoadShaderModule(GXVulkanContext *Context, const std::string& FilePath, VkShaderModule *OutShaderModule);
    void VulkanDestroyShaderModule(GXVulkanContext *Context, VkShaderModule *ShaderModule);
}

#endif