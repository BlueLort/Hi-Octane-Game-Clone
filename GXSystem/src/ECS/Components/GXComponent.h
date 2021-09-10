#ifndef GX_COMPONENT_H
#define GX_COMPONENT_H
#include "ECS/ECSConfig.h"
namespace gx {
	class IGXComponentArray
	{
	public:
		virtual ~IGXComponentArray() = default;
		virtual void EntityDestroyed(GXEntity entityId) = 0;
	};


	template<typename T>
	class GXComponentArray : public IComponentArray
	{
	public:
		void AddComponent(GXEntity entityId, T component);
		void RemoveComponent(GXEntity entityId);
		T& GetComponent(GXEntity entityId);
		void EntityDestroyed(GXEntity entityId) override;
	private:
		// each entity has a unique spot for components
		std::array<T, MAX_ENTITIES> components_array_;
		// which component is owned by entity?
		std::unordered_map<GXEntity, GXuint32> entity_to_index_;
		// which entity do own this component?
		std::unordered_map<GXuint32, GXEntity> index_to_entity_;
		GXuint32 current_size_;
	};

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
}
#endif
