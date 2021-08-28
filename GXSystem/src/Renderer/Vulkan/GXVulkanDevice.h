#ifndef GX_VULKAN_DEVICE_H_
#define GX_VULKAN_DEVICE_H_

#include "GXVulkanTypes.h"

#include <vector>

namespace gx {

    struct GXVulkanDeviceRequirements {
        bool has_graphics_queue;
        bool has_present_queue;
        bool has_compute_queue;
        bool has_transfer_queue;
        bool has_sampler_anisotropy;
        bool is_discrete;
        std::vector<const char*> extensions;
    };

    bool VulkanCreateDevice(GXVulkanContext* Context, GXVulkanDeviceRequirements* Requirements, GXVulkanDevice *outDevice);
    void VulkanDestroyDevice(GXVulkanDevice *Device);
}

#endif // GX_VULKAN_DEVICE_H_
