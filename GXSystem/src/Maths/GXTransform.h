#pragma once
#include "GXMaths.h"
namespace gx {

	class GXTransform {
	public:
		
		GXTransform() :
		position_(0.0f, 0.0f, 0.0f),
		scale_(1.0f, 1.0f, 1.0f),
		rotation_(0.0f, 0.0f, 0.0f),
		up_(0.0f, 1.0f, 0.0f),
		worldUp_(0.0f,1.0f,0.0f),
		forward_(0.0f,0.0f,1.0f),
		right_(1.0f,0.0f,0.0f)
		{
		}
		GXVec3 position_;
		GXVec3 scale_;
		//angles in degrees
		GXVec3 rotation_;
		GXVec3 up_;
		GXVec3 worldUp_;
		GXVec3 forward_;
		GXVec3 right_;


		inline GXMat4 GetModel() {
			
			GXMat4 model(1.0f);
			model = GXMaths::Translate(model, position_);
			GXMat4 rotMat(1.0f);
			//TODO MOVE TO Quaternions
			rotMat = GXMaths::Rotate(rotMat, GXRad(rotation_.x), GXVec3(1.0f, 0.0f, 0.0f)) * GXMaths::Rotate(rotMat, GXRad(rotation_.y), GXVec3(0.0f, 1.0f, 0.0f)) * GXMaths::Rotate(rotMat, GXRad(rotation_.z), GXVec3(0.0f, 0.0f, 1.0f));
			model = model * rotMat;
			model = GXMaths::Scale(model, scale_);
			return model;
		}

		inline GXMat4 GetLookAt(const GXVec3& target) {

			return GXMat4();
		}

	};

}