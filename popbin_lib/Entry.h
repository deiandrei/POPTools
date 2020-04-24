#pragma once

#include "defines.h"
#include "ByteBuffer.h"

namespace popbin {
	struct Entry {
		uint4 size;
		int4 magic;
		int4 fileID;
	
		byte1* data; // data.length = byte1 * size

		//For debugging
		uint4 entry_beginPos;
		uint4 entry_endPos;
		uint4 data_beginPos;

		//Utils
		char name[64];
		uint4 name_len;
		char type[64];

		Entry() {
			size = 0;
			magic = 0;
			fileID = 0;
			data = nullptr;

			entry_beginPos = entry_endPos = data_beginPos = 0;
		}

	};

	struct EntryLink {
		Entry* LinkedEntry;
		std::vector<uint4> LinkPositions;

		EntryLink() {
			LinkedEntry = nullptr;
		}
	};
}