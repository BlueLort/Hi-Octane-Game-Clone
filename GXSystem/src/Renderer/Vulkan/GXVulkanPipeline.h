#ifndef GX_VULKAN_PIPELINE_H_
#define GX_VULKAN_PIPELINE_H_

#include "GXVulkanTypes.h"

namespace gx {

    struct GXPipelineCreateInfo
    {
        VkShaderModule* vertex_shader;
        VkShaderModule* fragment_shader;

        std::vector<VkVertexInputBindingDescription> vertex_input_bindings;
        std::vector<VkVertexInputAttributeDescription> vertex_input_attributes;
    };

    bool VulkanCreatePipeline(GXVulkanContext *Context, const GXPipelineCreateInfo *CreateInfo, GXVulkanPipeline *OutPipeline);
    void VulkanDestroyPipeline(GXVulkanContext* Context, GXVulkanPipeline *pipeline);
}

#endif // GX_VULKAN_PIPELINE_H_