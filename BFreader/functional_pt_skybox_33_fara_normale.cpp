#include "defines.h"

#include "../popbin_lib/BinArchive.h"

#include <sstream>
#include <fstream>

struct vec3 { float x, y, z; vec3(float X, float Y, float Z) { x = X; y = Y; z = Z; } };
struct vec2 { float x, y; vec2(float X, float Y) { x = X; y = Y; } };

int main(int argc, char** argv) {
	std::ifstream in1("900_Skybox_wow_ff0e601f.bin", std::ios::binary);

	if (!in1.is_open()) {
		std::cout << "Its bad.";
		return 0;
	}

	popbin::BinArchive* arch = new popbin::BinArchive(in1);

	popbin::Entry& entry = arch->Entries[33];

	// Geometry parser
	std::vector<vec3> vertices, normals;
	std::vector<vec2> uvs;
	std::vector<int2> indices_verts, indices_uvs, indices_normals;

	
	ByteBuffer* bb = new ByteBuffer(entry.data, entry.size);
	uint4 type, temp;
	bb->Read<uint4>(&type);
	// lets parse

	// skip 12 unknown bytes (they are the same in every geometry file seen as of now)
	bb->Position() += 12;

	uint4 vertices_count, normals_count, uv_count;
	bb->Read<uint4>(&vertices_count); // vertices count
	bb->Read<uint4>(&normals_count); // normals count (hopefully)
	bb->Read<uint4>(&temp); // unknown but maybe its bones count?
	bb->Read<uint4>(&uv_count); // UVs count (hopefully)
	bb->Read<uint4>(&temp); // unknown, it was meshcount logic
	bb->Read<uint4>(&temp); // unknown
	bb->Read<uint4>(&temp); // unknown

	for (int i = 0; i < vertices_count; ++i) {
		float x, y, z;
		bb->Read<float>(&x);
		bb->Read<float>(&y);
		bb->Read<float>(&z);

		vertices.push_back(vec3(x, y, z));
		normals.push_back(vec3(0, -1, 0));
	}

	// idk why but there are no normals
	// i gotta find out which mesh has normals
	/*for (int i = 0; i < vertices_count; ++i) {
		float x, y, z;
		bb->Read<float>(&x);
		bb->Read<float>(&y);
		bb->Read<float>(&z);

		normals.push_back(vec3(x, y, z));
	}*/

	bb->Position() += 120;

	// read uvs
	for (int i = 0; i < uv_count; ++i) {
		float x, y;
		bb->Read<float>(&x);
		bb->Read<float>(&y);

		uvs.push_back(vec2(x, y));
	}

	uint4 indices_count;
	bb->Read<uint4>(&indices_count); // read indices count

	for (int i = 0; i < indices_count; ++i) {
		int2 v1, v2, v3, vt1, vt2, vt3, vn1, vn2, vn3;
		int2 temp;

		bb->Read<int2>(&temp);
		bb->Read<int2>(&temp);

		bb->Read<int2>(&v1);
		bb->Read<int2>(&v2);
		bb->Read<int2>(&v3);

		bb->Read<int2>(&vt1);
		bb->Read<int2>(&vt2);
		bb->Read<int2>(&vt3);

		//bb->Read<int2>(&vn1);
		//bb->Read<int2>(&vn2);
		//bb->Read<int2>(&vn3);

		indices_verts.push_back(v1 + 1); indices_verts.push_back(v2 + 1); indices_verts.push_back(v3 + 1);
		indices_uvs.push_back(vt1 + 1); indices_uvs.push_back(vt2 + 1); indices_uvs.push_back(vt3 + 1);
		/*indices_normals.push_back(vn1 + 1); indices_normals.push_back(vn2 + 1); indices_normals.push_back(vn3 + 1);*/
	}

	int x = 0;

	std::ofstream out("model.obj");

	if (!out.is_open()) {
		std::cout << "its bad.";
		return -1;
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

	for (int i = 0; i < indices_verts.size(); i += 3) {
		//if (i == 127 * 3) out << "\n\n";
		//out << "f " << indices_verts[i] << "/" << indices_uvs[i] << "/" << indices_normals[i] << " " << indices_verts[i + 1] << "/" << indices_uvs[i + 1] << "/" << indices_normals[i + 1] << " " << indices_verts[i + 2] << "/" << indices_uvs[i + 2] << "/" << indices_normals[i + 2] << "\n";
		out << "f " << indices_verts[i] << "/" << indices_uvs[i] << "/" << indices_verts[i] << " " << indices_verts[i + 1] << "/" << indices_uvs[i + 1] << "/" << indices_verts[i + 1] << " " << indices_verts[i + 2] << "/" << indices_uvs[i + 2] << "/" << indices_verts[i + 2] << "\n";
		//out << "f " << indices_verts[i] << " " << indices_verts[i + 1] << " " << indices_verts[i + 2] << "\n";
	}

	out << "\n";

	return 0;
}
