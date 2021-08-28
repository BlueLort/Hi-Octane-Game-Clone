#ifndef GX_VULKAN_RENDERER_H_
#define GX_VULKAN_RENDERER_H_

#include "Renderer/GXRendererTypes.h"

#define GX_VULKAN_DEBUG 1

namespace gx {

    bool VulkanInit(GXRendererAPIFunctions *ApiFunctions);
    void VulkanShutdown();
}

#endif // GX_VULKAN_RENDERER_H_