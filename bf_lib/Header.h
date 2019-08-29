#ifndef BFHEADER_H
#define BFHEADER_H

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

		void ReadFromFile(std::ifstream& in);
	};
}

#endif
