#pragma once

#include "AbstractModel.h"

struct vec3 { float x, y, z; vec3(float X, float Y, float Z) { x = X; y = Y; z = Z; } };
struct vec2 { float x, y; vec2(float X, float Y) { x = X; y = Y; } };

namespace popbin {

	class GeometryModel : public AbstractModel {
		public:
			GeometryModel(Entry* entry);

			void Export(const std::string& path);

			bool HasNormals;
			bool HasColors;

			std::vector<vec3> vertices, normals;
			std::vector<vec2> uvs;
			uint4 mesh_count;
			std::vector<std::vector<int2>> indices_verts, indices_uvs;

	};

}
