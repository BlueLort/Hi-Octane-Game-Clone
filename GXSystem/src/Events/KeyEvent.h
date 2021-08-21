#ifndef KEY_EVENT_H_
#define KEY_EVENT_H_

#include "Event.h"
namespace gx {
	namespace event {
		class KeyEvent : public GXEvent {
		public:
			inline GXuint32 GetKeyVal() const { return key_val_; }
			virtual inline GXuint32 GetEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_KEYBOARD); }
		protected:
			KeyEvent(GXuint32 KeyValue, GXuint32 WindowID) : key_val_(KeyValue),GXEvent(WindowID) {}
			virtual const char* ToString()const override {
				std::stringstream ss;
				ss <<  KeyValToSTR(key_val_) + " " <<this->GetName();
				return ss.str().c_str();
			}
			GXuint32 key_val_;
		};


		class KeyPressEvent : public KeyEvent {

		public:
			KeyPressEvent(GXuint32 KeyValue, GXuint32 WindowID) : KeyEvent(KeyValue,WindowID)  {}
			virtual inline const char* GetName() const override { return "KEY_PRESSED"; }
			virtual inline GXuint32 GetEventType() const override { return GXEventType::GX_KEY_PRESSED; }
			
		
		};
		class KeyReleaseEvent : public KeyEvent {

		public:
			KeyReleaseEvent(GXuint32 KeyValue, GXuint32 WindowID) : KeyEvent(KeyValue,WindowID) {}
			virtual inline const char* GetName() const override { return "KEY_RELEASED"; }
			virtual inline GXuint32 GetEventType() const override { return GXEventType::GX_KEY_RELEASED; }

		};
		class KeyTypedEvent : public GXEvent {

		public:
			KeyTypedEvent(char* TextValue, GXuint32 WindowID) : GXEvent(WindowID) { text_val_ += TextValue; }
			virtual inline const char* GetName() const override { return "KEY_TYPED"; }
			virtual inline GXuint32 GetEventType() const override { return GXEventType::GX_KEY_TYPED; }
			inline const char* GetTextVal() const { return text_val_.c_str(); }
			virtual inline GXuint32 GetEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_KEYBOARD); }
		private:
			std::string text_val_;
		};

	}

}

#endif