#ifndef GX_PHYSICS_SYSTEM_H
#define GX_PHYSICS_SYSTEM_H
#include "ECS/Systems/GXEntitySystem.h"

namespace gx {
	class GXPhysicsSystem : public GXEntitySystem
	{
	public:
		void Init();

		void Update(float dt);
	};
}
#endif
