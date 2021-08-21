#ifndef INPUT_MANAGER_H_
#define INPUT_MANAGER_H_

#include "Config/pch.h"
#include "GXKeys.h"
#include "Window/GXWindow.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
namespace gx {
	class InputManager {
	public:

		InputManager(const InputManager& inst) = delete;
		inline void Init() {
			key_state_ = std::vector<bool>(event::key::GX_NUM_SCANCODES);
			mouse_loc_ = std::make_pair(0, 0);//x,y
			mouse_loc_rel_ = std::make_pair(0, 0);//xRel,yRel
			mouse_wheel_ = std::make_pair(0, 0);//horizontal,vertical
		}
		inline void Update() {
			mouse_wheel_.first = 0;
			mouse_wheel_.second = 0;
			mouse_loc_rel_.first = 0;
			mouse_loc_rel_.second = 0;
			text_ = "";
		}
		inline static InputManager& GetInstance() {
			//TO DO MAKE IT HEAP ALLOCATED INSTEAD.
			static InputManager instance;
			return instance;
		}
		//EVENTS
		template<class T>
		inline GXint32 HandleEvent(std::shared_ptr<T>& Event) { return 0; }

		//INPUT POLLING 
		inline bool IsPressed(GXuint32 keyVal) const { return key_state_[keyVal]; }
		inline std::string GetTextTyped() const { return text_; }
		//x,y
		inline std::pair<GXint32, GXint32> GetMouseLoc() const { return mouse_loc_; }
		//xRel,yRel
		inline std::pair<GXint32, GXint32> GetMouseLocRel() const { return mouse_loc_rel_; }
		//horizontal,vertical
		inline std::pair<GXint32, GXint32> GetMouseWheel() const { return mouse_wheel_; }
	private:
		InputManager() {}
		std::vector<bool> key_state_;
		std::string text_;
		std::pair<GXint32, GXint32> mouse_loc_;//x,y
		std::pair<GXint32, GXint32> mouse_loc_rel_;//xRel,yRel
		std::pair<GXint32, GXint32> mouse_wheel_;//horizontal,vertical
	};

	//EVENTS 
	template<>
	inline GXint32 InputManager::HandleEvent<gx::event::KeyPressEvent>(std::shared_ptr<gx::event::KeyPressEvent>& Event) {
		key_state_[Event->GetKeyVal()] = true;
		return 1;
	}
	template<>
	inline GXint32 InputManager::HandleEvent<gx::event::KeyReleaseEvent>(std::shared_ptr<gx::event::KeyReleaseEvent>& Event) {
		key_state_[Event->GetKeyVal()] = false;
		return 1;
	}
	template<>
	inline GXint32 InputManager::HandleEvent<gx::event::KeyTypedEvent>(std::shared_ptr<gx::event::KeyTypedEvent>& Event) {
		text_ = Event->GetTextVal();
		return 1;
	}
	template<>
	inline GXint32 InputManager::HandleEvent<gx::event::MouseScrollEvent>(std::shared_ptr<gx::event::MouseScrollEvent>& Event) {
		mouse_wheel_.first += Event->GetXOffset() > 0 ? 1 : -1;
		mouse_wheel_.second += Event->GetYOffset() > 0 ? 1 : -1;
		return 1;
	}
	template<>
	inline GXint32 InputManager::HandleEvent<gx::event::MousePressEvent>(std::shared_ptr<gx::event::MousePressEvent>& Event) {
		key_state_[Event->GetKeyVal()] = true;
		return 1;
	}
	template<>
	inline GXint32 InputManager::HandleEvent<gx::event::MouseReleaseEvent>(std::shared_ptr<gx::event::MouseReleaseEvent>& Event) {
		key_state_[Event->GetKeyVal()] = false;
		return 1;
	}
	template<>
	inline GXint32 InputManager::HandleEvent<gx::event::MouseMoveEvent>(std::shared_ptr<gx::event::MouseMoveEvent>& Event) {
		mouse_loc_.first = Event->GetXVal();
		mouse_loc_.second = Event->GetYVal();
		return 1;
	}
	template<>
	inline GXint32 InputManager::HandleEvent<gx::event::MouseMoveRelEvent>(std::shared_ptr<gx::event::MouseMoveRelEvent>& Event) {
		mouse_loc_rel_.first = Event->GetXVal();
		mouse_loc_rel_.second = Event->GetYVal();
		return 1;
	}

}

#endif