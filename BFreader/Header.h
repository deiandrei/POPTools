#pragma once

#include "defines.h"

namespace bf {

	struct Header {
		uint4 magic;
		uint4 unknown1;
		uint4 fcount;
		uint4 dcount;
		int8 unknown2;
		int8 unknown3;
		uint4 capacity;
		uint4 unknown4;
		uint4 main_id;
		uint4 fcount_2;
		uint4 dcount_2;
		uint4 fileIdTableOffset;
		int unknown5;
		uint4 unknown6;
		uint4 last;
		int length;
		int entry;

		void ReadFromFile(std::ifstream& in) {
			std::cout << "Reading header\n";
			in.read((char*)(&magic), sizeof(uint4));
			in.read((char*)(&unknown1), sizeof(uint4));
			in.read((char*)(&fcount), sizeof(uint4));
			in.read((char*)(&dcount), sizeof(uint4));
			in.read((char*)(&unknown2), sizeof(int8));
			in.read((char*)(&unknown3), sizeof(int8));
			in.read((char*)(&capacity), sizeof(uint4));
			in.read((char*)(&unknown4), sizeof(uint4));
			in.read((char*)(&main_id), sizeof(uint4)); // hex
			in.read((char*)(&fcount_2), sizeof(uint4));
			in.read((char*)(&dcount_2), sizeof(uint4));
			in.read((char*)(&fileIdTableOffset), sizeof(uint4));
			in.read((char*)(&unknown5), sizeof(int));
			in.read((char*)(&unknown6), sizeof(uint4));
			in.read((char*)(&last), sizeof(uint4));
			length = (int)in.tellg();
			entry = 0;
		}
	};
}
