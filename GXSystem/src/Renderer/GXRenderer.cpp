#include "GXRenderer.h"
#include "Config/Config.h"

#ifdef GX_RENDERER_API_VULKAN
#include "Vulkan/GXVulkanRenderer.h"
#endif

namespace gx {

    GXBool GXRenderer::Init(GXRendererAPI Api)
    {
        api_ = Api;

        switch (Api)
        {
            case GXRendererAPI::Software:
            {
            } break;

#ifdef GX_RENDERER_API_VULKAN
            case GXRendererAPI::Vulkan:
            {
                return VulkanInit(&api_functions_);
            } break;
#endif
        }

        return false;
    }

    void GXRenderer::Shutdown()
    {
        switch (api_)
        {
            case GXRendererAPI::Software:
            {
            } break;

#ifdef GX_RENDERER_API_VULKAN
            case GXRendererAPI::Vulkan:
            {
                VulkanShutdown();
            } break;
#endif
        }
    }
}