#ifndef EVENT_H_
#define EVENT_H_

#include "Config/pch.h"
#include <SDL/SDL_events.h>
#define KeyValToSTR(K) (K) //map to input code map
namespace gx {
	namespace event {
		enum GXEventType
		{
			GX_NONE = 0,
			GX_KEY_PRESSED	=	SDL_KEYDOWN,
			GX_KEY_RELEASED	=   SDL_KEYUP,
			GX_KEY_TYPED = SDL_TEXTINPUT,
			GX_MOUSE_PRESSED = SDL_MOUSEBUTTONDOWN,
			GX_MOUSE_RELEASED = SDL_MOUSEBUTTONUP,
			GX_MOUSE_MOVED = SDL_MOUSEMOTION,
			GX_MOUSE_SCROLL = SDL_MOUSEWHEEL,
			GX_WINDOW_RESIZE = SDL_WINDOWEVENT_RESIZED,
			GX_WINDOW_CLOSE = SDL_WINDOWEVENT_CLOSE,
			GX_WINDOW_MOVED = SDL_WINDOWEVENT_MOVED,
			GX_WINDOW_SIZE_CHANGED = SDL_WINDOWEVENT_SIZE_CHANGED,
			GX_WINDOW_MINIMIZE = SDL_WINDOWEVENT_MINIMIZED,
			GX_WINDOW_MAXIMIZE = SDL_WINDOWEVENT_MAXIMIZED
		};

		enum GXEventClass {
			GX_NO_CLASS = 0,
			GX_INPUT = 1 << 1,
			GX_KEYBOARD = 1 << 2,
			GX_MOUSE = 1 << 3,
			GX_MOUSE_BUTTON = 1 << 4,
			GX_APPLICATION = SDL_WINDOWEVENT
		};

		class GXEvent
		{
		public:
			GXEvent(GXuint32 WindowID) : windowID_(WindowID) {}
			bool handled = false;
			GXuint32 windowID_;
			virtual inline const char* GetName() const = 0;
			virtual inline GXuint32 GetEventType() const = 0;
			virtual inline GXuint32 GetEventClass() const = 0;
			
			inline bool IsSameClassAs(const GXEvent& e) {
				return this->GetEventClass() & e.GetEventClass();
			}

			virtual inline const char* ToString() const { return this->GetName(); }
		};

		/* EVENT QUEUE IMPLEMENTATION HOWEVER CURRENTLY I'M USING SDL QUEUE TO POLL EVENTS AND HANDLE THEM*/
		/*
		constexpr GXuint32 EVENTS_LIMIT = 64;
		class GX_DLL EventQueue {
		public:
			static bool enqueue(const Event& e) {
				if (size == EVENTS_LIMIT)return false;
				EventBuffer[backIte] = std::make_unique<Event>(e);
				backIte = (backIte + 1) % EVENTS_LIMIT;
				size++;
				return true;
			}
			static bool processEvent() {
				if (size == 0)return false;
				//TODO EVENT DISPATCHER TO DISPATCH EVENTS
				EventBuffer[frontIte]->handled = true;
				size--;
				frontIte = (frontIte + 1) % EVENTS_LIMIT;
				return true;

			}
			inline static GXuint32 getSize(){ return size; }

		private:
			static std::vector<std::unique_ptr<Event>> EventBuffer;
			static GXuint32 frontIte;
			static GXuint32 backIte;
			static GXuint32 size;

		};
		std::vector<std::unique_ptr<Event> > EventQueue::EventBuffer(EVENTS_LIMIT);
		GXuint32 EventQueue::frontIte = 0;
		GXuint32 EventQueue::backIte = 0;
		GXuint32 EventQueue::size = 0;
	}
	*/
	}
}

#endif