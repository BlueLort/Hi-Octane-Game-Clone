#ifndef GX_VULKAN_RENDERER_H_
#define GX_VULKAN_RENDERER_H_

#include "Renderer/GXRendererTypes.h"

namespace gx {

    bool VulkanInit(GXRendererAPIFunctions *ApiFunctions);
    void VulkanShutdown();
}

#endif // GX_VULKAN_RENDERER_H_