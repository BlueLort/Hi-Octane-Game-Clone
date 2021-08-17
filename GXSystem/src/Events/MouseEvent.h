#pragma once

#include "Event.h"
namespace gx {
	namespace event {
		class MouseButtonEvent : public GXEvent {
		public:

			inline GXuint32 GetKeyVal() const { return key_val_; }
			virtual inline GXuint32 GetEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_MOUSE | GXEventClass::GX_MOUSE_BUTTON); }
			virtual inline const char* ToString()const override {
				std::stringstream ss;
				ss << KeyValToSTR(key_val_) + " " << this->GetName();
				return ss.str().c_str();
			}
		protected:

			MouseButtonEvent(GXuint32 KeyValue, GXuint32 WindowID) : key_val_(KeyValue) , GXEvent(WindowID) {}
		
			GXuint32 key_val_;

		};

		class MousePressEvent : public MouseButtonEvent {

		public:
			MousePressEvent(GXuint32 KeyValue, GXuint32 WindowID) : MouseButtonEvent(KeyValue,WindowID) {}
			virtual inline GXuint32 GetEventType() const override { return GXEventType::GX_MOUSE_PRESSED; };
			virtual inline  const char* GetName() const override { return "MOUSE_PRESSED"; }

		};

		class MouseReleaseEvent : public MouseButtonEvent {

		public:
			MouseReleaseEvent(GXuint32 KeyValue, GXuint32 WindowID) : MouseButtonEvent(KeyValue,WindowID) {}
			virtual inline GXuint32 GetEventType() const override { return GXEventType::GX_MOUSE_RELEASED; };
			virtual inline const char* GetName() const override { return "MOUSE_RELEASED"; }

		};


		class MouseMotionEvent : public GXEvent {
		public:
			inline GXint32 GetXVal() const { return x_; }
			inline GXint32 GetYVal() const { return y_; }
			virtual inline GXuint32 GetEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_MOUSE); }
			virtual inline const char* GetName() const override { return "MOUSE_MOVED"; }
		protected:
			MouseMotionEvent(GXint32 X, GXint32 Y, GXuint32 WindowID) : x_(X), y_ (Y),GXEvent(WindowID) {}
			virtual inline const char* ToString() const override {
				std::stringstream ss;
				ss << this->GetName() << "|X: " + std::to_string(x_) + " |Y: " + std::to_string(y_) + " |";
				return ss.str().c_str();
			}
			GXint32 x_;
			GXint32 y_;
		};

		class MouseMoveEvent : public MouseMotionEvent {

		public:
	
			MouseMoveEvent(GXint32 X, GXint32 Y, GXuint32 WindowID) : MouseMotionEvent(X,Y,WindowID) {}
			virtual inline GXuint32 GetEventType() const override { return GXEventType::GX_MOUSE_MOVED; }
		};
		class MouseMoveRelEvent : public MouseMotionEvent {

		public:
			MouseMoveRelEvent(GXint32 XRel, GXint32 YRel, GXuint32 WindowID) : MouseMotionEvent(XRel, YRel,WindowID) {}
			virtual inline GXuint32 GetEventType() const override { return GXEventType::GX_MOUSE_MOVED; }
		};

		class MouseScrollEvent : public GXEvent {

		public:
			MouseScrollEvent(GXint32 xOffset, GXint32 yOffset, GXuint32 WindowID) : xOff_(xOffset), yOff_(yOffset),GXEvent(WindowID) {}
			inline GXint32 GetXOffset() const { return xOff_; }
			inline GXint32 GetYOffset() const { return yOff_; }
			virtual inline GXuint32 GetEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_MOUSE); }
			virtual inline GXuint32 GetEventType() const override { return GXEventType::GX_MOUSE_SCROLL; }
			virtual inline const char* GetName() const override { return "MOUSE_SCROLLED"; }
		private:
			GXint32 xOff_;
			GXint32 yOff_;
		};
	}

}