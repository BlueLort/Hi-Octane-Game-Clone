#ifndef GX_RIGIDBODY_H
#define GX_RIGIDBODY_H
#include "Maths/GXMaths.h"

namespace gx {
	struct GXRigidBody {
		GXVec3 velocity;
		GXVec3 acceleration;
	};
}
#endif