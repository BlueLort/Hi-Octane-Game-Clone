#include "Config/pch.h"
#include "GXComponent.h"

namespace gx {
	template<typename T>
	inline void GXComponentArray<T>::AddComponent(GXEntity entityId, T component)
	{
		auto ite = entity_to_index_.find(entityId);
		if (ite != entity_to_index_.end()) {
			GXE_ERROR("Failed to add component that belong to entity id:{0} as it already exists", entityId)
				return;
		}

		GXuint32 newIndex = current_size_;
		entity_to_index_[entityId] = newIndex;
		entity_to_index_[newIndex] = entityId;
		components_array_[newIndex] = component;
		current_size_++;
		GXE_INFO("Component {0} added to entity id:{1}", typeid(T).name(), entityId);
	}

	template<typename T>
	inline void GXComponentArray<T>::RemoveComponent(GXEntity entityId)
	{
		auto ite = entity_to_index_.find(entityId);
		if (ite == entity_to_index_.end()) {
			GXE_ERROR("Failed to remove non-existent component.")
				return;
		}

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
	inline T& GXComponentArray<T>::GetComponent(GXEntity entityId)
	{
		auto ite = entity_to_index_.find(entityId);
		if (ite == entity_to_index_.end()) {
			GXE_ERROR("Failed to find component that belong to entity id:{0}", entityId)
				return nullptr;
		}

		return components_array_[ite->second];
	}

	template<typename T>
	inline void GXComponentArray<T>::EntityDestroyed(GXEntity entityId)
	{
		if (entity_to_index_.find(entityId) != entity_to_index_.end())
		{
			RemoveComponent(entityId);
		}
	}

	inline void GXComponentManager::EntityDestroyed(GXEntity entityId)
	{
		for (auto const& ite : component_to_arrays_)
		{
			auto const& component = ite.second;

			component->EntityDestroyed(entityId);
		}
	}



	template<typename T>
	inline void GXComponentManager::RegisterComponent()
	{
		const char* typeName = typeid(T).name();
		auto ite = component_to_type_.find(typeName);
		if (ite != component_to_type_.end()) {
			GXE_ERROR("Can't register same component [{0}] more than once.", typeName);
			return;
		}
		component_to_type_.insert({ typeName, next_component_type_ });
		component_to_arrays_.insert({ typeName, std::make_shared<GXComponentArray<T>>() });
		next_component_type_++;
		GXE_INFO("Component {0} is registered.", typeName);
	}

	template<typename T>
	inline GXComponentType GXComponentManager::GetComponentType()
	{
		const char* typeName = typeid(T).name();
		auto ite = component_to_type_.find(typeName);
		if (ite == component_to_type_.end()) {
			GXE_ERROR("Component {0} is not registered.", typeName);
			return;
		}
		return ite->second;
	}

	template<typename T>
	inline void GXComponentManager::AddComponent(GXEntity entityId, T component)
	{
		GetComponentArray<T>()->AddComponent(entityId, component);
	}

	template<typename T>
	inline void GXComponentManager::RemoveComponent(GXEntity entityId)
	{
		GetComponentArray<T>()->RemoveComponent(entityId);
	}

	template<typename T>
	inline T& GXComponentManager::GetComponent(GXEntity entityId)
	{
		return GetComponentArray<T>()->GetComponent(entity);
	}

	// Convenience function to get the statically casted pointer to the ComponentArray of type T.

	template<typename T>
	inline std::shared_ptr<GXComponentArray<T>> GXComponentManager::GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		auto ite = component_to_type_.find(typeName);
		if (ite == component_to_type_.end()) {
			GXE_ERROR("Component {0} is not registered before usage.", typeName);
			return nullptr;
		}

		return std::static_pointer_cast<GXComponentArray<T>>(ite->second);
	}
}