#ifndef ECS_MANAGER_H
#define ECS_MANAGER_H
#include "ECS/Components/GXComponentManager.h"
#include "ECS/Entity/GXEntityManager.h"
#include "ECS/Systems/GXSystemManager.h"
#include "ECS/ECSConfig.h"

namespace gx {
	class ECSManager
	{
	public:
		void Init()
		{
			component_manager_ = std::make_unique<GXComponentManager>();
			entity_manager_ = std::make_unique<GXEntityManager>();
			system_manager_ = std::make_unique<GXSystemManager>();
		}
		// Entity
		inline GXEntity CreateEntity()
		{
			return entity_manager_->CreateEntity();
		}

		inline void DestroyEntity(GXEntity entityId)
		{
			entity_manager_->DestroyEntity(entityId);
			component_manager_->EntityDestroyed(entityId);
			system_manager_->EntityDestroyed(entityId);
		}
		// Component
		template<typename T>
		inline void RegisterComponent()
		{
			component_manager_->RegisterComponent<T>();
		}

		template<typename T>
		inline void AddComponent(GXEntity entityId, T component)
		{
			component_manager_->AddComponent<T>(entityId, component);

			auto mask = entity_manager_->GetComponentMask(entityId);
			mask.set(component_manager_->GetComponentType<T>(), true);
			entity_manager_->SetComponentMask(entityId, signature);

			system_manager_->EntityMaskChanged(entityId, mask);
		}

		template<typename T>
		inline void RemoveComponent(GXEntity entityId)
		{
			component_manager_->RemoveComponent<T>(entityId);

			auto mask = entity_manager_->GetComponentMask(entityId);
			signature.set(component_manager_->GetComponentType<T>(), false);
			entity_manager_->SetComponentMask(entityId, signature);

			system_manager_->EntityMaskChanged(entityId, signature);
		}

		template<typename T>
		inline T& GetComponent(GXEntity entityId)
		{
			return component_manager_->GetComponent<T>(entityId);
		}

		template<typename T>
		inline GXComponentType GetComponentType()
		{
			return component_manager_->GetComponentType<T>();
		}
		// System
		template<typename T>
		inline std::shared_ptr<T> RegisterSystem()
		{
			return system_manager_->RegisterSystem<T>();
		}

		template<typename T>
		inline void SetSystemMask(GXComponentMask mask)
		{
			system_manager_->SetMask<T>(mask);
		}

	private:
		std::unique_ptr<GXComponentManager> component_manager_;
		std::unique_ptr<GXEntityManager> entity_manager_;
		std::unique_ptr<GXSystemManager> system_manager_;
	};
}

#endif

