#include "Config/pch.h"
#include "GameXApp.h"

//IGNORE EVENTS DEBUGGING DATA FOR NOW

#define DISPATCH_EVENT(ev, windowID, ...) std::shared_ptr<##ev> gxEvent = std::make_shared<##ev>(__VA_ARGS__);\
 DispatchSystemEvent<##ev>(gxEvent, windowID)

namespace gx {

	bool GameXApp::is_running_ = true;
	bool GameXApp::is_suspended_ = false;

	GameXApp::GameXApp() {

	}

	GameXApp::~GameXApp() {
	}

	void GameXApp::Start() {
		while (is_running_) {
			GXFloat deltaTime = 1.0f / GXTimer::GetAppTimer().GetDeltaTicks();
			GXTimer::GetAppTimer().Update();
			InputManager::GetInstance().Update();
			GX_SDLEvent event;
			while (GXPollEvents(&event)); //Send events to callback
			
			if (!is_suspended_)
			{
				GXRenderer::BeginFrame();
				GXRenderer::EndFrame();
			}
		}
	}

	GXint32 GameXApp::OnEvent(void* userdata, GX_SDLEvent* Event)
	{
		//SDL Event Mapping 
		bool dispatched = false;
		std::string eventName;
		switch (Event->type) {
		case SDL_QUIT:
		{
			DISPATCH_EVENT(gx::event::WindowCloseEvent, Event->window.windowID, 0);
		}
		break;
		case gx::event::GXEventClass::GX_APPLICATION:
			switch (Event->window.event) {
			case gx::event::GXEventType::GX_WINDOW_CLOSE:
			{
				DISPATCH_EVENT(gx::event::WindowCloseEvent, Event->window.windowID, Event->window.windowID); 
			}
			break;
			case gx::event::GXEventType::GX_WINDOW_RESIZE:
			{
				GXRenderer::OnResize(Event->window.data1, Event->window.data2);
				DISPATCH_EVENT(gx::event::WindowResizeEvent, Event->window.windowID, Event->window.data1, Event->window.data2, Event->window.windowID); 
			}
			break;
			case gx::event::GXEventType::GX_WINDOW_MINIMIZE:
			{
				is_suspended_ = true;
				DISPATCH_EVENT(gx::event::WindowMinimizeEvent, Event->window.windowID, Event->window.data1, Event->window.data2, Event->window.windowID);
			}
			break;
			case gx::event::GXEventType::GX_WINDOW_MAXIMIZE:
			{
				DISPATCH_EVENT(gx::event::WindowMaximizeEvent, Event->window.windowID, Event->window.data1, Event->window.data2, Event->window.windowID);
			}
			break;

			case gx::event::GXEventType::GX_WINDOW_RESTORED:
			{
				is_suspended_ = false;
				DISPATCH_EVENT(gx::event::WindowRestoredEvent, Event->window.windowID, Event->window.windowID);
			}
			break;
			}
			break;
		case gx::event::GXEventType::GX_KEY_PRESSED:
		{DISPATCH_EVENT(gx::event::KeyPressEvent, Event->key.windowID, Event->key.keysym.scancode, Event->key.windowID); }
		break;
		case gx::event::GXEventType::GX_KEY_RELEASED:
		{DISPATCH_EVENT(gx::event::KeyReleaseEvent, Event->key.windowID, Event->key.keysym.scancode, Event->key.windowID); }
		break;
		case gx::event::GXEventType::GX_KEY_TYPED:
		{DISPATCH_EVENT(gx::event::KeyTypedEvent, Event->text.windowID, Event->text.text, Event->text.windowID); }
		break;
		case gx::event::GXEventType::GX_MOUSE_MOVED:
		{DISPATCH_EVENT(gx::event::MouseMoveEvent, Event->motion.windowID, Event->motion.x, Event->motion.y, Event->motion.windowID); }
		{DISPATCH_EVENT(gx::event::MouseMoveRelEvent, Event->motion.windowID, Event->motion.xrel, Event->motion.yrel, Event->motion.windowID); }
		break;
		case gx::event::GXEventType::GX_MOUSE_PRESSED:
		{DISPATCH_EVENT(gx::event::MousePressEvent, Event->button.windowID, Event->button.button, Event->button.windowID); }
		break;
		case gx::event::GXEventType::GX_MOUSE_RELEASED:
		{DISPATCH_EVENT(gx::event::MouseReleaseEvent, Event->button.windowID, Event->button.button, Event->button.windowID); }
		break;
		case gx::event::GXEventType::GX_MOUSE_SCROLL:
		{DISPATCH_EVENT(gx::event::MouseScrollEvent, Event->wheel.windowID, Event->wheel.x, Event->wheel.y, Event->wheel.windowID); }
		break;

		}
		return dispatched;
	}

}