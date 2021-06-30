#include "GeometryModel.h"
#include "../BinArchive.h"

#include <fstream>

namespace popbin {
	GeometryModel::GeometryModel(Entry* entry) : AbstractModel(entry) {
		BinArchive* archive = entry->parentArchive;
		ByteBuffer* bb = new ByteBuffer(entry->data, entry->size);

		///// 
		uint4 type, temp, param1, param3;
		uint2 param2_1, param2_2;
		bb->Read<uint4>(&type);
		// lets parse

		bb->Read<uint4>(&param1); // idk
		bb->Read<uint2>(&param2_1); // normals have this higher than 4
		bb->Read<uint2>(&param2_2); // normals have this higher than 4
		bb->Read<uint4>(&param3); // idk

		uint4 vertices_count, floats_count, uv_count;
		bb->Read<uint4>(&vertices_count); // vertices count
		bb->Read<uint4>(&floats_count); // floats count (hopefully), dunno what they are used for
		bb->Read<uint4>(&temp); // unknown but maybe its bones count?
		bb->Read<uint4>(&uv_count); // UVs count (hopefully)
		bb->Read<uint4>(&mesh_count); // meshcount seems to be
		bb->Read<uint4>(&temp); // unknown
		bb->Read<uint4>(&temp); // unknown

		bool has_normals = (param2_1) & 1; // first bit
		bool has_floats = (param2_1 >> 1) & 1; // second bit

		for (int i = 0; i < vertices_count; ++i) {
			float x, y, z;
			bb->Read<float>(&x);
			bb->Read<float>(&y);
			bb->Read<float>(&z);

			vertices.push_back(vec3(x, z, -y)); // Z up to Y up fix
		}

		if (has_normals) {
			// i gotta find out which mesh has normals in a stable way
			for (int i = 0; i < vertices_count; ++i) {
				float x, y, z;
				bb->Read<float>(&x);
				bb->Read<float>(&y);
				bb->Read<float>(&z);

				normals.push_back(vec3(x, z, -y)); // Z up to Y up fix
			}
		}
		else {
			for (int i = 0; i < vertices_count; ++i) {
				normals.push_back(vec3(0, 1, 0));
			}
		}

		if (floats_count && has_floats) {
			std::vector<float> floats; floats.resize(floats_count);
			bb->ReadArray(&floats[0], floats_count);
		}

		// read uvs
		for (int i = 0; i < uv_count; ++i) {
			float x, y;
			bb->Read<float>(&x);
			bb->Read<float>(&y);

			uvs.push_back(vec2(x, y));
		}

		indices_verts.resize(mesh_count);
		indices_uvs.resize(mesh_count);
		for (int i = 0; i < mesh_count; i++) {
			uint4 faces_count;
			uint4 spacer; // dunno what it is but its placed after indices count and after every triangle (which has 6 bytes for 3 uint2 indices and 6 bytes for 3 uint2 uvindices = 12 bytes)
			bb->Read<uint4>(&faces_count); // read indices count
			bb->Read<uint4>(&spacer); // read spacer

			indices_verts[i].push_back(faces_count);
		}

		for (int i = 0; i < mesh_count; i++) {
			uint4 spacer; // damn spacer

			int faces_count = indices_verts[i][0];
			indices_verts[i].clear();

			for (int j = 0; j < faces_count; j++) {
				int2 v1, v2, v3, vt1, vt2, vt3;

				bb->Read<int2>(&v1);
				bb->Read<int2>(&v2);
				bb->Read<int2>(&v3);

				bb->Read<int2>(&vt1);
				bb->Read<int2>(&vt2);
				bb->Read<int2>(&vt3);

				bb->Read<uint4>(&spacer); // read spacer

				indices_verts[i].push_back(v1 + 1);
				indices_verts[i].push_back(v2 + 1);
				indices_verts[i].push_back(v3 + 1);

				indices_uvs[i].push_back(vt1 + 1);
				indices_uvs[i].push_back(vt2 + 1);
				indices_uvs[i].push_back(vt3 + 1);
			}
		}
	}

	void GeometryModel::Export(const std::string& path) {
		std::ofstream out(path);

		if (!out.is_open()) {
			std::cout << "its bad.";
			return;
		}

		out.setf(std::ios::fixed, std::ios::floatfield);
		out.setf(std::ios::showpoint);

		out << "# POPBin generated mesh file\n\n";

		for (auto v : vertices) {
			out << "v " << v.x << " " << v.y << " " << v.z << "\n";
		}

		out << "\n\n";

		for (auto v : normals) {
			out << "vn " << v.x << " " << v.y << " " << v.z << "\n";
		}

		out << "\n\n";

		for (auto v : uvs) {
			out << "vt " << v.x << " " << v.y << "\n";
		}

		out << "\n\n";

		for (int j = 0; j < mesh_count; j++) {
			out << "g " << j << "\n";
			for (int i = 0; i < indices_verts[j].size(); i += 3) {
				//out << "f " << indices_verts[i] << "/" << indices_uvs[i] << "/" << indices_normals[i] << " " << indices_verts[i + 1] << "/" << indices_uvs[i + 1] << "/" << indices_normals[i + 1] << " " << indices_verts[i + 2] << "/" << indices_uvs[i + 2] << "/" << indices_normals[i + 2] << "\n";
				out << "f " << indices_verts[j][i] << "/" << indices_uvs[j][i] << "/" << indices_verts[j][i] << " " << indices_verts[j][i + 1] << "/" << indices_uvs[j][i + 1] << "/" << indices_verts[j][i + 1] << " " << indices_verts[j][i + 2] << "/" << indices_uvs[j][i + 2] << "/" << indices_verts[j][i + 2] << "\n";
				//out << "f " << indices_verts[i] << " " << indices_verts[i + 1] << " " << indices_verts[i + 2] << "\n";
			}
			out << "\n";
		}

		out << "\n";

	}

}
