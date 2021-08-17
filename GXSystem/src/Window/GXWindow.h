#pragma once
#include "Config/Config.h"
namespace gx {
	struct GXWindowData {
		GXuint32 width;
		GXuint32 height;
		GXuint32 flags;
		std::string title;
		GXuint32 id;
		GXWindowData(GXuint32 Width, GXuint32 Height, GXuint32 Flags, const std::string& Title,GXuint32 ID) :width(Width), height(Height), flags(Flags), title(Title),id(ID) {
		}
	};
	class GXWindow {

	public:
		static void Init(const std::string& Title, GXuint32 Width, GXuint32 Height, GXuint32 Flags);
		inline static void AddEventsCallBack(SDL_EventFilter filter, void* userdata) { SDL_AddEventWatch(filter, userdata); }
		static void Destroy();
		inline static SDL_Window* GetSDLWindow() { return window_; }

#ifdef USING_OPENGL
		//SDL OPEN_GL SPECIFIC
		static void SetAttr(SDL_GLattr flag, GXint32 val){ SDL_GL_SetAttribute(flag, val); }
		static void SetSwapInterval(GXint32 val){ SDL_GL_SetSwapInterval(val); }
		static void SwapWindow(){ SDL_GL_SwapWindow(window); }
#endif
		static std::shared_ptr<GXWindowData> window_data_;
		GXWindow() = delete;
	private:
		static SDL_Window* window_;
	

	};


}