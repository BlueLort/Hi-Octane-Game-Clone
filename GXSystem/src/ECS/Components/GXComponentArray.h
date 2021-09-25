#ifndef GX_COMPONENT_ARRAY_H
#define GX_COMPONENT_ARRAY_H
#include "ECS/ECSConfig.h"
namespace gx {
	class IGXComponentArray
	{
	public:
		virtual ~IGXComponentArray() = default;
		virtual void EntityDestroyed(GXEntity entityId) = 0;
	};


	template<typename T>
	class GXComponentArray : public IGXComponentArray
	{
	public:
		void AddComponent(GXEntity entityId, T component);
		void RemoveComponent(GXEntity entityId);
		T& GetComponent(GXEntity entityId);
		void EntityDestroyed(GXEntity entityId);
	private:
		// each entity has a unique spot for components
		std::array<T, MAX_ENTITIES> components_array_;
		// which component is owned by entity?
		std::unordered_map<GXEntity, GXuint32> entity_to_index_;
		// which entity do own this component?
		std::unordered_map<GXuint32, GXEntity> index_to_entity_;
		GXuint32 current_size_;
	};

	template<typename T>
	void GXComponentArray<T>::AddComponent(GXEntity entityId, T component)
	{
		auto ite = entity_to_index_.find(entityId);
		GXE_ASSERT(ite == entity_to_index_.end(), "Failed to add component that belong to entity id:{0} as it already exists", entityId);

		GXuint32 newIndex = current_size_;
		entity_to_index_[entityId] = newIndex;
		entity_to_index_[newIndex] = entityId;
		components_array_[newIndex] = component;
		current_size_++;
		GXE_INFO("Component {0} added to entity id:{1}", typeid(T).name(), entityId);
	}

	template<typename T>
	void GXComponentArray<T>::RemoveComponent(GXEntity entityId)
	{
		auto ite = entity_to_index_.find(entityId);
		GXE_ASSERT(ite != entity_to_index_.end(), "Failed to remove non-existent component.");

		// Copy element at end into deleted element's place to maintain density
		GXuint32 indexOfRemovedEntity = ite->second;
		GXuint32 indexOfLastElement = current_size_ - 1;
		components_array_[indexOfRemovedEntity] = components_array_[indexOfLastElement];

		// Update map to point to moved spot
		GXEntity entityOfLastElement = index_to_entity_[indexOfLastElement];
		entity_to_index_[entityOfLastElement] = indexOfRemovedEntity;
		index_to_entity_[indexOfRemovedEntity] = entityOfLastElement;

		entity_to_index_.erase(entityId);
		index_to_entity_.erase(indexOfLastElement);
		current_size_--;
		GXE_INFO("Component {0} removed from entity id:{1}", typeid(T).name(), entityId);
	}

	template<typename T>
	T& GXComponentArray<T>::GetComponent(GXEntity entityId)
	{
		auto ite = entity_to_index_.find(entityId);
		GXE_ASSERT(ite != entity_to_index_.end(), "Failed to find component that belong to entity id:{0}", entityId);

		return components_array_[ite->second];
	}

	template<typename T>
	void GXComponentArray<T>::EntityDestroyed(GXEntity entityId)
	{
		if (entity_to_index_.find(entityId) != entity_to_index_.end())
		{
			RemoveComponent(entityId);
		}
	}
}
#endif