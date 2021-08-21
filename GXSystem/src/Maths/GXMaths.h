#ifndef GX_MATHS_H_
#define GX_MATHS_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GX/GXTypes.h"


namespace gx {

#define CLAMP(a,start,end) if (a < start)a = start;else if (a > end)a = end;	
using GXMat4 = glm::mat4;
using GXMat3 = glm::mat3;
using GXMat2 = glm::mat2;
using GXVec4 = glm::vec4;
using GXVec3 = glm::vec3;
using GXVec2 = glm::vec2;
using GXiVec4 = glm::ivec4;
using GXiVec3 = glm::ivec3;
using GXiVec2 = glm::ivec2;
using GXuVec4 = glm::uvec4;
using GXuVec3 = glm::uvec3;
using GXuVec2 = glm::uvec2;
#define GXPtr(structure) glm::value_ptr(structure)
#define GXRad glm::radians
#define GXCos glm::cos
#define GXSin glm::sin
#define GXPI (3.14159265359)
	//Static class that wraps the Maths Library used around the Engine
	//Adding whatever function i need to the wrapper before using it in the engine.
	class GXMaths {
		GXMaths() = delete;
	public:
		inline static double InverseLerp(double val,double a,double b){ return (val - a) / (b - a); }
		inline static double Lerp(double f, double a, double b) { return a + f * (b - a); }
		inline static double CosInterp(double f,double a,double b){
			double newFactor = (1 - cos(f *GXPI)) / 2;
			return(a + newFactor * (b - a));
		}
		
		inline static GXMat4 Prespective(const float& FOV,const float& AR,const float& zNear,const float& zFar ) {
			return glm::perspective(FOV,AR,zNear,zFar);
		}
		inline static GXMat4 Ortho(const float& left, const float& right, const float& bottom, const float& top, const float& zNear, const float& zFar) {
			return glm::ortho(left, right, bottom, top, zNear, zFar);
		}
		inline static GXMat4 LookAt(const GXVec3& pos, const GXVec3& target, const GXVec3& up) {
			return glm::lookAt(pos, target, up);
		}
		inline static GXMat4 Translate(const GXMat4& Model,const GXVec3& Amount) {
			return glm::translate(Model,Amount);
		}
		inline static GXMat4 Rotate(const GXMat4& Model,const GXFloat& Angle, const GXVec3& Axis) {
			return glm::rotate(Model,Angle,Axis);
		}
		inline static GXMat4 Scale(const GXMat4& Model,const GXVec3& Amount) {
			return glm::scale(Model,Amount);
		}
		inline static GXMat4 Inverse(const GXMat4& mat) {
			return glm::inverse(mat);
		}
		inline static GXMat3 Inverse(const GXMat3& mat) {
			return glm::inverse(mat);
		}
		inline static GXVec3 Normalize(const GXVec3& vec) {
			return glm::normalize(vec);
		}
		inline static GXVec3 Cross(const GXVec3& vec1, const GXVec3& vec2) {
			return glm::cross(vec1, vec2);
		}

		// TODO IMPROVE THE NEXT FUNCTIONS [ remove redundency  ? curiously recurring template pattern? ]
		inline static GXMat4 Transpose(const GXMat4& mat) {
			return glm::transpose(mat);
		}
		inline static GXMat4 Transpose(const GXMat3& mat) {
			return glm::transpose(mat);
		}

		//DO NOT FORGET TO DELETE THE DATA 
		inline static GXFloat* GetDataPtr(const GXMat4& data) {
			const GXFloat* cptr = GXPtr(data);
			GXFloat* out= new GXFloat[16];
			for (GXint32 i = 0; i < 16; i++)out[i] = cptr[i];
			return out;
		}
		//DO NOT FORGET TO DELETE THE DATA 
		inline static GXFloat* GetDataPtr(const GXMat3& data) {
			const GXFloat* cptr = GXPtr(data);
			GXFloat* out = new GXFloat[9];
			for (GXint32 i = 0; i < 9; i++)out[i] = cptr[i];
			return out;
		}
		//DO NOT FORGET TO DELETE THE DATA 
		inline static GXFloat* GetDataPtr(const GXVec3& data) {
			const GXFloat* cptr = GXPtr(data);
			GXFloat* out = new GXFloat[3];
			for (GXint32 i = 0; i < 3; i++)out[i] = cptr[i];
			return out;
		}
		//DO NOT FORGET TO DELETE THE DATA 
		inline static GXFloat* GetDataPtr(const GXVec4& data) {
			const GXFloat* cptr = GXPtr(data);
			GXFloat* out = new GXFloat[4];
			for (GXint32 i = 0; i < 4; i++)out[i] = cptr[i];
			return out;
		}
	};
}

#endif