#ifndef GX_SYSTEM_MANAGER_H
#define GX_SYSTEM_MANAGER_H

#include "GXEntitySystem.h"
#include "ECS/ECSConfig.h"

namespace gx {
	class GXSystemManager
	{
	public:
		template<typename T>
		std::shared_ptr<T> RegisterSystem();

		template<typename T>
		void SetMask(GXComponentMask mask);
		void EntityDestroyed(GXEntity entityId);
		void EntityMaskChanged(GXEntity entityId, GXComponentMask entityMask);

	private:
		std::unordered_map<const char*, GXComponentMask> systems_mask_;
		std::unordered_map<const char*, std::shared_ptr<GXEntitySystem>> systems_;
	};
	template<typename T>
	std::shared_ptr<T> GXSystemManager::RegisterSystem()
	{
		const char* typeName = typeid(T).name();
		auto ite = systems_.find(typeName);
		GXE_ASSERT(ite == systems_.end(), "Can't register same system [{0}] more than once.", typeName);

		auto system = std::make_shared<T>();
		systems_.insert({ typeName, system });
		GXE_INFO("System {0} is registered.", typeName);
		return system;
	}
	template<typename T>
	void GXSystemManager::SetMask(GXComponentMask mask)
	{
		const char* typeName = typeid(T).name();
		auto ite = systems_.find(typeName);
		GXE_ASSERT(ite != systems_.end(), "System {0} is not registered.", typeName);

		systems_mask_.insert({ typeName, mask });
	}
}

#endif