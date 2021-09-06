#ifndef WINDOW_EVENT_H_
#define WINDOW_EVENT_H_

#include "Event.h"
namespace gx {
	namespace event {
		class WindowEvent : public GXEvent {
		public:
			virtual inline GXuint32 GetEventClass() const override { return (GXEventClass::GX_APPLICATION); }
		protected:
			WindowEvent(GXuint32 WindowID) : GXEvent(WindowID) {}

		};
		class WindowSizeEvent : public WindowEvent {//in SDL this event is followed by Resize event so i just check for resize.
		public:
	

			virtual inline const char* GetName() const override { return "WINDOW_SIZE"; }
			virtual inline GXuint32 GetEventType() const override { return GXEventType::GX_WINDOW_SIZE_CHANGED; }
			inline GXuint32 GetWidth() const { return width_; }
			inline GXuint32 GetHeight() const { return height_; }
			virtual inline const char* ToString() const override
			{
				std::stringstream ss;
				ss << this->GetName() << "| " << width_ << " , " << height_ << " |";
				return ss.str().c_str();
			}
		protected:
			WindowSizeEvent(GXuint32 Width, GXuint32 Height, GXuint32 WindowID)
				: width_(Width), height_(Height), WindowEvent(WindowID) {}
			GXuint32 width_;
			GXuint32 height_;

		};
		class WindowMoveEvent : public WindowEvent {//in SDL this event is followed by Resize event so i just check for resize.
		public:
			WindowMoveEvent(GXuint32 X, GXuint32 Y, GXuint32 WindowID)
				: x_(X), y_(Y),WindowEvent(WindowID) {}
			virtual inline const char* GetName() const override { return "WINDOW_MOVED"; }
			virtual inline GXuint32 GetEventType() const override { return GXEventType::GX_WINDOW_MOVED; }
			inline GXuint32 GetXLoc() const { return x_; }
			inline GXuint32 GetYLoc() const { return y_; }

			virtual inline const char* ToString() const override
			{
				std::stringstream ss;
				ss << this->GetName() << "| " << x_ << " , " << y_ << " |";
				return ss.str().c_str();
			}
		protected:
			GXuint32 x_;
			GXuint32 y_;

		};
		class WindowResizeEvent : public WindowSizeEvent {
		public:
			WindowResizeEvent(GXuint32 Width, GXuint32 Height, GXuint32 WindowID)
				: WindowSizeEvent(Width,Height,WindowID) {}
			virtual inline const char* GetName() const override { return "WINDOW_RESIZE"; }
			virtual inline GXuint32 GetEventType() const override { return GXEventType::GX_WINDOW_RESIZE; }
		};
		class WindowMaximizeEvent : public WindowSizeEvent {
		public:
			WindowMaximizeEvent(GXuint32 Width, GXuint32 Height ,GXuint32 WindowID)
				: WindowSizeEvent(Width, Height,WindowID) {}
			virtual inline const char* GetName() const override { return "WINDOW_MAXIMIZE"; }
			virtual inline GXuint32 GetEventType() const override { return GXEventType::GX_WINDOW_MAXIMIZE; }
		};
		class WindowMinimizeEvent : public WindowSizeEvent {
		public:
			WindowMinimizeEvent(GXuint32 Width, GXuint32 Height, GXuint32 WindowID)
				: WindowSizeEvent(Width, Height,WindowID) {}
			virtual inline const char* GetName() const override { return "WINDOW_MINIMIZE"; }
			virtual inline GXuint32 GetEventType() const override { return GXEventType::GX_WINDOW_MINIMIZE; }
		};
		class WindowCloseEvent : public WindowEvent {
		public:
			WindowCloseEvent(GXuint32 WindowID):WindowEvent(WindowID) {}
			virtual inline const char* GetName() const override { return "WINDOW_CLOSE"; }
			virtual inline GXuint32 GetEventClass() const override {return (GXEventClass::GX_APPLICATION);}
			virtual inline GXuint32 GetEventType() const override { return GXEventType::GX_WINDOW_CLOSE; }
		};
		class WindowRestoredEvent : public WindowEvent {
		public:
			WindowRestoredEvent(GXuint32 WindowID) : WindowEvent(WindowID) {}
			virtual inline const char* GetName() const override { return "WINDOW_RESOTRED"; }
			virtual inline GXuint32 GetEventClass() const override { return (GXEventClass::GX_APPLICATION); }
			virtual inline GXuint32 GetEventType() const override { return GXEventType::GX_WINDOW_RESTORED; }
		};
	}

}

#endif