#ifndef GX_RENDERER_TYPES_H_
#define GX_RENDERER_TYPES_H_

#include "GX/GXTypes.h"

enum class GXRendererAPI : GXuint8 {
    Software,
    Vulkan
};

struct GXRendererAPIFunctions {
    GXBool (*begin_frame)();
    void (*end_frame)();
    void (*on_resize)(GXuint32 Width, GXuint32 Height);
};

#endif // GX_RENDERER_TYPES_H_