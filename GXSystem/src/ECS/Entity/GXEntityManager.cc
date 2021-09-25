#include "Config/pch.h"
#include "GXEntityManager.h"
namespace gx {
	GXEntity GXEntityManager::CreateEntity()
	{
		GXE_INFO("Trying to create an entity current limit: {0} , count: {1}", MAX_ENTITIES, live_entity_count_);
		GXE_ASSERT(live_entity_count_ < MAX_ENTITIES, "Failed to create an entity because of max limit.");

		GXEntity entityId = available_entities_.front();
		available_entities_.pop();
		live_entity_count_++;
		GXE_INFO("Created an entity id: {0}", entityId);
		return entityId;
	}

	void GXEntityManager::DestroyEntity(GXEntity entityId)
	{
		GXE_INFO("Trying to destroy an entity id: {0}", entityId);
		GXE_ASSERT(entityId < MAX_ENTITIES || entityId >= 0, "Failed to destroy an entity because of wrong range.");

		// clear the components it has
		entity_components_mask_[entityId].reset();
		// return the id to the available ids
		available_entities_.push(entityId);
		live_entity_count_--;
		GXE_INFO("Destroyed entity with id: {0}", entityId);
	}

	void GXEntityManager::SetComponentMask(GXEntity entityId, GXComponentMask mask)
	{
		GXE_INFO("Trying to Set a mask for entity: {0}", entityId);
		GXE_ASSERT(entityId < MAX_ENTITIES || entityId >= 0, "Failed to destroy an entity because of wrong range.");

		entity_components_mask_[entityId] = mask;
		GXE_INFO("Changed mask for entity with id: {0}", entityId);
	}

	GXComponentMask GXEntityManager::GetComponentMask(GXEntity entityId)
	{
		GXE_ASSERT(entityId < MAX_ENTITIES || entityId >= 0, "Failed to destroy an entity because of wrong range.");

		return entity_components_mask_[entityId];
	}
}