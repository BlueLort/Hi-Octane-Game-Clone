#ifndef GX_RENDERER_H_
#define GX_RENDERER_H_

#include "GXRendererTypes.h"

namespace gx {

    class GXRenderer {
    public:
        GXRenderer() = delete;

        static GXBool Init(GXRendererAPI Api);
        static void Shutdown();

        static const GXRendererAPI& GetAPI() { return api_; }

        inline static void BeginFrame()
        {
            api_functions_.begin_frame();
        }

        inline static void EndFrame()
        {
            api_functions_.end_frame();
        }

        inline static void OnResize(GXuint32 Width, GXuint32 Height)
        {
            api_functions_.on_resize(Width, Height);
        }

    private:
        static GXRendererAPI api_;
        static GXRendererAPIFunctions api_functions_;
    };
}

#endif // GX_RENDERER_H_