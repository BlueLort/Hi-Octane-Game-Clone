#include "Config/pch.h"
#include "GXComponentManager.h"

namespace gx {
	void GXComponentManager::EntityDestroyed(GXEntity entityId)
	{
		for (auto const& ite : component_to_arrays_)
		{
			auto const& component = ite.second;

			component->EntityDestroyed(entityId);
		}
	}
}