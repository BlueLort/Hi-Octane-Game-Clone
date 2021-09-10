#include "Config/pch.h"
#include "ECSManager.h"

namespace gx {
	inline void ECSManager::Init()
	{
		component_manager_ = std::make_unique<GXComponentManager>();
		entity_manager_ = std::make_unique<GXEntityManager>();
		system_manager_ = std::make_unique<GXSystemManager>();
	}

	// Entity

	inline GXEntity ECSManager::CreateEntity()
	{
		return entity_manager_->CreateEntity();
	}
	inline void ECSManager::DestroyEntity(GXEntity entityId)
	{
		entity_manager_->DestroyEntity(entityId);
		component_manager_->EntityDestroyed(entityId);
		system_manager_->EntityDestroyed(entityId);
	}
	// Component

	template<typename T>
	inline void ECSManager::RegisterComponent()
	{
		component_manager_->RegisterComponent<T>();
	}

	template<typename T>
	inline void ECSManager::AddComponent(GXEntity entityId, T component)
	{
		component_manager_->AddComponent<T>(entityId, component);

		auto mask = entity_manager_->GetComponentMask(entityId);
		mask.set(component_manager_->GetComponentType<T>(), true);
		entity_manager_->SetComponentMask(entityId, signature);

		system_manager_->EntityMaskChanged(entityId, mask);
	}

	template<typename T>
	inline void ECSManager::RemoveComponent(GXEntity entityId)
	{
		component_manager_->RemoveComponent<T>(entityId);

		auto mask = entity_manager_->GetComponentMask(entityId);
		signature.set(component_manager_->GetComponentType<T>(), false);
		entity_manager_->SetComponentMask(entityId, signature);

		system_manager_->EntityMaskChanged(entityId, signature);
	}

	template<typename T>
	inline T& ECSManager::GetComponent(GXEntity entityId)
	{
		return component_manager_->GetComponent<T>(entityId);
	}

	template<typename T>
	inline GXComponentType ECSManager::GetComponentType()
	{
		return component_manager_->GetComponentType<T>();
	}

	// System

	template<typename T>
	inline std::shared_ptr<T> ECSManager::RegisterSystem()
	{
		return system_manager_->RegisterSystem<T>();
	}

	template<typename T>
	inline void ECSManager::SetSystemMask(GXComponentMask mask)
	{
		system_manager_->SetMask<T>(mask);
	}

}
