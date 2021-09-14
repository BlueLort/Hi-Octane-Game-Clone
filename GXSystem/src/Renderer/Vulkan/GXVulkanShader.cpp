#include "GXVulkanShader.h"
#include "Logging/Logger.h"

#include <cstdio>

namespace gx {

    bool VulkanLoadShaderModule(GXVulkanContext *Context, const std::string& FilePath, VkShaderModule *OutShaderModule)
    {
        FILE *shaderFile = fopen(FilePath.c_str(), "rb");

        if (!shaderFile)
        {
            GXE_ERROR("failed to open shader module file: {}", FilePath);
            return false;
        }

        fseek(shaderFile, 0, SEEK_END);
        GXuint32 size = ftell(shaderFile);
        fseek(shaderFile, 0, SEEK_SET);

        // TODO(Harlequin): using new for now
        GXuint8* buffer = new GXuint8[size];
        GXuint32 elementsReadCount = fread(buffer, size, 1, shaderFile);

        if (elementsReadCount != 1)
        {
            GXE_ERROR("failed to read shader module file: {}", FilePath);
            return false;
        }

        VkShaderModuleCreateInfo shaderModuleCreateInfo = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
        shaderModuleCreateInfo.codeSize = size;
        shaderModuleCreateInfo.pCode = (GXuint32*)buffer;

        VkShaderModule shaderModule;

        if (vkCreateShaderModule(Context->device.logical, &shaderModuleCreateInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            OutShaderModule = nullptr;
            return false;
        }

        *OutShaderModule = shaderModule;

        GXE_INFO("loading shader module: {}", FilePath);

        return true;
    }

    void VulkanDestroyShaderModule(GXVulkanContext *Context, VkShaderModule *ShaderModule)
    {
        vkDestroyShaderModule(Context->device.logical, *ShaderModule, nullptr);
    }
}