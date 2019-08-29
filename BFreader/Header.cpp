#include "Header.h"

namespace bf {
	void Header::ReadFromFile(std::ifstream& in) {
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
}
