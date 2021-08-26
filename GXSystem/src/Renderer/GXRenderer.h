#ifndef GX_RENDERER_H_
#define GX_RENDERER_H_

#include "GXRendererTypes.h"

namespace gx {

    class GXRenderer {
    public:
        GXBool Init(GXRendererAPI Api);
        void Shutdown();

        const GXRendererAPI& GetAPI() { return api_; }

        inline bool BeginFrame()
        {
            return api_functions_.begin_frame();
        }

        inline void EndFrame()
        {
            api_functions_.end_frame();
        }

        inline void OnResize(GXuint32 Width, GXuint32 Height)
        {
            api_functions_.on_resize(Width, Height);
        }

    private:
        GXRendererAPI api_;
        GXRendererAPIFunctions api_functions_;
    };
}

#endif // GX_RENDERER_H_