#ifndef GX_ENTITY_SYSTEM_H
#define GX_ENTITY_SYSTEM_H
namespace gx {

class GXEntitySystem {
public:
	std::set<GXEntity> entites_managed_;
};
}


#endif