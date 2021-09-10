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
}

#endif