#ifndef ECS_MANAGER_H
#define ECS_MANAGER_H
#include "ECS/Components/GXComponent.h"
#include "ECS/Entity/GXEntityManager.h"
#include "ECS/Systems/GXSystemManager.h"
#include "ECS/ECSConfig.h"

namespace gx {
	class ECSManager
	{
	public:
		void Init();
		// Entity
		GXEntity CreateEntity();

		void DestroyEntity(GXEntity entityId);
		// Component
		template<typename T>
		void RegisterComponent();

		template<typename T>
		void AddComponent(GXEntity entityId, T component);

		template<typename T>
		void RemoveComponent(GXEntity entityId);

		template<typename T>
		T& GetComponent(GXEntity entityId);

		template<typename T>
		GXComponentType GetComponentType();
		// System
		template<typename T>
		std::shared_ptr<T> RegisterSystem();

		template<typename T>
		void SetSystemMask(GXComponentMask mask);

	private:
		std::unique_ptr<GXComponentManager> component_manager_;
		std::unique_ptr<GXEntityManager> entity_manager_;
		std::unique_ptr<GXSystemManager> system_manager_;
	};
}

#endif

