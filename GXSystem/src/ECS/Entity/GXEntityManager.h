#ifndef GX_ENTITY_MANAGER_H
#define GX_ENTITY_MANAGER_H
#include "ECS/ECSConfig.h"
namespace gx {
	class GXEntityManager
	{
	public:
		GXEntityManager(): live_entity_count_(0)
		{
			for (GXEntity entity = 0; entity < MAX_ENTITIES; entity++)
			{
				available_entities_.push(entity);
			}
		}

		GXEntity CreateEntity();
		void DestroyEntity(GXEntity entityId);
		void SetComponentMask(GXEntity entityId, GXComponentMask mask);
		GXComponentMask GetComponentMask(GXEntity entityId);
	private:
		// populate with IDs and keep unused IDs in it.
		std::queue<GXEntity> available_entities_;

		std::array<GXComponentMask, MAX_ENTITIES> entity_components_mask_;
		GXuint32 live_entity_count_;
	};
}
#endif
