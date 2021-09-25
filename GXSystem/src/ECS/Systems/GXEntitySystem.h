#ifndef GX_ENTITY_SYSTEM_H
#define GX_ENTITY_SYSTEM_H
#include "Config/pch.h"
namespace gx {
class GXEntitySystem {
public:
	std::set<GXEntity> entites_managed_;
};
}


#endif