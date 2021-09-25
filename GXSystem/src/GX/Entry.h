#ifndef ENTRY_H_
#define ENTRY_H_
inline void InitSystems(gx::GameXApp* gameInst);
inline void DestroySystems(gx::GameXApp* gameInst);
GXint32 main(GXint32 argc, char** argv) {
	gx::GameXApp* Game = gx::CreateApp();
	InitSystems(Game);
	Game->Start();
	DestroySystems(Game);
	delete Game;

	return 0;
}
constexpr GXuint32 WINDOW_WIDTH = 1024;
constexpr GXuint32 WINDOW_HEIGHT = 768;
constexpr GXuint32 SDL_FLAGS =  NULL;
//init different components of the gx system.
void InitSystems(gx::GameXApp* gameInst) {
	gx::Logger::Init();	GXE_INFO("Logger initialized successfully !");
	gx::GXWindow::Init("Hi-Octane Game Clone", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_FLAGS); GXE_INFO("Window initialized successfully using SDL2.0 !");
	gx::GXWindow::AddEventsCallBack(gameInst->GetEventCallBack(), nullptr); GXE_INFO("Event CallBack Attached to SDL2.0 Event Queue !");
	gx::InputManager::GetInstance().Init(); GXE_INFO("InputManager initialized successfully!");
#ifdef USING_GLM_MATHS
	GXE_INFO("Using GLM as Maths library!");
#endif
	gx::GXTimer::GetAppTimer().Init(); GXE_INFO("Application Timer Started using SDL_GetTicks !");
//	init Graphics Context and appropriate Rendering System
#ifndef USING_VULKAN_API
	GXE_INFO("Using Software Rasterizer as default rendering engine.");
	//gx::GXRenderer::getInstance().init(); GXE_INFO("GXRenderer is Initialized Successfully !");
#else
	//TODO VULKAN INIT
	//gx::GXRenderer::getInstance().init(); GXE_INFO("GXRenderer is Initialized Successfully !");
	//TODO INIT REQUIRED SUB_SYSTEMS LIKE SHADERS.
#endif // !USING_VULKAN_API
	//TODO INIT IOMANAGER
}



void DestroySystems(gx::GameXApp* gameInst) {
	//TODO Destroy Core Systems + Resources Systems [like IOManager] in reverse order of init... 


}


#endif