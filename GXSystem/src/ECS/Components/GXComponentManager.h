#ifndef GX_COMPONENT_H
#define GX_COMPONENT_H
#include "ECS/ECSConfig.h"
#include "GXComponentArray.h"
namespace gx {

	class GXComponentManager
	{
	public:
		template<typename T>
		void RegisterComponent();

		template<typename T>
		GXComponentType GetComponentType();

		template<typename T>
		void AddComponent(GXEntity entityId, T component);

		template<typename T>
		void RemoveComponent(GXEntity entityId);

		template<typename T>
		T& GetComponent(GXEntity entityId);
		void EntityDestroyed(GXEntity entityId);
	private:
		std::unordered_map<const char*, GXComponentType> component_to_type_;
		std::unordered_map<const char*, std::shared_ptr<IGXComponentArray>> component_to_arrays_;

		GXComponentType next_component_type_;

		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template<typename T>
		std::shared_ptr<GXComponentArray<T>> GetComponentArray();
	};
	

	template<typename T>
	void GXComponentManager::RegisterComponent()
	{
		const char* typeName = typeid(T).name();
		auto ite = component_to_type_.find(typeName);
		GXE_ASSERT(ite == component_to_type_.end(), "Can't register same component [{0}] more than once.", typeName);

		component_to_type_.insert({ typeName, next_component_type_ });
		component_to_arrays_.insert({ typeName, std::make_shared<GXComponentArray<T>>() });
		next_component_type_++;
		GXE_INFO("Component {0} is registered.", typeName);
	}

	template<typename T>
	GXComponentType GXComponentManager::GetComponentType()
	{
		const char* typeName = typeid(T).name();
		auto ite = component_to_type_.find(typeName);
		GXE_ASSERT(ite != component_to_type_.end(), "Component {0} is not registered.", typeName);

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
		return GetComponentArray<T>()->GetComponent(entityId);
	}

	// Convenience function to get the statically casted pointer to the ComponentArray of type T.

	template<typename T>
	std::shared_ptr<GXComponentArray<T>> GXComponentManager::GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		auto ite = component_to_arrays_.find(typeName);
		GXE_ASSERT(ite != component_to_arrays_.end(), "Component Array {0} is not added before usage.", typeName);

		return std::static_pointer_cast<GXComponentArray<T>>(ite->second);
	}
}
#endif
