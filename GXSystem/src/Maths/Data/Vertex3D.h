#ifndef VERTEX3D_H_
#define VERTEX3D_H_

#include "Maths/GXMaths.h"
namespace gx {
	struct Vertex3D {
		GXVec3 position;
		GXVec3 normal;
		GXVec3 tangent;
		GXVec3 bitangent;
		GXVec2 texCoords;
		Vertex3D(GXVec3 Position = GXVec3(0.0f, 0.0f, 0.0f),
			GXVec2 TexCoords = GXVec2(0.0f, 0.0f),
			GXVec3 Normal = GXVec3(0.0f, 1.0f, 0.0f),
			GXVec3 Tangent = GXVec3(0.0f, 0.0f, 0.0f),
			GXVec3 Bitangent = GXVec3(0.0f, 0.0f, 0.0f)) :
			position(Position),
			normal(Normal),
			tangent(Tangent),
			bitangent(Bitangent),
			texCoords(TexCoords)
		{

		}
	};
	struct PlaneVertex3D {
		GXVec2 position;
		GXiVec2 sampling_offset;
		PlaneVertex3D(GXVec2 Position = GXVec2(0.0f, 0.0f),
			glm::ivec2 SamplingOffset = glm::ivec2(0, 0)) :
			position(Position),
			sampling_offset(SamplingOffset)
		{

		}
	};

	struct PlaneInstanceData {
		GXVec2 position_offset;
		GXiVec2 sampling_start_offset;
		GXint32 level_LOD;
		PlaneInstanceData(GXVec2 PositionOffset = GXVec2(0.0f, 0.0f),
			GXiVec2 SamplingStartOffset = GXiVec2(0, 0),
			GXint32 LevelLOD = 0) :
			position_offset(PositionOffset),
			sampling_start_offset(SamplingStartOffset),
			level_LOD(LevelLOD)
		{

		}
	};
}

#endif