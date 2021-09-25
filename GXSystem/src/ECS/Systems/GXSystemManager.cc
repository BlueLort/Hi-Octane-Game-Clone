#include "Config/pch.h"
#include "GXSystemManager.h"

namespace gx {
	
	void GXSystemManager::EntityDestroyed(GXEntity entityId)
	{
		for (auto const& ite : systems_)
		{
			auto const& system = ite.second;

			system->entites_managed_.erase(entityId);
		}
	}
	void GXSystemManager::EntityMaskChanged(GXEntity entityId, GXComponentMask entityMask)
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