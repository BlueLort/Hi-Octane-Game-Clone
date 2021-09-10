#include "Config/pch.h"
#include "GXSystemManager.h"

namespace gx {
	template<typename T>
	inline std::shared_ptr<T> GXSystemManager::RegisterSystem()
	{
		const char* typeName = typeid(T).name();
		auto ite = systems_.find(typeName);
		if (ite != systems_.end()) {
			GXE_ERROR("Can't register same system [{0}] more than once.", typeName);
			return;
		}

		auto system = std::make_shared<T>();
		systems_.insert({ typeName, system });
		GXE_INFO("System {0} is registered.", typeName);
		return system;
	}
	template<typename T>
	inline void GXSystemManager::SetMask(GXComponentMask mask)
	{
		const char* typeName = typeid(T).name();
		auto ite = systems_.find(typeName);
		if (ite == systems_.end()) {
			GXE_ERROR("System {0} is not registered.", typeName);
			return;
		}

		systems_mask_.insert({ typeName, mask });
	}
	inline void GXSystemManager::EntityDestroyed(GXEntity entityId)
	{
		for (auto const& ite : systems_)
		{
			auto const& system = ite.second;

			system->entites_managed_.erase(entityId);
		}
	}
	inline void GXSystemManager::EntityMaskChanged(GXEntity entityId, GXComponentMask entityMask)
	{
		for (auto const& ite : systems_)
		{
			auto const& type = ite.first;
			auto const& system = ite.second;
			auto const& systemMask = systems_mask_[type];

			if ((entityMask & systemMask) == systemMask)
			{
				system->entites_managed_.insert(entityId);
			}
			else
			{
				system->entites_managed_.erase(entityId);
			}
		}
	}
}