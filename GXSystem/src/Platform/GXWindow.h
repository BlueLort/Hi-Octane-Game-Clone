#ifndef GX_WINDOW_H_
#define GX_WINDOW_H_

#include "Config/Config.h"
#include <SDL/SDL.h>

namespace gx {
	struct GXWindowData {
		GXuint32 width;
		GXuint32 height;
		GXuint32 flags;
		std::string title;
		GXuint32 id;
		GXWindowData(GXuint32 Width, GXuint32 Height, GXuint32 Flags, const std::string& Title,GXuint32 ID) :width(Width), height(Height), flags(Flags), title(Title), id(ID) {
		}
	};
	class GXWindow {
	public:
		GXWindow() = delete;

		static void Init(const std::string& Title, GXuint32 Width, GXuint32 Height, GXuint32 Flags);
		inline static void AddEventsCallBack(SDL_EventFilter filter, void* userdata) { SDL_AddEventWatch(filter, userdata); }
		static void Destroy();
		inline static SDL_Window* GetSDLWindow() { return window_; }

		static std::shared_ptr<GXWindowData> window_data_;
	private:
		static SDL_Window* window_;
	};


}

#endif