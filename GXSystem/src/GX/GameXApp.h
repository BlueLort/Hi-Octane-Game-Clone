#ifndef GAME_X_APP_H_
#define GAME_X_APP_H_

#include "Config/Config.h"
#include "Platform/GXWindow.h"
#include "GXTimer.h"
#include "Events/WindowEvent.h"
#include "Input/InputManager.h"
#include "Renderer/GXRenderer.h"

#define GXPRINTF(...) (gx::LayerManager::getInstance().GXPrintf(__VA_ARGS__));

namespace gx {
	//adapt it to whatever API im using.
	using GX_SDLEvent = SDL_Event;
	using GXEventCallBack = SDL_EventFilter;
#define GXPollEvents SDL_PollEvent

	class GameXApp
	{
	public:
		GameXApp();
		virtual ~GameXApp();
		void Start();
		GXEventCallBack GetEventCallBack() const { return OnEvent; }
		//static std::unique_ptr<Renderer> renderer;
	private:
		
		//EVENT HANDLING
		static GXint32 OnEvent(void* userdata, GX_SDLEvent* Event);
		
		template<class T>
		static GXint32 DispatchSystemEvent(std::shared_ptr<T>& gxEvent,GXuint32 windowID);

		template<class T>
		inline static GXint32 HandleEvent(std::shared_ptr<T>& Event) { return 0; }
		static bool is_running_;
		static bool is_suspended_;
		

	};
	extern GameXApp* CreateApp();

	template<class T>
	inline GXint32 GameXApp::DispatchSystemEvent(std::shared_ptr<T>& gxEvent,GXuint32 windowID)
	{
		//TODO PROFILE THIS ,OVERHEAD -> Change it !
		bool handled = false;
		//input manager can only manage main window inputs
		if (InputManager::GetInstance().HandleEvent(gxEvent)) {
			handled = true;
		}
		if (HandleEvent(gxEvent)) {
			handled = true;
		}


		return handled;
	}


	template<>
	inline GXint32 GameXApp::HandleEvent<gx::event::WindowCloseEvent>(std::shared_ptr<gx::event::WindowCloseEvent>& Event) {
		is_running_ = false;
		Event->handled = true;
		return 1;
	}
	template<>
	inline GXint32 GameXApp::HandleEvent<gx::event::WindowMinimizeEvent>(std::shared_ptr<gx::event::WindowMinimizeEvent>& Event) {
		Event->handled = true;
		return 1;
	}
	template<>
	inline GXint32 GameXApp::HandleEvent<gx::event::WindowMaximizeEvent>(std::shared_ptr<gx::event::WindowMaximizeEvent>& Event) {
		Event->handled = true;
		return 1;
	}
	template<>
	inline GXint32 GameXApp::HandleEvent<gx::event::MousePressEvent>(std::shared_ptr<gx::event::MousePressEvent>& Event) {
		Event->handled = true;
		return 1;
	}
}


#endif