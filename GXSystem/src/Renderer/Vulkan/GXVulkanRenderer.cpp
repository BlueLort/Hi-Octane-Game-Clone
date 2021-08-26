#include "GXVulkanRenderer.h"
#include "GXVulkanTypes.h"
#include "Logging/Logger.h"

namespace gx {
    
    static GXVulkanContext context;
    
    static GXBool VulkanBeginFrame()
    {
        GXE_INFO("VulkanBeginFrame");
        return true;
    }

    static void VulkanEndFrame()
    {
        GXE_INFO("VulkanEndFrame");
    }

    static void VulkanOnResize(GXuint32 Width, GXuint32 Height)
    {
        GXE_INFO("VulkanOnResize {0}, {1}", Width, Height);
    }

    bool VulkanInit(GXRendererAPIFunctions *ApiFunctions)
    {
        ApiFunctions->begin_frame = &VulkanBeginFrame;
        ApiFunctions->end_frame   = &VulkanEndFrame;
        ApiFunctions->on_resize   = &VulkanOnResize;
        
        return true;
    }

    void VulkanShutdown()
    {
    }
}