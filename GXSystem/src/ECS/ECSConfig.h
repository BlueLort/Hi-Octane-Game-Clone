#ifndef ECS_CONFIG_H
#define ECS_CONFIG_H
#include "GX/GXTypes.h"
#include "Config/Config.h"
#include <bitset>
namespace gx {
	const GXEntity		  MAX_ENTITIES = 1024;
	const GXComponentType MAX_COMPONENTS = 32;
	typedef std::bitset<MAX_COMPONENTS> GXComponentMask;
}
#endif