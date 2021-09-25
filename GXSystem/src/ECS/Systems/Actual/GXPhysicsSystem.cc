#include "GXPhysicsSystem.h"

#include "ECS/Components/Actual/GXGravity.h"
#include "ECS/Components/Actual/GXRigidBody.h"
#include "ECS/Components/Actual/GXTransform.h"
#include "ECS/ECSManager.h"


extern gx::ECSManager ECScoordinator;

namespace gx{

	void GXPhysicsSystem::Init()
	{
	}

	void GXPhysicsSystem::Update(float dt)
	{
		for (auto const& entity : entites_managed_)
		{
			auto& rigidBody = ECScoordinator.GetComponent<GXRigidBody>(entity);
			auto& transform = ECScoordinator.GetComponent<GXTransform>(entity);

			auto const& gravity = ECScoordinator.GetComponent<GXGravity>(entity);

			transform.position += rigidBody.velocity * dt;

			rigidBody.velocity += gravity.force * dt;
		}
	}

}