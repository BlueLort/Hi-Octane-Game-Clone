#include "Config/pch.h"
#include "GXWindow.h"


namespace gx {
	SDL_Window* GXWindow::window_ = nullptr;
	std::shared_ptr<GXWindowData> GXWindow::window_data_ = nullptr;

	void GXWindow::Init(const std::string& Title, GXuint32 Width, GXuint32 Height, GXuint32 Flags)
	{
		GXint32 SDLStatus = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);//inits video and events
		GXE_ASSERT(SDLStatus == 0, "Failed to initialize SDL2.0 Video,Timer or Events ...\n", SDL_GetError());
		
		GXWindow::window_ = SDL_CreateWindow(Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, Flags);
		GXE_ASSERT(GXWindow::window_ != nullptr, "Failed to create SDL Window ...\n", SDL_GetError());

		GXWindow::window_data_ = std::make_shared<GXWindowData>(Width, Height, Flags, Title, SDL_GetWindowID(window_));
	}

	void GXWindow::Destroy()
	{
		SDL_DestroyWindow(window_);
		SDL_Quit();
	}
	
}

