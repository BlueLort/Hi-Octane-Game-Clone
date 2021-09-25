#include "Config/pch.h"
#include <GX.h>
gx::ECSManager ECScoordinator;

class Game : public gx::GameXApp {
public:
	Game() {}

	void Start() override;
	~Game() {}
private:

	std::shared_ptr<gx::GXPhysicsSystem> physics_system_;
};

gx::GameXApp* gx::CreateApp() {
	return new Game();
}

void Game::Start()
{
	ECScoordinator.Init();
	ECScoordinator.RegisterComponent<gx::GXTransform>();
	ECScoordinator.RegisterComponent<gx::GXGravity>();
	ECScoordinator.RegisterComponent<gx::GXRigidBody>();
	auto physicsSystem = ECScoordinator.RegisterSystem<gx::GXPhysicsSystem>();
	{
		gx::GXComponentMask mask;
		mask.set(ECScoordinator.GetComponentType<gx::GXGravity>());
		mask.set(ECScoordinator.GetComponentType<gx::GXRigidBody>());
		mask.set(ECScoordinator.GetComponentType<gx::GXTransform>());
		ECScoordinator.SetSystemMask<gx::GXPhysicsSystem>(mask);
	}
	physicsSystem->Init();

	while (is_running_) {
		GXFloat deltaTime = 1.0f / gx::GXTimer::GetAppTimer().GetDeltaTicks();
		gx::GXTimer::GetAppTimer().Update();
		gx::InputManager::GetInstance().Update();
		gx::GX_SDLEvent event;
		while (GXPollEvents(&event) == 1); //Send events to callback
	}
}