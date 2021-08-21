#ifndef GX_TIMER_H_
#define GX_TIMER_H_

namespace gx {
	class GXTimer {//Class to calculate delta time and ticks between frames using whatever windowing API
	public:
		void Init() {
			last_ticks_ = SDL_GetTicks();
		}
		inline static GXTimer& GetAppTimer() {
			static GXTimer gxt;
			return gxt;
		}
		void Update() {
			GXuint32 currentTicks = SDL_GetTicks();
			delta_ticks_ = currentTicks - last_ticks_;
			last_ticks_ = currentTicks;
		}
		inline GXuint32 GetDeltaTicks()const { return delta_ticks_; }

	private:
		GXuint32 delta_ticks_;
		GXuint32 last_ticks_;

	};
}

#endif